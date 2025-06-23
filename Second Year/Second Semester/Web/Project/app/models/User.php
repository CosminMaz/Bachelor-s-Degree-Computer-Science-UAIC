<?php
class User {
    private $db;

    public function __construct() {
        $this->db = new Database;
    }

    public function login($email, $password) {
        $this->db->query('SELECT * FROM users WHERE email = :email');
        $this->db->bind(':email', $email);

        $row = $this->db->single();

        if($row) {
            $hashed_password = $row->password;
            if(password_verify($password, $hashed_password)) {
                return $row;
            }
        }
        return false;
    }

    public function getUserRole($userId) {
        $this->db->query('SELECT role FROM users WHERE id = :id');
        $this->db->bind(':id', $userId);
        $row = $this->db->single();
        return $row ? $row->role : 'user';
    }

    public function register($data) {
        $this->db->query('INSERT INTO users (nume, prenume, email, password) VALUES(:nume, :prenume, :email, :password)');
        $this->db->bind(':nume', $data['nume']);
        $this->db->bind(':prenume', $data['prenume']);
        $this->db->bind(':email', $data['email']);
        $this->db->bind(':password', password_hash($data['password'], PASSWORD_DEFAULT));

        if($this->db->execute()) {
            return true;
        } else {
            return false;
        }
    }

    public function findUserByEmail($email) {
        $this->db->query('SELECT * FROM users WHERE email = :email');
        $this->db->bind(':email', $email);

        $row = $this->db->single();

        if($this->db->rowCount() > 0) {
            return true;
        } else {
            return false;
        }
    }

    // Returns all users from the database
    public function getAllUsers() {
        $this->db->query('SELECT id, nume, prenume, email, role FROM users');
        return $this->db->resultSet();
    }
}
