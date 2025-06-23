document.addEventListener('DOMContentLoaded', function() {
    console.log('DOM loaded');
    const loginForm = document.getElementById('loginForm');
    console.log('Login form:', loginForm);

    loginForm.addEventListener('submit', async function(e) {
        e.preventDefault();
        console.log('Form submitted');
        
        const email = document.getElementById('email').value;
        const password = document.getElementById('password').value;
        
        console.log('Email:', email);
        console.log('Password:', password);
        
        try {
            // URL root should be injected by PHP in the HTML script tag
            const urlRoot = window.urlRoot || '';
            const response = await fetch(urlRoot + '/users/login', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    email: email,
                    password: password
                })
            });
            
            console.log('Response received:', response);
            
            const data = await response.json();
            console.log('Data received:', data);
            
            if (data.status === 'success') {
                console.log('Login successful, storing token and redirecting');
                localStorage.setItem('token', data.token);
                
                // Get user's location
                if (navigator.geolocation) {
                    navigator.geolocation.getCurrentPosition(
                        function(position) {
                            console.log('%c Latitudine: ' + position.coords.latitude, 'color: #4CAF50; font-size: 14px; font-weight: bold;');
                            console.log('%c Longitudine: ' + position.coords.longitude, 'color: #4CAF50; font-size: 14px; font-weight: bold;');
                            // Redirect after getting position
                            window.location.href = data.redirect;
                        },
                        function(error) {
                            console.error('Eroare la obținerea locației:', error.message);
                            // Redirect even if there's an error
                            window.location.href = data.redirect;
                        }
                    );
                } else {
                    console.log('Geolocation nu este suportat de acest browser.');
                    window.location.href = data.redirect;
                }
            } else {
                console.log('Login failed:', data.message);
                document.getElementById('error-message').textContent = data.message;
                document.getElementById('error-message').style.display = 'block';
            }
        } catch (error) {
            console.error('Error during login:', error);
            document.getElementById('error-message').textContent = 'A apărut o eroare. Vă rugăm să încercați din nou.';
            document.getElementById('error-message').style.display = 'block';
        }
    });
});
