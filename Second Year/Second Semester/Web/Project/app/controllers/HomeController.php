<!--
This is the home controller.
It handles the home page and the about page.
-->
<?php
class HomeController extends Controller {
    public function __construct() {}
    // Load the home page
    public function index() {
        $this->view('home/HomeView');
    }
} 