<?php
// Load the HTML template
$templatePath = __DIR__ . '/RegisterTemplate.html';
$htmlContent = file_get_contents($templatePath);

// Replace placeholders with dynamic data
$htmlContent = str_replace('/TW/Proiect-Web', URLROOT, $htmlContent);
$htmlContent = str_replace('{NUME_CLASS}', (!empty($data['nume_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{NUME_VALUE}', $data['nume'], $htmlContent);
$htmlContent = str_replace('{NUME_ERR}', $data['nume_err'], $htmlContent);
$htmlContent = str_replace('{PRENUME_CLASS}', (!empty($data['prenume_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{PRENUME_VALUE}', $data['prenume'], $htmlContent);
$htmlContent = str_replace('{PRENUME_ERR}', $data['prenume_err'], $htmlContent);
$htmlContent = str_replace('{EMAIL_CLASS}', (!empty($data['email_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{EMAIL_VALUE}', $data['email'], $htmlContent);
$htmlContent = str_replace('{EMAIL_ERR}', $data['email_err'], $htmlContent);
$htmlContent = str_replace('{PASSWORD_CLASS}', (!empty($data['password_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{PASSWORD_VALUE}', $data['password'], $htmlContent);
$htmlContent = str_replace('{PASSWORD_ERR}', $data['password_err'], $htmlContent);
$htmlContent = str_replace('{CONFIRM_PASSWORD_CLASS}', (!empty($data['confirm_password_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{CONFIRM_PASSWORD_VALUE}', $data['confirm_password'], $htmlContent);
$htmlContent = str_replace('{CONFIRM_PASSWORD_ERR}', $data['confirm_password_err'], $htmlContent);

// Output the final HTML
echo $htmlContent;
?>
