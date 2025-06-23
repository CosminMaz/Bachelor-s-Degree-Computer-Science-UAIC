<?php
// Load the HTML template
$templatePath = __DIR__ . '/HomeTemplate.html';
$htmlContent = file_get_contents($templatePath);

// Replace placeholders with dynamic data
$htmlContent = str_replace('{URLROOT}', URLROOT, $htmlContent);

// Output the final HTML
echo $htmlContent;
?>
