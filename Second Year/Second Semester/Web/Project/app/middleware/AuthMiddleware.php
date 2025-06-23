<?php
require_once 'app/helpers/jwt_helper.php';

class AuthMiddleware {
    public static function authenticate() {
        $token = JwtHelper::getTokenFromHeader();
        
        if (!$token) {
            header('HTTP/1.0 401 Unauthorized');
            echo json_encode(['error' => 'No token provided']);
            exit();
        }

        $decoded = JwtHelper::validateToken($token);
        
        if (!$decoded) {
            header('HTTP/1.0 401 Unauthorized');
            echo json_encode(['error' => 'Invalid token']);
            exit();
        }

        return $decoded;
    }
} 