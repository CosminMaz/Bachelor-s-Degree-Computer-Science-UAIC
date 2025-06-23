<?php
// Load the HTML template
$templatePath = __DIR__ . '/AddPropertyTemplate.html';
$htmlContent = file_get_contents($templatePath);

// Replace placeholders with dynamic data
$htmlContent = str_replace('{URLROOT}', URLROOT, $htmlContent);
$htmlContent = str_replace('{TITLE_CLASS}', (!empty($data['title_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{TITLE_VALUE}', $data['title'], $htmlContent);
$htmlContent = str_replace('{TITLE_ERR}', $data['title_err'], $htmlContent);
$htmlContent = str_replace('{DESCRIPTION_VALUE}', $data['description'], $htmlContent);
$htmlContent = str_replace('{PRICE_CLASS}', (!empty($data['price_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{PRICE_VALUE}', $data['price'], $htmlContent);
$htmlContent = str_replace('{PRICE_ERR}', $data['price_err'], $htmlContent);
$htmlContent = str_replace('{SUPRAFATA_CLASS}', (!empty($data['suprafata_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{SUPRAFATA_VALUE}', $data['suprafata'], $htmlContent);
$htmlContent = str_replace('{SUPRAFATA_ERR}', $data['suprafata_err'], $htmlContent);
$htmlContent = str_replace('{LATITUDE_CLASS}', (!empty($data['latitude_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{LATITUDE_VALUE}', $data['latitude'], $htmlContent);
$htmlContent = str_replace('{LATITUDE_ERR}', $data['latitude_err'], $htmlContent);
$htmlContent = str_replace('{LONGITUDE_CLASS}', (!empty($data['longitude_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{LONGITUDE_VALUE}', $data['longitude'], $htmlContent);
$htmlContent = str_replace('{LONGITUDE_ERR}', $data['longitude_err'], $htmlContent);
$htmlContent = str_replace('{CONTACT_CLASS}', (!empty($data['contact_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{CONTACT_VALUE}', $data['contact'], $htmlContent);
$htmlContent = str_replace('{CONTACT_ERR}', $data['contact_err'], $htmlContent);
$htmlContent = str_replace('{STATUS_CLASS}', (!empty($data['status_err'])) ? 'is-invalid' : '', $htmlContent);
$htmlContent = str_replace('{VANZARE_SELECTED}', ($data['status'] == 'vanzare') ? 'selected' : '', $htmlContent);
$htmlContent = str_replace('{INCHIRIERE_SELECTED}', ($data['status'] == 'inchiriere') ? 'selected' : '', $htmlContent);
$htmlContent = str_replace('{STATUS_ERR}', $data['status_err'], $htmlContent);
$htmlContent = str_replace('{FACILITIES_VALUE}', $data['facilities'], $htmlContent);
$htmlContent = str_replace('{RISKS_VALUE}', $data['risks'], $htmlContent);

// Output the final HTML
echo $htmlContent;
?>
