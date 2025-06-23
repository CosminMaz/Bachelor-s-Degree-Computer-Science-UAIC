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
                .bindPopup('Your current location').openPopup();
            addMarkers(map, properties);
        }, function(error) {
            map = L.map('map').setView([45.9432, 24.9668], 6); 
            L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                maxZoom: 19,
                attribution: '© OpenStreetMap contributors'
            }).addTo(map);
            addMarkers(map, properties);
        });
    } else {
        map = L.map('map').setView([45.9432, 24.9668], 6); 
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            maxZoom: 19,
            attribution: '© OpenStreetMap contributors'
        }).addTo(map);
        addMarkers(map, properties);
    }
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

// Logout functionality is now handled by auth-utils.js
