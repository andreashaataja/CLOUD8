package com.cloud8;

import javafx.application.Platform;
import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MqttHandler implements MqttCallback {
    private final static ExecutorService THREAD_POOL = Executors.newSingleThreadExecutor();

    private final IMqttClient cloudClient;
    private final  String[] SUBSCRIBE_TOPIC = {"CLOUD8/temperature", "CLOUD8/humidity"};
    private final String PUBLISH_TOPIC = "CLOUD8";
    private final String USERNAME = "MPSDCLOUD8";
    private final String PASSWORD = "Cloud123";
    private final String BROKER = "ssl://34ce87e6f6cd40d68bdf77f056a9f57e.s1.eu.hivemq.cloud:8883";
    private final String USER_ID = "CLOUD8";
    private final MainController mainController;
    public static String temperatureValue;
    public static String humidityValue;

    public MqttHandler(MainController controller) throws MqttException {
        cloudClient = new MqttClient(BROKER, USER_ID, new MemoryPersistence());
        this.mainController = controller;
    }

    public void connectToBroker() throws MqttException {
        if (!cloudClient.isConnected()) {
            try {
                System.out.println("Connecting...");
                cloudClient.connect(setConnectOptions());
                cloudClient.setCallback(this);
                subToSubscribeTopic();
                System.out.println("Connected to CLOUD8 MQTT Broker successfully.");

            } catch (MqttException e) {
                e.printStackTrace();
            }
        }
    }

    public MqttConnectOptions setConnectOptions() {
        MqttConnectOptions connectOptions = new MqttConnectOptions();

        connectOptions.setCleanSession(true); // https://stackoverflow.com/questions/51183171/mqtt-client-connectionlost-do-not-work-after-reconnect-to-broker
        connectOptions.setUserName(USERNAME);
        connectOptions.setPassword(PASSWORD.toCharArray());
        connectOptions.setKeepAliveInterval(5);
        connectOptions.setAutomaticReconnect(true);

        return connectOptions;
    }

    public void subToSubscribeTopic() {
        THREAD_POOL.submit(() -> {
            try {
                if(cloudClient.isConnected()) {
                    cloudClient.subscribe(SUBSCRIBE_TOPIC);
                } else {
                    connectToBroker();
                    cloudClient.subscribe(SUBSCRIBE_TOPIC);
                }
            } catch (MqttSecurityException e) {
                e.printStackTrace();
            } catch (MqttException e) {
                e.printStackTrace();
            }
        });
    }

    @Override
    public void connectionLost(Throwable cause) {
        try {
            cloudClient.subscribe(SUBSCRIBE_TOPIC);
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void messageArrived(String topic, MqttMessage message) {
        String payloadMessage = message.toString();
        if (topic.equals("CLOUD8/temperature")) {
            temperatureValue = payloadMessage;
            Platform.runLater(() -> mainController.updateTemperature(temperatureValue));
        } else if (topic.equals("CLOUD8/humidity")) {
            humidityValue = payloadMessage;
            Platform.runLater(() -> mainController.updateHumidity(humidityValue));
        }
    }

    public void publishMessages(String message) throws MqttException {
        MqttMessage payload = new MqttMessage(message.getBytes());
        if(cloudClient.isConnected()) {
            cloudClient.publish(PUBLISH_TOPIC, payload);
        } else {
            connectToBroker();
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {

    }

    public boolean isClientConnected() {
        return cloudClient.isConnected();
    }

    public void publishThresholds(String tempThresholdLow, String tempThresholdHigh, String humThresholdLow,
                                  String humThresholdHigh, String buzzerInterval, String buzzerDuration)
            throws MqttException {

        if (cloudClient.isConnected()) {
            MqttMessage messageTempLow = new MqttMessage(tempThresholdLow.getBytes());
            MqttMessage messageTempHigh = new MqttMessage(tempThresholdHigh.getBytes());
            MqttMessage messageHumLow = new MqttMessage(humThresholdLow.getBytes());
            MqttMessage messageHumHigh = new MqttMessage(humThresholdHigh.getBytes());
            MqttMessage messageBuzzInt = new MqttMessage(buzzerInterval.getBytes());
            MqttMessage messageBuzzDur = new MqttMessage(buzzerDuration.getBytes());
            cloudClient.publish(PUBLISH_TOPIC, messageTempLow);
            cloudClient.publish(PUBLISH_TOPIC, messageTempHigh);
            cloudClient.publish(PUBLISH_TOPIC, messageHumLow);
            cloudClient.publish(PUBLISH_TOPIC, messageHumHigh);
            cloudClient.publish(PUBLISH_TOPIC, messageBuzzInt);
            cloudClient.publish(PUBLISH_TOPIC, messageBuzzDur);
        } else {
            connectToBroker();
        }
    }
    public void publishTextSize(String newSize) throws MqttException {
        if (cloudClient.isConnected()) {
            MqttMessage messageTextSize = new MqttMessage(String.valueOf(newSize).getBytes());
            cloudClient.publish(PUBLISH_TOPIC, messageTextSize);
        } else {
            connectToBroker();
        }
    }
}
