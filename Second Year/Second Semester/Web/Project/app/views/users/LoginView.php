<?php
// Load the HTML template
$templatePath = __DIR__ . '/LoginTemplate.html';
$htmlContent = file_get_contents($templatePath);

// Replace placeholders with dynamic data
$htmlContent = str_replace('/TW/Proiect-Web', URLROOT, $htmlContent);
$htmlContent = str_replace('{EMAIL_CLASS}', (!empty($data['email_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{EMAIL_VALUE}', $data['email'], $htmlContent);
$htmlContent = str_replace('{EMAIL_ERR}', $data['email_err'], $htmlContent);
$htmlContent = str_replace('{PASSWORD_CLASS}', (!empty($data['password_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{PASSWORD_VALUE}', $data['password'], $htmlContent);
$htmlContent = str_replace('{PASSWORD_ERR}', $data['password_err'], $htmlContent);

// Output the final HTML
echo $htmlContent;
?>
