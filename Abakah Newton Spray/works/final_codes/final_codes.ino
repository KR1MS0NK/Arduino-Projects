#include <WiFi.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAsyncWebServer.h>
#include <HX711.h>
#include <ESP32Servo.h>

#define OLED_RESET -1
#define SERVO_PIN 18
#define LED_PIN 19
#define LOADCELL_DOUT_PIN 32
#define LOADCELL_CLK_PIN 33

Adafruit_SSD1306 display(OLED_RESET);
Servo sprayServo;
HX711 loadCell;
HX711 scale;
AsyncWebServer server(80);
WiFiManager wifiManager;

bool deviceState = false;
const float batteryVoltageMin = 2.0;  //Minimum Voltage of the battery (fully discharged)
const float batteryVoltageMax = 4.2;  //Maximum voltage of the battery (fully discharged)
int adcValue = analogRead(35);
unsigned long startMillis = 0;
const int SPRAY_TIME_MIN = 5;
const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <link  rel="stylesheet" href='https://unpkg.com/boxicons@2.1.4/css/boxicons.min.css' >
    <link rel="stylesheet" href="styles.css">
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <script>
        // Existing JavaScript functions
// Function to get data from device
        function getData() {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    var data = JSON.parse(this.responseText);
                    document.getElementById("batteryPercentage").innerHTML = data.batteryPercentage;
                    document.getElementById("sprayCanLevel").innerHTML = data.sprayCanContent;
                }
            };
            xhr.open("GET", "/spray-can-level", true);
            xhr.send();
        }

        // Function to set spraying time
        function setSprayingTime() {
            var sprayingTime = document.querySelector('input[name="sprayingTime"]:checked').value;
            if (sprayingTime == "") {
                sprayingTime = document.getElementById("customTime").value;
            }
            var xhr = new XMLHttpRequest();
            xhr.open("POST", "/setSprayingTime", true);
            xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
            xhr.send("sprayingTime=" + sprayingTime);
        }

        // Function to reset WiFi credentials
        function resetWiFi() {
            var xhr = new XMLHttpRequest();
            xhr.open("POST", "/resetWiFi", true);
            xhr.send();
        }

        // Function to toggle device state
        // Function to toggle device state
        function toggleDeviceState() {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    var data = JSON.parse(this.responseText);
                    if (data.deviceState) {
                        document.querySelector(".status-indicator").classList.remove("status-off");
                        document.querySelector(".status-indicator").classList.add("status-on");
                        document.querySelector(".status-label").innerHTML = "Device is ON";
                    } else {
                        document.querySelector(".status-indicator").classList.remove("status-on");
                        document.querySelector(".status-indicator").classList.add("status-off");
                        document.querySelector(".status-label").innerHTML = "Device is OFF";
                    }
                }
            };
            xhr.open("POST", "/toggleDeviceState", true);
            xhr.send();
        }


        // Get device state every 5 seconds
        // Get device state every 5 seconds
        setInterval(function() {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    var data = JSON.parse(this.responseText);
                    if (data.deviceState) {
                        document.querySelector(".status-indicator").classList.remove("status-off");
                        document.querySelector(".status-indicator").classList.add("status-on");
                        document.querySelector(".status-label").innerHTML = "Device is ON";
                    } else {
                        document.querySelector(".status-indicator").classList.remove("status-on");
                        document.querySelector(".status-indicator").classList.add("status-off");
                        document.querySelector(".status-label").innerHTML = "Device is OFF";
                    }
                }
            };
            xhr.open("GET", "/deviceState", true);
            xhr.send();on
        }, 5000);

// Function to toggle device state
function toggleDeviceState() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            // Update the toggle button state based on the device state
            document.getElementById("toggleSwitch").checked = data.deviceState;
            // Update the device status indicators
            updateDeviceStatus(data.deviceState);
        }
    };
    xhr.open("POST", "/toggleDeviceState", true);
    xhr.send();
}

// Function to update device status indicators based on device state
function updateDeviceStatus(deviceState) {
    var connectionStatusIndicator = document.getElementById("connectionStatus");
    var deviceStatusLabel = document.getElementById("deviceStatusLabel");

    if (deviceState) {
        connectionStatusIndicator.classList.remove("status-off");
        connectionStatusIndicator.classList.add("status-on");
        deviceStatusLabel.innerHTML = "Device is ON";
    } else {
        connectionStatusIndicator.classList.remove("status-on");
        connectionStatusIndicator.classList.add("status-off");
        deviceStatusLabel.innerHTML = "Device is OFF";
    }
}

