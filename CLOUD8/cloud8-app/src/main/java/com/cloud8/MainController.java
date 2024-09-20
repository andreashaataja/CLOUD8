package com.cloud8;


import javafx.event.ActionEvent;
import javafx.event.EventTarget;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;
import java.io.IOException;
import java.util.Locale;
import java.util.ResourceBundle;
import java.util.prefs.Preferences;
import javafx.scene.control.MenuItem;
import javafx.scene.control.MenuButton;
import org.eclipse.paho.client.mqttv3.MqttException;



public class MainController {

    public Button primaryButton;
    public Button humanButton;
    public Button HearButton;
    public Button HearDButton;
    public Button ColorButton;
    public Button ColorBLindButton;
    public Button NormalButton;
    public Button saveButton;
    @FXML
    public CheckBox alarmCheckBox;
    @FXML
    public MenuItem protanopia;
    @FXML
    public MenuItem defaultItem;
    @FXML
    public MenuItem deuteranopia;
    @FXML
    public MenuItem tritanopia;
    @FXML
    public MenuItem monochromacy;
    @FXML
    public MenuButton disabilityMenuButton;
    @FXML
    public MenuButton modeMenuButton;
    @FXML
    public MenuButton languageMenuButton;

    // Initialize text field
    @FXML
    public TextField temperatureLimitLowField;
    @FXML
    public TextField temperatureLimitHighField;
    @FXML
    public TextField humidityLimitLowField;
    @FXML
    public TextField humidityLimitHighField;
    @FXML
    public TextField buzzerIntervalField;
    @FXML
    public TextField buzzerDurationField;

    // Initialize and declare key variables
    private static final String TEMPERATURE_LIMIT_LOW_KEY = "temperatureLimitLow";
    private static final String TEMPERATURE_LIMIT_HIGH_KEY = "temperatureLimitHigh";
    private static final String HUMIDITY_LIMIT_LOW_KEY = "humidityLimitLow";
    private static final String HUMIDITY_LIMIT_HIGH_KEY = "humidityLimitHigh";
    private static final String BUZZER_INTERVAL_KEY = "buzzerInterval";
    private static final String BUZZER_DURATION_KEY = "buzzerDuration";
    @FXML
    public Label temperature = new Label();
    public Label humidity = new Label();
    @FXML
    public AnchorPane temperatureNumber = new AnchorPane();
    @FXML
    public AnchorPane humidityNumber = new AnchorPane();
    private double FontSize = 20.0;

    private static final String BLINDNESS_MODE_KEY = "BlindMode";
    private final MqttHandler MQTT_CLIENT;
    private final Preferences saving = Preferences.userNodeForPackage(PrimaryController.class);
    // Preferences will store data in java. Making a preferences with object name "saving"
    // to handling user preferences and app's settings
    // userNodeForPackage(PrimaryController.class) will return
    // a node in the preferences hierarchy associated with primary class
    ResourceBundle bundle;
    static Locale current = new Locale("en_GB");
    static int option;
    private final LanguageOptions langOptions = new LanguageOptions();


    // Initialization
    public MainController() throws MqttException {
        MQTT_CLIENT = new MqttHandler(this);
    }
    @FXML
    void initialize() throws MqttException {
        // Initialize the mode menu text after the controller is initialized
        String savedMode = saving.get(BLINDNESS_MODE_KEY , "Default");
        switchToMode(savedMode);
        this.modeMenuButton = new MenuButton();
        this.languageMenuButton = new MenuButton();
    }

    // Method for handling disability menu items
    @FXML
    public void handleDisabilitySelection(ActionEvent event) {
        MenuItem selectedMenuItem = ((MenuItem) event.getTarget());
        String selectedDisability = selectedMenuItem.getText();

        if (selectedDisability != null) {
            switch (selectedDisability) {

                case "None":
                    // If Color Blindness is selected, user can choose "Hearing Impairment" and "None"
                    if (disabilityMenuButton.getItems().get(0).getText().equals("Color Blindness")) {
                        updateMenuButtonItems("Hearing Impairment", "None");
                    }
                    // If Hearing Impairment is selected, user can choose "Color Blindness" and "None"
                    else if (disabilityMenuButton.getItems().get(0).getText().equals("Hearing Impairment")) {
                        updateMenuButtonItems("Color Blindness", "None");
                    }
                    break;

                case "Color Blindness":
                    updateMenuButtonItems("Hearing Impairment", "None");
                    break;

                case "Hearing Impairment":
                    updateMenuButtonItems("Color Blindness", "None");
                    break;
            }
        }
    }

    // Method for updating menu items while choosing one of the items
    public void updateMenuButtonItems(String... items) {
        disabilityMenuButton.getItems().clear();
        for (String item : items) {
            MenuItem menuItem = new MenuItem(item);
            menuItem.setOnAction(event -> handleDisabilitySelection(event)); // Pass the event to the handler
            disabilityMenuButton.getItems().add(menuItem);
        }
    }

