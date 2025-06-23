<!--
This is the dashboard controller.
It is used to display the dashboard page.
It is used to check if the user is authenticated and if the token is valid.
If the user is not authenticated or the token is invalid, it redirects to the login page.
If the user is authenticated and the token is valid, it displays the dashboard page.
-->
<?php
require_once 'app/helpers/jwt_helper.php';

class DashboardController extends Controller {
    private $propertyModel;

    public function __construct() {
        // Check if user is authenticated
        $token = JwtHelper::getTokenFromHeader();
        
        // If no token in header, check cookie
        if (!$token && isset($_COOKIE['token'])) {
            $token = $_COOKIE['token'];
        }

        if (!$token) {
            // If no token found, redirect to login
            header('Location: ' . URLROOT . '/users/login');
            exit();
        }

        // Validate token
        $decoded = JwtHelper::validateToken($token);
        if (!$decoded) {
            // If token is invalid, redirect to login
            header('Location: ' . URLROOT . '/users/login');
            exit();
        }

        $this->propertyModel = $this->model('Property');
    }

    public function index() {
        // Get token from header or cookie
        $token = JwtHelper::getTokenFromHeader();
        
        if (!$token && isset($_COOKIE['token'])) {
            $token = $_COOKIE['token'];
        }
        
        // Validate token
        $decoded = JwtHelper::validateToken($token);
        if (!$decoded) {
            header('Location: ' . URLROOT . '/users/login');
            exit();
        }
        
        $user = $decoded->user;
        $properties = $user->role === 'admin' ? $this->propertyModel->getAllProperties() : $this->propertyModel->getAllPropertiesLatLng();
        $data = [
            'title' => $user->role === 'admin' ? 'Admin Dashboard' : 'Dashboard',
            'user' => $user,
            'properties' => $properties
        ];

        if ($user->role === 'admin') {
            $userModel = $this->model('User');
            $data['users'] = $this->getAllUsers($userModel);
            $this->view('dashboard/AdminDashboardView', $data);
        } else {
            $this->view('dashboard/DashboardView', $data);
        }
    }

    private function getAllUsers($userModel) {
        $this->db = new Database;
        $this->db->query('SELECT id, nume, prenume, email, role FROM users');
        return $this->db->resultSet();
    }
}
