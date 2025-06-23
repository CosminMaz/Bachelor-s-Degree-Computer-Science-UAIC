<?php
require_once 'app/helpers/jwt_helper.php';


class UsersController extends Controller {
    private $userModel;

    public function __construct() {
        $this->userModel = $this->model('User');
    }

    public function register() {
        if($_SERVER['REQUEST_METHOD'] == 'POST') {
            // Process form
            $_POST = filter_input_array(INPUT_POST, FILTER_SANITIZE_STRING);

            $data = [
                'nume' => trim($_POST['nume']),
                'prenume' => trim($_POST['prenume']),
                'email' => trim($_POST['email']),
                'password' => trim($_POST['password']),
                'confirm_password' => trim($_POST['confirm_password']),
                'nume_err' => '',
                'prenume_err' => '',
                'email_err' => '',
                'password_err' => '',
                'confirm_password_err' => ''
            ];

            // Validate email
            if(empty($data['email'])) {
                $data['email_err'] = 'Please enter email';
            } else {
                if($this->userModel->findUserByEmail($data['email'])) {
                    $data['email_err'] = 'Email is already taken';
                }
            }

            // Validate nume
            if(empty($data['nume'])) {
                $data['nume_err'] = 'Please enter nume';
            }

            // Validate prenume
            if(empty($data['prenume'])) {
                $data['prenume_err'] = 'Please enter prenume';
            }

            // Validate password
            if(empty($data['password'])) {
                $data['password_err'] = 'Please enter password';
            } elseif(strlen($data['password']) < 6) {
                $data['password_err'] = 'Password must be at least 6 characters';
            }

            // Validate confirm password
            if(empty($data['confirm_password'])) {
                $data['confirm_password_err'] = 'Please confirm password';
            } else {
                if($data['password'] != $data['confirm_password']) {
                    $data['confirm_password_err'] = 'Passwords do not match';
                }
            }

            // Make sure errors are empty
            if(empty($data['email_err']) && empty($data['nume_err']) && empty($data['prenume_err']) && empty($data['password_err']) && empty($data['confirm_password_err'])) {
                // Validated
                if($this->userModel->register($data)) {
                    header('location: ' . URLROOT . '/users/login');
                    exit();
                } else {
                    die('Something went wrong');
                }
            } else {
                // Load view with errors
                $this->view('users/RegisterView', $data);
            }
        } else {
            // Init data
            $data = [
                'nume' => '',
                'prenume' => '',
                'email' => '',
                'password' => '',
                'confirm_password' => '',
                'nume_err' => '',
                'prenume_err' => '',
                'email_err' => '',
                'password_err' => '',
                'confirm_password_err' => ''
            ];

            // Load view
            $this->view('users/RegisterView', $data);
        }
    }

    public function login() {
        if($_SERVER['REQUEST_METHOD'] == 'POST') {
            // Get JSON input
            $json = file_get_contents('php://input');
            $data = json_decode($json, true);

            if (!$data) {
                header('Content-Type: application/json');
                echo json_encode([
                    'status' => 'error',
                    'message' => 'Invalid JSON input'
                ]);
                exit();
            }

            $email = trim($data['email']);
            $password = trim($data['password']);

            // Validate email
            if(empty($email)) {
                header('Content-Type: application/json');
                echo json_encode([
                    'status' => 'error',
                    'message' => 'Please enter email'
                ]);
                exit();
            }

            // Validate password
            if(empty($password)) {
                header('Content-Type: application/json');
                echo json_encode([
                    'status' => 'error',
                    'message' => 'Please enter password'
                ]);
                exit();
            }

            // Check for user/email
            if($this->userModel->findUserByEmail($email)) {
                // User found
            } else {
                header('Content-Type: application/json');
                echo json_encode([
                    'status' => 'error',
                    'message' => 'No user found'
                ]);
                exit();
            }

            // Check and set logged in user
            $loggedInUser = $this->userModel->login($email, $password);

            if($loggedInUser) {
                // Generate JWT token
                $token = JwtHelper::generateToken($loggedInUser);
                
                // Set headers
                header('Content-Type: application/json');
                header('Access-Control-Allow-Headers: Content-Type, Authorization');
                
                // Set token in response header
                header('Authorization: Bearer ' . $token);
                
                // Set token in cookie
                setcookie('token', $token, time() + (86400 * 30), "/"); // 30 days
                
                // Return success response with redirect URL
                $response = [
                    'status' => 'success',
                    'message' => 'Login successful',
                    'token' => $token,
                    'redirect' => URLROOT . '/dashboard',
                    'user' => [
                        'id' => $loggedInUser->id,
                        'email' => $loggedInUser->email,
                        'nume' => $loggedInUser->nume,
                        'prenume' => $loggedInUser->prenume
                    ]
                ];
                
                echo json_encode($response);
                exit();
            } else {
                header('Content-Type: application/json');
                echo json_encode([
                    'status' => 'error',
                    'message' => 'Password incorrect'
                ]);
                exit();
            }
        } else {
            // Init data
            $data = [
                'email' => '',
                'password' => '',
                'email_err' => '',
                'password_err' => ''
            ];

            // Load view
            $this->view('users/LoginView', $data);
        }
    }

    public function logout() {
        // Set headers for JSON response
        header('Content-Type: application/json');
        header('Access-Control-Allow-Origin: *');
        header('Access-Control-Allow-Methods: POST, GET, OPTIONS');
        header('Access-Control-Allow-Headers: Content-Type, Authorization');

        // Clear the token cookie
        setcookie('token', '', time() - 3600, '/');

        // Return success response
        echo json_encode([
            'status' => 'success',
            'message' => 'Logout successful',
            'redirect' => URLROOT . '/users/login'
        ]);
        exit();
    }

    // Returns all users as JSON for AJAX
    public function all() {
        $users = $this->userModel->getAllUsers();
        if (isset($_GET['json'])) {
            header('Content-Type: application/json');
            echo json_encode($users);
            exit;
        }
    }
}