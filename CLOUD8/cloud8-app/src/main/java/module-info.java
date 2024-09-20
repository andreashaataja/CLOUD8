module com.cloud8 {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.sql;
    requires java.prefs;
    requires org.eclipse.paho.client.mqttv3;

    opens com.cloud8 to javafx.fxml;
    exports com.cloud8;
}