    @FXML
    public void handleLanguageMenu(ActionEvent event) {
        MenuItem selectedMenuItem = ((MenuItem) event.getTarget());
        String selectedLanguage = selectedMenuItem.getText();

        if (selectedLanguage != null) {
            switch (selectedLanguage) {

                case "English":
                    if (languageMenuButton.getItems().get(0).getText().equals("Farsi")) {
                        updateMenuButton("Swedish", "English" , "Spanish","Urdu");
                    }
                    else if (languageMenuButton.getItems().get(0).getText().equals("Swedish")) {
                        updateMenuButton("Farsi", "English" , "Spanish","Urdu");
                    }
                    else if(languageMenuButton.getItems().get(0).getText().equals("Spanish")){
                        updateMenuButton("Farsi" , "English" , "Swedish","Urdu");
                    }
                    else if(languageMenuButton.getItems().get(0).getText().equals("Urdu")){
                        updateMenuButton("Farsi" , "English" , "Swedish","Spanish");
                    }
                    break;

                case "Farsi":
                    updateMenuButton("Swedish", "English" , "Spanish","Urdu");
                    break;

                case "Swedish":
                    updateMenuButton("Farsi", "English" , "Spanish","Urdu");
                    break;

                case "Spanish" :
                    updateMenuButton("Farsi" , "English" , "Swedish","Urdu");
                    break;
                case "Urdu" :
                    updateMenuButton("Farsi" , "English" , "Swedish","Spanish");
                    break;
            }
        }
    }

    public void updateMenuButton(String... items) {
        languageMenuButton.getItems().clear();
        for (String item : items) {
            MenuItem menuItem = new MenuItem(item);
            menuItem.setOnAction(event -> handleLanguageMenu(event)); // Pass the event to the handler
            languageMenuButton.getItems().add(menuItem);
        }
    }

