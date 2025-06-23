<?php
class Database {
    //Database credentials
    private $host = DB_HOST;
    private $user = DB_USER;
    private $pass = DB_PASS;
    private $dbname = DB_NAME;

    //Database handler
    private $dbh;
    //Statement
    private $stmt;
    // Error
    private $error;

    //Constructor
    public function __construct() {
        //Data Source Name
        $dsn = 'mysql:host=' . $this->host . ';dbname=' . $this->dbname;
        //Options
        $options = array(
            PDO::ATTR_PERSISTENT => true,
            PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION
        );
        //Try to connect to the database
        try {
            $this->dbh = new PDO($dsn, $this->user, $this->pass, $options); // Create a new PDO instance
        } catch(PDOException $e) {
            $this->error = $e->getMessage(); // Set the error message
            echo $this->error; 
        }
    }

    //Prepare the query
    public function query($sql) {
        $this->stmt = $this->dbh->prepare($sql); 
    }

    //Bind the values
    public function bind($param, $value, $type = null) {
        if(is_null($type)) {
            switch(true) {
                case is_int($value):
                    $type = PDO::PARAM_INT;
                    break;
                case is_bool($value):
                    $type = PDO::PARAM_BOOL;
                    break;
                case is_null($value):
                    $type = PDO::PARAM_NULL;
                    break;
                default:
                    $type = PDO::PARAM_STR;
            }
        }
        $this->stmt->bindValue($param, $value, $type);
    }

    //Execute the query
    public function execute() {
        return $this->stmt->execute();
    }

    // Get the result set
    public function resultSet() {
        $this->execute();
        return $this->stmt->fetchAll(PDO::FETCH_OBJ);
    }

    // Get a single record
    public function single() {
        $this->execute();
        return $this->stmt->fetch(PDO::FETCH_OBJ);
    }

    // Get the number of rows
    public function rowCount() {
        return $this->stmt->rowCount();
    }
} 