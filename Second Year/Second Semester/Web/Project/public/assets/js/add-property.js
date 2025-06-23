document.addEventListener('DOMContentLoaded', function() {
    const form = document.getElementById('add-property-form');
    const messageDiv = document.getElementById('form-message');

    form.addEventListener('submit', async function(e) {
        e.preventDefault();
        // Clear previous errors
        document.querySelectorAll('.invalid-feedback').forEach(el => el.textContent = '');
        messageDiv.textContent = '';
        messageDiv.style.color = '';

        const formData = new FormData(form);
        const data = {};
        formData.forEach((value, key) => {
            data[key] = value;
        });

        try {
            const formData = new FormData(form);
            const params = new URLSearchParams();
            for (const pair of formData.entries()) {
                params.append(pair[0], pair[1]);
            }
            const response = await fetch('/TW/Proiect-Web/properties/add', {
                method: 'POST',
                headers: {
                    'Accept': 'application/json'
                },
                body: params
            });
            let result = null;
            let isJson = false;
            try {
                result = await response.clone().json();
                isJson = true;
            } catch (e) {
                isJson = false;
            }
            if (isJson && result && result.status === 'success') {
                window.location.href = '/TW/Proiect-Web/dashboard';
                return;
            } else if (isJson && result && result.errors) {
                // Show validation errors
                for (const key in result.errors) {
                    const errorSpan = document.getElementById(key + '-error');
                    if (errorSpan) {
                        errorSpan.textContent = result.errors[key];
                    }
                }
                messageDiv.textContent = result.message || 'Eroare la validare!';
                messageDiv.style.color = 'red';
            } else if (response.redirected || response.status === 302 || response.status === 301) {
                window.location.href = '/TW/Proiect-Web/dashboard';
                return;
            } else if (response.ok) {
                // If response is ok but not JSON, treat as success (property added)
                window.location.href = '/TW/Proiect-Web/dashboard';
                return;
            } else {
                messageDiv.textContent = 'Eroare la adăugare!';
                messageDiv.style.color = 'red';
            }
        } catch (err) {
            messageDiv.textContent = 'Eroare de rețea sau server!';
            messageDiv.style.color = 'red';
        }
    });
});