    // Method to switch to primary.fxml
    @FXML
    public void switchToPrimary() {
        try {
            // Loading the primary.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("Primary.fxml"), ResourceBundle.getBundle("lang", langOptions.getLangOpts()[option]));
            Parent root = loader.load();
            // Setting the scene on the stage
            Stage stage = (Stage) primaryButton.getScene().getWindow();
            stage.setScene(new Scene(root));

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Method to switch to secondary.fxml
    @FXML
    public void switchToSecondary(ActionEvent event) throws IOException {
        String langSelection = "";
        EventTarget userAction = event.getTarget();       // gets what the user clicked to call this method

        if(userAction instanceof MenuItem) {              // checks if userAction is a MenuItem
            MenuItem item = (MenuItem) userAction;        // if it is it casts userAction to a MenuItem
            langSelection = item.getText();               // gets the text of the MenuItem
        }

        bundle = ResourceBundle.getBundle("lang", langOptions.getLangOpts()[option]);

        if(langSelection.equals(bundle.getString("langSwedish"))) {
            option = 1;
        } else if (langSelection.equals(bundle.getString("langEnglish"))) {
            option = 0;
        } else if (langSelection.equals(bundle.getString("langSpanish"))) {
            option = 2;
        } else if (langSelection.equals(bundle.getString("langUrdu"))) {
            option = 3;
        } else if (langSelection.equals(bundle.getString("langFarsi"))) {
            option = 4;
        }
        try{
            if(!MQTT_CLIENT.isClientConnected()) { // checking if client is already connected
                MQTT_CLIENT.connectToBroker();     // if client is not connected then connectToBroker() is called
            }
            current = langOptions.getLangOpts()[option];
            bundle = ResourceBundle.getBundle("lang", current);

            // Loading the secondary.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("Main.fxml"), bundle);
            Parent root = loader.load();
            // Setting the scene on the stage
            Stage stage = (Stage) humanButton.getScene().getWindow();
            stage.setScene(new Scene(root));

        } catch (IOException e) {
            e.printStackTrace();
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    // Method to switch to Normal.fxml
    public void switchToTemp() {
        try{
            // Loading the Normal.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("Temp&Humidity.fxml"), ResourceBundle.getBundle("lang", current));
            Parent root = loader.load();
            // Setting the scene on the stage
            Stage stage = (Stage) humanButton.getScene().getWindow();
            stage.setScene(new Scene(root));

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Method to switch to ThresholdNormal.fxml
    public void switchToNormalThreshold() {
        try {
            // Loading the ThresholdNormal.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("ThresholdNormal.fxml"), ResourceBundle.getBundle("lang", current));
            Parent root = loader.load();
            // Setting the scene on the stage
            Stage stage = (Stage) NormalButton.getScene().getWindow();
            stage.setScene(new Scene(root));

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Method to switch to ThresholdHearingDef.fxml
    public void switchToHearDefThreshold() {
        try {
            // Loading the HearingDef.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("ThresholdHearingDef.fxml"), ResourceBundle.getBundle("lang", current));
            Parent root = loader.load();
            // Setting the scene on the stage
            Stage stage = (Stage) HearDButton.getScene().getWindow();
            stage.setScene(new Scene(root));

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Method to switch to ThresholdColorBlind.fxml
    public void switchToColorBlindThreshold() {
        try {
            // Loading the ThresholdColorBlind.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("ThresholdColorBlind.fxml"), ResourceBundle.getBundle("lang", current));
            Parent root = loader.load();
            // Setting the scene on the stage
            Stage stage = (Stage) ColorBLindButton.getScene().getWindow();
            stage.setScene(new Scene(root));

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Method to switch to Hearing-DefMode.fxml by clicking confirm button
    public void switchToHearD() {
        try{
            // Loading the Hearing-DefMode.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("Hearing-DefMode.fxml"), ResourceBundle.getBundle("lang", current));
            Parent root = loader.load();
            // Setting the scene on the stage
            Stage stage = (Stage) HearButton.getScene().getWindow();
            stage.setScene(new Scene(root));

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Method to switch to Color-blindMode.fxml by clicking confirm button
    public void switchToColorB() {
        try{
            // Loading the Color-blindMode.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("Color-blindMode.fxml"), ResourceBundle.getBundle("lang", current));
            Parent root = loader.load();
            // Setting the scene on the stage
            Stage stage = (Stage) ColorButton.getScene().getWindow();
            stage.setScene(new Scene(root));

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Method to change to color blindness disability by choosing in menu
    @FXML
    public void handleColorBlindness() {
        try {
            String colorMode = "colorBlindMode";
            MQTT_CLIENT.publishMessages(colorMode);
            // Loading the Color-blindMode.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("Color-blindMode.fxml"), ResourceBundle.getBundle("lang", current));
            Parent root = loader.load();

            // Getting the stage and set the new scene
            Stage stage = (Stage) disabilityMenuButton.getScene().getWindow();
            stage.setScene(new Scene(root));
        } catch (IOException e) {
            e.printStackTrace();
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }

    // Method to change to hearing disability by choosing in menu
    @FXML
    public void handleHearingImpairment() throws IOException {

            // Loading the Hearing-DefMode.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("Hearing-DefMode.fxml"), ResourceBundle.getBundle("lang", current));
            Parent root = loader.load();

            // Getting the stage and set the new scene
            Stage stage = (Stage) disabilityMenuButton.getScene().getWindow();
            stage.setScene(new Scene(root));

    }

    // Method to change to none disability by choosing in menu
    @FXML
    public void handleNone() {
        try{
            String messageNone = "defaultMode";
            MQTT_CLIENT.publishMessages(messageNone);
            // Loading the secondary.fxml
            FXMLLoader loader = new FXMLLoader(getClass().getResource("Main.fxml"), ResourceBundle.getBundle("lang", current));
            Parent root = loader.load();

            // Getting the stage and setting the new scene
            Stage stage = (Stage) disabilityMenuButton.getScene().getWindow();
            stage.setScene(new Scene(root));
        } catch (IOException e) {
            e.printStackTrace();
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }

    // Method to save and update temperature and humidity limits
    @FXML
    public void saveLimits() throws MqttException {

        String temperatureLimitLow = "tempThresholdLow" + temperatureLimitLowField.getText();
        String temperatureLimitHigh = "tempThresholdHigh" + temperatureLimitHighField.getText();
        String humidityLimitLow = "humThresholdLow" + humidityLimitLowField.getText();
        String humidityLimitHigh = "humThresholdHigh" + humidityLimitHighField.getText();
        String buzzerInterval = "buzzerInterval" + buzzerIntervalField.getText();
        String buzzerDuration = "buzzerDuration" + buzzerDurationField.getText();

        // Updating the limits
        saving.put(TEMPERATURE_LIMIT_LOW_KEY, temperatureLimitLow);
        saving.put(TEMPERATURE_LIMIT_HIGH_KEY, temperatureLimitHigh);
        saving.put(HUMIDITY_LIMIT_LOW_KEY, humidityLimitLow);
        saving.put(HUMIDITY_LIMIT_HIGH_KEY, humidityLimitHigh);
        saving.put(BUZZER_INTERVAL_KEY, buzzerInterval);
        saving.put(BUZZER_DURATION_KEY, buzzerDuration);


        // Updated limits have been saved
        saving.putBoolean("limitsSaved", true);

        // Print out the updated limits for testing
        System.out.println("Lower temperature limit updated: " + temperatureLimitLow);
        System.out.println("Higher temperature limit updated " + temperatureLimitHigh);
        System.out.println("Lower humidity limit updated: " + humidityLimitLow);
        System.out.println("Higher humidity limit updated: " + humidityLimitHigh);
        System.out.println("Buzzer interval updated: " + buzzerInterval);
        System.out.println("Buzzer duration updated: " + buzzerDuration);

        // Method call to send threshold limits and buzzer settings
        MQTT_CLIENT.publishThresholds(temperatureLimitLow, temperatureLimitHigh, humidityLimitLow,
                humidityLimitHigh, buzzerInterval, buzzerDuration);
    }

    // Method to save the mode the user choose
    public void savedMode(String mode) {
        saving.put(BLINDNESS_MODE_KEY, mode);
    }
    @FXML
    private void handleMenuItemAction(ActionEvent event) throws MqttException {
        MenuItem menuItem = (MenuItem) event.getSource();
        String selectedOption = menuItem.getText();
        modeMenuButton.setText(selectedOption);
        switchToMode(selectedOption);
    }

    // Method to switch to the selected mode
    private void switchToMode(String mode) throws MqttException {
        switch (mode) {
            case "Default":
                handleDefault();
                break;
            case "Protanopia":
                handleProtanopia();
                break;
            case "Deuteranopia":
                handleDeuteranopia();
                break;
            case "Tritanopia":
                handleTritanopia();
                break;
            case "Monochromacy":
                handleMonochromacy();
                break;
            default:
                break;
        }
    }

    // Methods to select the color of alert for different colorblindness
    @FXML
    public void handleDefault () throws MqttException{
        String message = "ColorSchemeDefault";
        MQTT_CLIENT.publishMessages(message);
        savedMode("Default");
        System.out.println("Default mode selected");
    }
    @FXML
    public void handleProtanopia() throws MqttException {
        String message = "ColorSchemeProtanopia";
        MQTT_CLIENT.publishMessages(message);
        savedMode("Protanopia");
        System.out.println("Protanopia mode selected");
    }

    @FXML
    public void handleDeuteranopia() throws MqttException {
        String message = "ColorSchemeDeuteranopia";
        MQTT_CLIENT.publishMessages(message);
        savedMode("Deuteranopia");
        System.out.println("Deuteranopia mode selected"); // Corrected message
    }

    @FXML
    public void handleTritanopia() throws MqttException {
        String message = "ColorSchemeTritanopia";
        MQTT_CLIENT.publishMessages(message);
        savedMode("Tritanopia");
        System.out.println("Tritanopia mode selected");
    }

    @FXML
    public void handleMonochromacy() throws MqttException {
        String message = "ColorSchemeMonochromacy";
        MQTT_CLIENT.publishMessages(message);
        savedMode("Monochromacy");
        System.out.println("Monochromacy mode selected");
    }

    // Method for showing the selected color on wio terminal
    public void handleSelectedColor(String Hex) {
        // Updating the background of Wio terminal to selected color
        temperature.setStyle("-fx-background-color: " + Hex + ";");
        humidity.setStyle("-fx-background-color: " + Hex + ";");
    }

    public void updateTemperature(String temperatureValue) {
        temperature.setText(temperatureValue + " °C");
    }

    public void updateHumidity(String humidityValue){

        humidity.setText(humidityValue + " %");
    }

    // Method to increase text size
    @FXML
    public void increaseTextSize() throws MqttException {
        FontSize = FontSize + 2;
        if (FontSize < 6) {
            FontSize = 6; // Maximum font size
        }
        temperatureNumber.setStyle("-fx-font-size: " + FontSize + "px;");
        humidityNumber.setStyle("-fx-font-size: " + FontSize + "px;");
        MQTT_CLIENT.publishTextSize("fontSizeIncrease"); // Publish the new font size to MQTT
    }

    // Method to decrease text size
    @FXML
    public void decreaseTextSize() throws MqttException {
        FontSize = FontSize - 2;
        if (FontSize < 8) {
            FontSize = 8; // Minimum font size
        }
        temperatureNumber.setStyle("-fx-font-size: " + FontSize + "px;");
        humidityNumber.setStyle("-fx-font-size: " + FontSize + "px;");
        MQTT_CLIENT.publishTextSize("fontSizeDecrease"); // Publish the new font size to MQTT
    }

    // Method to Deactivate alarm in HearingDef Threshold
    @FXML
    public void handleDeactivateAlarmCheckBox() throws MqttException{
        if (alarmCheckBox.isSelected()) {
            String hearingImpMode = "hearingImpMode";
            MQTT_CLIENT.publishMessages(hearingImpMode);
            System.out.println("Alarm deactivated");
        }else if (!alarmCheckBox.isSelected()){
            String messageNone = "defaultMode";
            MQTT_CLIENT.publishMessages(messageNone);
            System.out.println("Alarm activated");
        }
    }
}
