module com.example.lab6 {
    requires javafx.swing;
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;



    opens com.example.lab6 to javafx.fxml;
    exports com.example.lab6;
}