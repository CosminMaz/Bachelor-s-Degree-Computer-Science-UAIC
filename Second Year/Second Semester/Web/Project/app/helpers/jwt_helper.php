<?php
require_once __DIR__ . '/../../vendor/autoload.php';
use \Firebase\JWT\JWT;
use \Firebase\JWT\Key;

class JwtHelper {
    private static $key = "cam-real-estate-secret-key"; // Secure key
    private static $algorithm = 'HS256';

    public static function generateToken($userData) {
        $issuedAt = time();
        $expirationTime = $issuedAt + 3600; // Token valid for 1 hour

        $payload = array(
            "iat" => $issuedAt,
            "exp" => $expirationTime,
            "user" => array(
                "id" => $userData->id,
                "email" => $userData->email,
                "nume" => $userData->nume,
                "prenume" => $userData->prenume,
                "role" => isset($userData->role) ? $userData->role : 'user'
            )
        );

        return JWT::encode($payload, self::$key, self::$algorithm);
    }

    public static function validateToken($token) {
        if (!$token) {
            return false;
        }

        try {
            $decoded = JWT::decode($token, new Key(self::$key, self::$algorithm));
            return $decoded;
        } catch (Exception $e) {
            return false;
        }
    }

    public static function getTokenFromHeader() {
        $headers = getallheaders();
        if (isset($headers['Authorization'])) {
            $authHeader = $headers['Authorization'];
            if (preg_match('/Bearer\s(\S+)/', $authHeader, $matches)) {
                return $matches[1];
            }
        }
        return null;
    }
}