// Get device state when the page loads
window.onload = function() {
    toggleDeviceState();
};

// Get device state every 5 seconds
setInterval(toggleDeviceState, 5000);

        // Function to toggle device state
function toggleDeviceState() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            // Update the toggle button state based on the device state
            document.getElementById("toggleSwitch").checked = data.deviceState;
        }
    };
    xhr.open("POST", "/toggleDeviceState", true);
    xhr.send();
}

// Get device state when the page loads
window.onload = function() {
    toggleDeviceState();
};

// Get device state every 5 seconds
setInterval(toggleDeviceState, 5000);

    </script>
</head>
<body>
  <style>
    body {
  font-family: Arial, sans-serif;
  background-color: #f5f5f5;
}

.header-text{
    font-family: fangsong;
    font-size: 30px;
}

.navbar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  background-color: #333;
  color: #fff;
  padding: 10px;
}

.navbar ul {
  list-style: none;
  display: flex;
}

.navbar li {
  margin-right: 20px;
}

.bx{
  padding: 15px 15px 15px 13px;
  font-size: 30px;
}

.bx-reset{
  color: red;
}

.bx:hover{
  color:  hsl(120, 4%, 48%);
}

#status-container{
  padding: 15px 15px 15px 13px;
}

.status-indicator {
    width: 20px;
    height: 20px;
    border-radius: 50%;
    margin-right: 5px;
}

.status-on {
    background-color: green;
}

.status-off {
    background-color: red;
}

.card-container {
  display: flex;
  justify-content: space-around;
  margin: 20px 0;
}

.card {
  background-color: #fff;
  padding: 20px;
  border-radius: 20px;
  box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.2);
  width: 30%;
}
.cardInterval {
 max-width: 350px;
  margin: 50px auto;
  padding: 20px;
  background-color: #fff;
  border-radius: 20px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
}

.card h2 {
  text-align: center;
  margin-bottom: 20px;
}

.buttons-container {
display: flex;
margin-bottom: 20px;
gap: 0.5rem !important;
flex-direction: row;
align-items: stretch !important;
padding-bottom: 1rem !important;
}

.buttons-container button {
padding: 10px 30px 10px;
background-color: #007bff;
color: #fff;
border: none;
cursor: pointer;
flex: 1;
border-radius: 5px 5px 5px 5px;
}

.input-container {
  display: flex;
  align-items: center;
}

.input-container input {
  padding: 10px;
  flex: 1;
  border: 1px solid #ccc;
  border-radius: 5px 0 0 5px;
}

.input-container button {
  padding: 10px 20px;
  background-color: #28a745;
  color: #fff;
  border: none;
  border-radius: 0 5px 5px 0;
  cursor: pointer;
}

.watermark {
  position: fixed;
  bottom: 10px;
  right: 10px;
  font-size: 12px;
  color: rgba(0, 0, 0, 0.2);
}

  </style>
  
  <div class="navbar">
    <span class="header-text">AIR REFRESHER</span>
    <ul>
      <li><i class='bx bx-reset' onclick="resetWiFi()" title="reset credentials " ></i>
</li>
      <li>               
         <div class="status-indicator status-off" id="connectionStatus"></div>
      </li>
      <li>  
        <div class="form-check form-switch text-center">
            <input class="form-check-input" type="checkbox" id="toggleSwitch" onchange="toggleDeviceState()">
            <label class="form-check-label" for="toggleSwitch">Device ON/OFF</label>
        </div></li>
    </ul>
  </div>

  <div class="card-container">
    <div class="card">
      <h2>Spray Level</h2>
      <p><span id="sprayCanLevel">0</span></p>
    </div>
    <div class="card">
      <h2>Battery Level</h2>
      <p><span id="batteryPercentage">0</span>%</p>
    </div>
  </div>

  <div class="cardInterval">
    <h2>Spray Interval</h2>
    <div class="buttons-container">
      <button type="button" name="sprayingTime">10 min</button>
      <button type="button" name="sprayingTime">30 min</button>
      <button type="button" name="sprayingTime">60 min</button>
    </div>
    <div class="input-container">
      <input id="customTime" placeholder="Enter spray time">
      <button id="save-button" type="submit" onclick="setSprayingTime()">Save</button>
    </div>
  </div>
  </div>

  <div class="watermark">AIR REFFRESHER</div>
