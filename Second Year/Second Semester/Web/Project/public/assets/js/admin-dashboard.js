// Ensure old token key is removed
localStorage.removeItem('jwt_token');

// Add token to all future requests
const token = localStorage.getItem('token');
if (token) {
    // Add token to all fetch requests
    const originalFetch = window.fetch;
    window.fetch = function() {
        let [resource, config] = arguments;
        if (config === undefined) {
            config = {};
        }
        if (config.headers === undefined) {
            config.headers = {};
        }
        config.headers['Authorization'] = 'Bearer ' + token;
        return originalFetch(resource, config);
    };
}

// OpenStreetMap + Leaflet map with property markers
document.addEventListener('DOMContentLoaded', function() {
    var properties = window.propertiesData || [];
    var map;
    function addMarkers(map, properties) {
        properties.forEach(function(prop) {
            if (prop.latitude && prop.longitude) {
                L.marker([prop.latitude, prop.longitude]).addTo(map)
                    .bindPopup('<b>' + prop.title + '</b>');
            }
        });
    }
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(function(position) {
            var lat = position.coords.latitude;
            var lng = position.coords.longitude;
            map = L.map('map').setView([lat, lng], 13);
            L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                maxZoom: 19,
                attribution: '© OpenStreetMap contributors'
            }).addTo(map);
            L.marker([lat, lng]).addTo(map)
                .bindPopup('Locația ta curentă').openPopup();
            addMarkers(map, properties);
        }, function(error) {
            map = L.map('map').setView([45.9432, 24.9668], 6); // Romania
            L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                maxZoom: 19,
                attribution: '© OpenStreetMap contributors'
            }).addTo(map);
            addMarkers(map, properties);
        });
    } else {
        map = L.map('map').setView([45.9432, 24.9668], 6); // Romania
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            maxZoom: 19,
            attribution: '© OpenStreetMap contributors'
        }).addTo(map);
        addMarkers(map, properties);
    }

    // Fetch all properties and add markers to map
    fetch('/TW/Proiect-Web/properties/all?json=1')
        .then(response => response.json())
        .then(properties => {
            if (typeof map === 'undefined') return;
            properties.forEach(function(prop) {
                if (prop.latitude && prop.longitude) {
                    L.marker([prop.latitude, prop.longitude]).addTo(map)
                        .bindPopup('<b>' + prop.title + '</b>');
                }
            });
        });
});

// Get users location when dashboard loads
if (navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(
        function(position) {
            console.log('%c Latitudine: ' + position.coords.latitude, 'color: #4CAF50; font-size: 14px; font-weight: bold;');
            console.log('%c Longitudine: ' + position.coords.longitude, 'color: #4CAF50; font-size: 14px; font-weight: bold;');
        },
        function(error) {
            console.error('Eroare la obținerea locației:', error.message);
        }
    );
} else {
    console.log('Geolocation nu este suportat de acest browser.');
}

// Handle logout
document.getElementById('logoutBtn').addEventListener('click', async function(e) {
    e.preventDefault();
    
    try {
        // URL root should be injected by PHP in the HTML script tag
        const urlRoot = window.urlRoot || '';
        const response = await fetch(urlRoot + '/users/logout', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': 'Bearer ' + token
            }
        });

        const text = await response.text();
        let data;
        try {
            data = JSON.parse(text);
        } catch (e) {
            console.error('JSON parse error:', e, 'Response text:', text);
            throw new Error('Invalid JSON response from server');
        }
        
        if (data.status === 'success') {
            // Clear local storage
            localStorage.removeItem('token');
            // Clear any cookies
            document.cookie = 'token=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;';
            
            // Redirect to login page
            window.location.href = data.redirect;
        } else {
            console.error('Logout failed:', data.message);
        }
    } catch (error) {
        console.error('Error during logout:', error);
        localStorage.removeItem('token');
        document.cookie = 'token=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;';
        // Fallback redirect in case of error
        const urlRoot = window.urlRoot || '';
        window.location.href = urlRoot + '/users/login';
    }
});

async function deleteProperty(propertyId) {
    if (confirm('Ești sigur că vrei să ștergi această proprietate?')) {
        try {
            // URL root should be injected by PHP in the HTML script tag
            const urlRoot = window.urlRoot || '';
            const response = await fetch(urlRoot + '/properties/delete/' + propertyId, {
                method: 'DELETE',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': 'Bearer ' + token
                }
            });

            // Check if response is ok before parsing JSON
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }

            const text = await response.text();
            let data;
            try {
                data = JSON.parse(text);
            } catch (e) {
                console.error('JSON parse error:', e, 'Response text:', text);
                throw new Error('Invalid JSON response from server');
            }
            
            if (data.status === 'success') {
                alert('Proprietatea a fost ștersă cu succes.');
                window.location.reload();
            } else {
                alert('Eroare la ștergerea proprietății: ' + (data.message || 'Unknown error'));
            }
        } catch (error) {
            console.error('Error deleting property:', error);
            alert('Eroare la ștergerea proprietății. Verifică consola pentru detalii.');
        }
    }
}

// Script for admin dashboard: dynamic loading of properties and users, property deletion

document.addEventListener('DOMContentLoaded', function() {
    // Fetch properties
    fetch('/TW/Proiect-Web/properties/all?json=1')
        .then(response => response.json())
        .then(data => {
            const tbody = document.getElementById('properties-table-body');
            tbody.innerHTML = '';
            data.forEach(prop => {
                tbody.innerHTML += `<tr>
                    <td>${prop.id}</td>
                    <td>${prop.title}</td>
                    <td>${prop.description}</td>
                    <td><button class="delete-property-btn" data-id="${prop.id}">Șterge</button></td>
                </tr>`;
            });
            // Add event listener for delete buttons
            tbody.querySelectorAll('.delete-property-btn').forEach(btn => {
                btn.addEventListener('click', function() {
                    if(confirm('Sigur vrei să ștergi această proprietate?')) {
                        fetch(`/TW/Proiect-Web/properties/delete/${this.dataset.id}`, {
                            method: 'DELETE',
                            headers: {
                                'Content-Type': 'application/json'
                            }
                        })
                        .then(response => response.json())
                        .then(result => {
                            if(result.status === 'success') {
                                this.closest('tr').remove();
                            } else {
                                alert(result.message || 'Eroare la ștergere!');
                            }
                        });
                    }
                });
            });
        });
    // Fetch users
    fetch('/TW/Proiect-Web/users/all?json=1')
        .then(response => response.json())
        .then(data => {
            const tbody = document.getElementById('users-table-body');
            tbody.innerHTML = '';
            data.forEach(user => {
                tbody.innerHTML += `<tr>
                    <td>${user.id}</td>
                    <td>${user.nume}</td>
                    <td>${user.email}</td>
                    <td>${user.role}</td>
                </tr>`;
            });
        });
    window.urlRoot = '/TW/Proiect-Web';
});
