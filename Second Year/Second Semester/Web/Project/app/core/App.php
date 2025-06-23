<?php
class App {
    // Default controller and method (and its parameters)
    protected $controller = 'HomeController';
    protected $method = 'index';
    protected $params = [];

    // Constructor
    public function __construct() {
        $url = $this->parseUrl(); // Parse the URL to get the controller, method, and parameters

        // First element is the controller
        if(isset($url[0])) {
            if(file_exists('app/controllers/' . ucwords($url[0]) . 'Controller.php')) {
                $this->controller = ucwords($url[0]) . 'Controller'; // Set the controller
                unset($url[0]); // Remove the controller from the URL
            }
        }
        // load the controller
        require_once 'app/controllers/' . $this->controller . '.php';
        $this->controller = new $this->controller; // Instantiate the controller

        // Second element is the method
        if(isset($url[1])) {
            if(method_exists($this->controller, $url[1])) {
                $this->method = $url[1];
                unset($url[1]);
            }
        }

        // The rest are parameters
        $this->params = $url ? array_values($url) : [];

        // Call the controller method with parameters
        call_user_func_array([$this->controller, $this->method], $this->params);
    }

    // Parse the URL to get the controller, method, and parameters
    public function parseUrl() {
        if(isset($_GET['url'])) {
            $url = rtrim($_GET['url'], '/');
            $url = filter_var($url, FILTER_SANITIZE_URL);
            $url = explode('/', $url);
            return $url;
        }
        return ['home'];
    }
} 