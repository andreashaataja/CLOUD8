# CLOUD8

![](https://git.chalmers.se/courses/dit113/2024/group-5/mini-project-system-development/-/wikis/uploads/1769c863014cf308fab969a431d04695/output-onlinegiftools-min.gif)

## Purpose and benefits

The CLOUD8 system is designed as a simple, yet functional local weather station. It allows the user to monitor temperature and humidity levels in any chosen environment, such as homes, offices, schools and cars, and receive alerts once a threshold has been met. This allows the user to quickly make adjustments to the environment to ensure optimal conditions for their particular situation. 

> The key purpose of this project was to ensure accessibility for people with hearing deficiencies as well as various types of color blindness.


## Benefits
The system provides accurate real-time measurements of temperature and humidity, helping users to always be aware of the environmental conditions and offering customizable alerts when a temperature or humidity level goes beyond the set range. 

The alerts can be customized in terms of duration and interval allowing for timely responses to potential issues. In default mode, the screen of the Wio Terminal changes color from blue to red once a threshold has been reached. However,  in color-blind mode the user can choose from different color scheme presets in order to cater to their specific needs.

On the other hand, in hearing deficiency mode the user can choose to switch off the sound alert and instead be alerted by a LED light. Another accessibility feature of the system is the language package, offering users a selection from a variety of languages and enhancing usability for individuals from different linguistic backgrounds. The system is designed in such a way that in newer iterations of the product, more languages and features can easily be added.

## Hardware & Software Architecture
![](https://cdn.discordapp.com/attachments/1155226658316505119/1241782586603208846/cloud8_diagram.png?ex=664b73a7&is=664a2227&hm=787d3b201c29691db53621e77b62e38ae5f37fdad07d5cee9f23a718b276c0ea&)

### Prerequisites
Hardware/Software that you need in order for everything to work.
1.  [Seeed Wio Terminal.](https://www.seeedstudio.com/Wio-Terminal-p-4509.html)
2. Install [Arduino IDE.](https://www.arduino.cc/en/software)
3. Install any IDE that can run a JavaFX application ([IntelliJ](https://www.jetbrains.com/idea/) for example)
4. The Seeed Wio Terminal Board Library [(Guide here)](https://wiki.seeedstudio.com/Wio-Terminal-Getting-Started/#getting-started)
5. Sensors needed [(Sensor List)](https://git.chalmers.se/courses/dit113/2024/group-5/mini-project-system-development/-/wikis/Sensor-List)
6. Install the needed Arduino libraries (In the Arduino IDE, go to Tools → Manage Libraries and search and download the following libraries.
    - WiFiClientSecure
    - PubSubClient
    - TFT_eSPI
    - DHT

7. A [‘Secrets.h’](https://www.dropbox.com/scl/fi/p6pc6k1mz05rkx7dh29o4/Secrets.h?rlkey=84obvg4mqgvp3m84ugblnl08o&e=1&st=0tmrl0q3&dl=0) file (for storing client ID, WiFi-name and password).
8.  Git (to clone the project).

## User Manual 
### Installation (Seeed Wio Terminal and Application)

Assuming that you have all the prerequisites, you can start with the installation process.

#### Seeed Wio Terminal and project setup

1. Connect the DHT11 and Blue LED sensor (see [images](https://imgur.com/a/5fMYBJt)) to your Seeed Wio Terminal
2. Ensure that the Seeed Wio Terminal is connected to your machine.
3. Turn on the Seeed Wio Terminal
4. Clone the project into your preferred local folder
    - git clone git@git.chalmers.se:courses/dit113/2024/group-5/mini-project-system-development.git

5. Navigate to where you cloned the project to on your machine.

6. Navigate to the Arduino/cloud8/ and add the Secrets.h to this folder.
7. Edit the Secrets.h file with a text-editor of your choice and update the ID (to whatever you want) as well as the WiFi name and password to match your network.
8. Upload the code to the terminal when it is connected to your machine.

#### Run the application

1. Open your IDE that handles Java and JavaFX
2. Open the folder that holds the project (where you saved the clone)
3. In your IDE, navigate to cloud8-app/src/main/java/com/cloud8.
4. Run the java file called App.java

After all these steps are completed, you will be able to access the app and Seeed Wio Terminal with all the features built in.


## Contributors
### The project members
##### Andreas Haataja
- Focused on back-end development.
- Special focus on MQTT, Wio Terminal, and internationalization of the app.
- Some back-end utilities for the JavaFX application.

##### Anna Skeppstedt
- Focused on back-end development.
- Special focus on the Wio Terminal, sensor integration, and internationalization of the app.
- Some back-end utilities for the JavaFX application.

##### Claudia Sevilla Eslava
- Focused on the UI development of the JavaFX application.
- Made the CI Pipeline for Maven and Arduino.
- A few back-end utilities.

##### Junaid Nasir
- Focused on back-end development.
- Special focus on the Wio Terminal.
- Some focus on language and disability support features.

##### Nika Balouchzadeh
- Took the role as the leader of the project.
- Focused on the UI development of the JavaFX application.
- Back-end utilities related to the JavaFX application.

## Demo
We have created a demo for our system where we demonstrate all the functionalities and features. To view the demo, please [click this link.](https://youtu.be/OrZcVwI1H6M?si=RFCHFXPaiMvskQxJ)