<?php
// Load the HTML template
$templatePath = __DIR__ . '/DashboardTemplate.html';
$htmlContent = file_get_contents($templatePath);

// Replace placeholders with dynamic data
$userName = isset($data['user']->nume) ? htmlspecialchars($data['user']->nume) : 'Utilizator';
$htmlContent = str_replace('{USER_NAME}', $userName, $htmlContent);
$htmlContent = str_replace('{URLROOT}', URLROOT, $htmlContent);
$htmlContent = str_replace('{PROPERTIES_DATA}', json_encode($data['properties']), $htmlContent);

// Output the final HTML
echo $htmlContent;
?>
