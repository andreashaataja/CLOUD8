package com.cloud8;

import java.io.IOException;
import java.util.Locale;
import java.util.ResourceBundle;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;


public class PrimaryController {
    @FXML
    private Button primaryButton;
    @FXML
    private Button secondaryButton;


    // Method to switch to Primary.fxml
    @FXML
    public void switchToPrimary(ActionEvent actionEvent) {
        try {
            // Loading the Primary.fxml
            Parent root = FXMLLoader.load(getClass().getResource("Primary.fxml"));
            Scene scene = primaryButton.getScene();
            // Setting the scene on the stage
            scene.setRoot(root);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    // Method to switch to Main.fxml
    @FXML
    public void switchToSecondary(ActionEvent event) {
        try{
            // Loading the Main.fxml
            Locale ENGLISH = new Locale("en_GB");
            ResourceBundle bundle = ResourceBundle.getBundle("lang", ENGLISH);

            FXMLLoader loader = new FXMLLoader(getClass().getResource("Main.fxml"), bundle);
            Parent root = loader.load();
            // Setting the scene on the stage
            Stage stage = (Stage) secondaryButton.getScene().getWindow();
            stage.setScene(new Scene(root));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