</body>
</html>
)=====";

    
void setup() {
  pinMode(LED_PIN, OUTPUT); // Set the LED_PIN as an output
  digitalWrite(LED_PIN, LOW); // Turn off the LED initially
  sprayServo.attach(SERVO_PIN);
  
  wifiManager.autoConnect("AIR REFRESHNER");
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  // Display IP address on OLED for one minute
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("IP Address: " + WiFi.localIP().toString());
  display.display();
  delay(60000);

  // Initialize HX711
  loadCell.begin(LOADCELL_DOUT_PIN, LOADCELL_CLK_PIN);
  scale.set_scale(-620);
  scale.tare();

  // Handle HTTP GET request to retrieve battery percentage and spray can level
  server.on("/spray-can-level", HTTP_GET, [](AsyncWebServerRequest *request){
    int sprayCanContent = loadCell.read();
    int adcValue = analogRead(35);
    //Map the ADC value to the battery voltage range
    float voltage = map(adcValue, 0, 4095, batteryVoltageMin * 1000, batteryVoltageMax * 1000) / 1000.0;

    //Calculate the battery percentage
    int batteryPercentage = map(voltage, batteryVoltageMin, batteryVoltageMax, 0, 100);
    batteryPercentage = constrain(batteryPercentage, 0, 100);  //Make sure the battery percentage is within 0-100 range

    String json = "{\"batteryPercentage\": " + String(batteryPercentage) + ", \"sprayCanContent\": " + String(sprayCanContent) + "}";
    request->send(200, "application/json", json);
  });

  // Handle HTTP POST request to toggle device state
server.on("/toggleDeviceState", HTTP_POST, [](AsyncWebServerRequest *request){
    deviceState = !deviceState; // Toggle the device state
    request->send(200);
});

  // Handle HTTP POST request to set spraying time
  server.on("/setSprayingTime", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("sprayingTime", true)) {
      int sprayingTime = request->getParam("sprayingTime", true)->value().toInt();
      spraytime
      // Set spraying time
      startMillis = millis();
      deviceState = true;
    }
    request->send(200);
  });

  // Handle HTTP POST request to reset WiFi credentials
  server.on("/resetWiFi", HTTP_POST, [](AsyncWebServerRequest *request){
    wifiManager.resetSettings();
    wifiManager.autoConnect("AIR REFRESHNER");
    request->send(200);
  });

  // Serve the HTML webpage
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

void loop() {

  Serial.println(startMillis);
    // if(deviceState && (millis() - startMillis >= SPRAY_TIME_MIN * 60000)) {
    //     sprayServo.write(0);
    //     delay(1000);
    //     sprayServo.write(180);
    //     delay(1000);

    //     digitalWrite(LED_PIN, HIGH);
    //     delay(1000);
    //     digitalWrite(LED_PIN, LOW);

    //     deviceState = false;
    // }

        // Check if the device is ON, if yes, turn on the LED and execute the spray process
    if (deviceState) {
        digitalWrite(LED_PIN, HIGH); // Turn on the LED
        sprayServo.write(0);
        delay(1000);
        sprayServo.write(180);
        delay(1000);

        digitalWrite(LED_PIN, LOW); // Turn off the LED
        deviceState = false; // Turn off the device
    }
    
        //Value for the Spray time
        int sprayingTime = request->getParam("sprayingTime", true)->value().toInt();
        //Value read of the spray can level
        int sprayCanContent = loadCell.read();
        //Value of the adcValue read
        int adcValue = analogRead(35);
    //Map the ADC value to the battery voltage range
    float voltage = map(adcValue, 0, 4095, batteryVoltageMin * 1000, batteryVoltageMax * 1000) / 1000.0;

    //Calculate the battery percentage
    int batteryPercentage = map(voltage, batteryVoltageMin, batteryVoltageMax, 0, 100);
    batteryPercentage = constrain(batteryPercentage, 0, 100);  //Make sure the battery percentage is within 0-100 range

  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.print(" V | Battery Percentage: ");
  Serial.print(batteryPercentage);
  Serial.print("%");
  Serial.print("SprayLevel: ");
  Serial.print(sprayCanContent);

  display.print("BATTERY VOLTAGE: ");
  display.print(batteryPercentage);
  display.println("%");

  display.print("SPRAY: ");
  display.print(sprayCanContent);
  display.println("%");
  display.println();

  display.print("TIMESET(m): ");
  display.println(sprayingTime);
  

  delay(5000);  // Wait for 5 seconds before taking the next reading
}
