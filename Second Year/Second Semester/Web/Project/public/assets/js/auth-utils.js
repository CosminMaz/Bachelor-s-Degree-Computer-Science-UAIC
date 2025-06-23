/**
 * Utility functions for authentication and authorization
 */

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

/**
 * Handle user logout
 * @param {Event} e - The click event
 */
function handleLogout(e) {
    e.preventDefault();
    
    try {
        // URL root should be injected by PHP in the HTML script tag
        const urlRoot = window.urlRoot || '';
        const response = fetch(urlRoot + '/users/logout', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': 'Bearer ' + localStorage.getItem('token')
            }
        })
        .then(response => response.json())
        .then(data => {
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
        })
        .catch(error => {
            console.error('Error during logout:', error);
            // Even if there's an error, try to clear storage and redirect
            localStorage.removeItem('token');
            document.cookie = 'token=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;';
            // Fallback redirect in case of error
            const urlRoot = window.urlRoot || '';
            window.location.href = urlRoot + '/users/login';
        });
    } catch (error) {
        console.error('Error during logout:', error);
        // Even if there's an error, try to clear storage and redirect
        localStorage.removeItem('token');
        document.cookie = 'token=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;';
        // Fallback redirect in case of error
        const urlRoot = window.urlRoot || '';
        window.location.href = urlRoot + '/users/login';
    }
}

// Attach logout handler to the logout button
document.addEventListener('DOMContentLoaded', function() {
    const logoutBtn = document.getElementById('logoutBtn');
    if (logoutBtn) {
        logoutBtn.addEventListener('click', handleLogout);
    }
});
