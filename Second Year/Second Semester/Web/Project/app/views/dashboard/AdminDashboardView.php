<?php
// Load the HTML template
$templatePath = __DIR__ . '/AdminDashboardTemplate.html';
$htmlContent = file_get_contents($templatePath);

// Replace placeholders with dynamic data
$userName = isset($data['user']->nume) ? htmlspecialchars($data['user']->nume) : 'Utilizator';
$htmlContent = str_replace('{USER_NAME}', $userName, $htmlContent);
$htmlContent = str_replace('{URLROOT}', URLROOT, $htmlContent);
$htmlContent = str_replace('{PROPERTIES_DATA}', json_encode($data['properties']), $htmlContent);

// Generate properties table content
$propertiesTable = '';
foreach ($data['properties'] as $property) {
    $propertiesTable .= '<tr>';
    $propertiesTable .= '<td>' . htmlspecialchars($property->id) . '</td>';
    $propertiesTable .= '<td>' . htmlspecialchars($property->title) . '</td>';
    $propertiesTable .= '<td>' . htmlspecialchars(substr($property->description, 0, 50)) . (strlen($property->description) > 50 ? '...' : '') . '</td>';
    $propertiesTable .= '<td><button style="background:none; border:none; color:#ff0000; cursor:pointer; font-size:14px;" onclick="deleteProperty(' . $property->id . ')">Șterge</button></td>';
    $propertiesTable .= '</tr>';
}
$htmlContent = str_replace('{PROPERTIES_TABLE}', $propertiesTable, $htmlContent);

// Generate users table content
$usersTable = '';
foreach ($data['users'] as $user) {
    $id = is_object($user) ? $user->id : $user['id'];
    $nume = is_object($user) ? $user->nume : $user['nume'];
    $prenume = is_object($user) ? $user->prenume : $user['prenume'];
    $email = is_object($user) ? $user->email : $user['email'];
    $role = is_object($user) ? $user->role : (isset($user['role']) ? $user['role'] : '');
    $usersTable .= '<tr>';
    $usersTable .= '<td>' . htmlspecialchars($id) . '</td>';
    $usersTable .= '<td>' . htmlspecialchars($nume . ' ' . $prenume) . '</td>';
    $usersTable .= '<td>' . htmlspecialchars($email) . '</td>';
    $usersTable .= '<td>' . htmlspecialchars($role) . '</td>';
    $usersTable .= '</tr>';
}
$htmlContent = str_replace('{USERS_TABLE}', $usersTable, $htmlContent);

// Output the final HTML
echo $htmlContent;
?>
