<?php
require_once 'app/helpers/jwt_helper.php';

class PropertiesController extends Controller {
    private $propertyModel;

    public function __construct() {
        // Check if user is authenticated
        $token = JwtHelper::getTokenFromHeader();
        
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

    public function search() {
        // Preia filtrele din GET
        $status = isset($_GET['status']) ? $_GET['status'] : '';
        $min_price = isset($_GET['min_price']) && $_GET['min_price'] !== '' ? floatval($_GET['min_price']) : null;
        $max_price = isset($_GET['max_price']) && $_GET['max_price'] !== '' ? floatval($_GET['max_price']) : null;
        $min_suprafata = isset($_GET['min_suprafata']) && $_GET['min_suprafata'] !== '' ? intval($_GET['min_suprafata']) : null;

        // Obține proprietățile filtrate
        $properties = $this->propertyModel->getFilteredProperties($status, $min_price, $max_price, $min_suprafata);

        $data = [
            'title' => 'Caută Proprietăți',
            'properties' => $properties,
            'status' => $status,
            'min_price' => $min_price,
            'max_price' => $max_price,
            'min_suprafata' => $min_suprafata
        ];

        $this->view('properties/SearchView', $data);
    }

    public function add() {
        if ($_SERVER['REQUEST_METHOD'] == 'POST') {
            // Process form
            $_POST = filter_input_array(INPUT_POST, FILTER_SANITIZE_STRING);

            // Get user ID from token
            $token = JwtHelper::getTokenFromHeader();
            if (!$token && isset($_COOKIE['token'])) {
                $token = $_COOKIE['token'];
            }
            $decoded = JwtHelper::validateToken($token);

            // Init data
            $data = [
                'title' => trim($_POST['title']),
                'description' => trim($_POST['description']),
                'price' => trim($_POST['price']),
                'suprafata' => trim($_POST['suprafata']),
                'latitude' => trim($_POST['latitude']),
                'longitude' => trim($_POST['longitude']),
                'contact' => trim($_POST['contact']),
                'status' => trim($_POST['status']),
                'facilities' => trim($_POST['facilities']),
                'risks' => trim($_POST['risks']),
                'user_id' => $decoded->user->id,
                'title_err' => '',
                'price_err' => '',
                'suprafata_err' => '',
                'latitude_err' => '',
                'longitude_err' => '',
                'contact_err' => '',
                'status_err' => ''
            ];

            // Validate title
            if (empty($data['title'])) {
                $data['title_err'] = 'Vă rugăm să introduceți titlul';
            }

            // Validate price
            if (empty($data['price'])) {
                $data['price_err'] = 'Vă rugăm să introduceți prețul';
            }

            // Validate suprafata
            if (empty($data['suprafata'])) {
                $data['suprafata_err'] = 'Vă rugăm să introduceți suprafața';
            }

            // Validate latitude
            if (empty($data['latitude'])) {
                $data['latitude_err'] = 'Vă rugăm să introduceți latitudinea';
            }

            // Validate longitude
            if (empty($data['longitude'])) {
                $data['longitude_err'] = 'Vă rugăm să introduceți longitudinea';
            }

            // Validate contact
            if (empty($data['contact'])) {
                $data['contact_err'] = 'Vă rugăm să introduceți datele de contact';
            }

            // Validate status
            if (empty($data['status'])) {
                $data['status_err'] = 'Vă rugăm să selectați statusul';
            }

            // Make sure errors are empty
            if (empty($data['title_err']) && empty($data['price_err']) && 
                empty($data['suprafata_err']) && empty($data['latitude_err']) && 
                empty($data['longitude_err']) && empty($data['contact_err']) && 
                empty($data['status_err'])) {
                
                // Add property
                if ($this->propertyModel->addProperty([
                    'title' => $data['title'],
                    'description' => $data['description'],
                    'price' => $data['price'],
                    'suprafata' => $data['suprafata'],
                    'latitude' => $data['latitude'],
                    'longitude' => $data['longitude'],
                    'contact' => $data['contact'],
                    'status' => $data['status'],
                    'facilities' => $data['facilities'],
                    'risks' => $data['risks'],
                    'user_id' => $data['user_id']
                ])) {
                    // Redirect to properties search
                    header('Location: ' . URLROOT . '/properties/search');
                    exit();
                } else {
                    die('Something went wrong');
                }
            } else {
                // Load view with errors
                $this->view('properties/AddView', $data);
            }
        } else {
            // Init data
            $data = [
                'title' => '',
                'description' => '',
                'price' => '',
                'suprafata' => '',
                'latitude' => '',
                'longitude' => '',
                'contact' => '',
                'status' => '',
                'facilities' => '',
                'risks' => '',
                'title_err' => '',
                'price_err' => '',
                'suprafata_err' => '',
                'latitude_err' => '',
                'longitude_err' => '',
                'contact_err' => '',
                'status_err' => ''
            ];

            // Load view
            $this->view('properties/AddView', $data);
        }
    }

    public function index() {
        // Redirect to search as default action for properties
        header('Location: ' . URLROOT . '/properties/search');
        exit();
    }
    //Make the xml file for exprot with the properties
    public function export($format = 'xml') {
        if ($_SERVER['REQUEST_METHOD'] == 'GET') {
            // Get token from header or cookie
            $token = JwtHelper::getTokenFromHeader();
            if (!$token && isset($_COOKIE['token'])) {
                $token = $_COOKIE['token'];
            }
            $decoded = JwtHelper::validateToken($token);

            // Check if user is admin
            if ($decoded && isset($decoded->user->role) && $decoded->user->role === 'admin') {
                if ($format === 'xml') {
                    $properties = $this->propertyModel->getAllProperties();
                    $xml = '<?xml version="1.0" encoding="UTF-8"?>' . PHP_EOL;
                    $xml .= '<properties>' . PHP_EOL;
                    foreach ($properties as $prop) {
                        $xml .= '    <property>' . PHP_EOL;
                        $xml .= '        <id>' . htmlspecialchars($prop->id) . '</id>' . PHP_EOL;
                        $xml .= '        <title>' . htmlspecialchars($prop->title) . '</title>' . PHP_EOL;
                        $xml .= '        <description>' . htmlspecialchars($prop->description) . '</description>' . PHP_EOL;
                        if ($prop->latitude) {
                            $xml .= '        <latitude>' . htmlspecialchars($prop->latitude) . '</latitude>' . PHP_EOL;
                        }
                        if ($prop->longitude) {
                            $xml .= '        <longitude>' . htmlspecialchars($prop->longitude) . '</longitude>' . PHP_EOL;
                        }
                        $xml .= '    </property>' . PHP_EOL;
                    }
                    $xml .= '</properties>';
                    
                    header('Content-Type: application/xml');
                    header('Content-Disposition: attachment; filename="properties.xml"');
                    echo $xml;
                    exit();
                } else {
                    echo json_encode(['status' => 'error', 'message' => 'Unsupported format']);
                    exit();
                }
            } else {
                header('Location: ' . URLROOT . '/users/login');
                exit();
            }
        } else {
            header('Location: ' . URLROOT . '/dashboard');
            exit();
        }
    }

    public function delete($id) {
        if ($_SERVER['REQUEST_METHOD'] == 'DELETE') {
            // Get token from header or cookie
            $token = JwtHelper::getTokenFromHeader();
            if (!$token && isset($_COOKIE['token'])) {
                $token = $_COOKIE['token'];
            }
            $decoded = JwtHelper::validateToken($token);

            // Check if user is admin
            if ($decoded && isset($decoded->user->role) && $decoded->user->role === 'admin') {
                if ($this->propertyModel->deleteProperty($id)) {
                    echo json_encode(['status' => 'success', 'message' => 'Property deleted successfully']);
                } else {
                    echo json_encode(['status' => 'error', 'message' => 'Failed to delete property']);
                }
            } else {
                echo json_encode(['status' => 'error', 'message' => 'Unauthorized access']);
            }
            exit();
        } else {
            header('Location: ' . URLROOT . '/dashboard');
            exit();
        }
    }
    // Returns all properties as JSON for AJAX
    public function all() {
        $properties = $this->propertyModel->getAllProperties();
        if (isset($_GET['json'])) {
            header('Content-Type: application/json');
            echo json_encode($properties);
            exit;
        }
        // You can also add a view if you want to display in the browser
    }
}
