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
#define R1 27000 //27K ohms
#define R2 100000 //100K ohms
#define BATTERY_PIN 35
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
int rawValue = analogRead(BATTERY_PIN);
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
  margin: 0;
  padding: 0;
  font-family: Arial, sans-serif;
  background-color: #f1f1f1;
}

.navbar {
  display: flex;
  justify-content: space-between;
  background-color: black;
  color: white;
  padding: 15px;
}

.left-item {
  margin-left: 10px;
  font-size: 40px;
  font-family:'Times New Roman', Times, serif
}

.right-item {
  margin-right: 10px;
  display: flex;
  flex-direction: row;
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

.main-content {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  margin: 20px;
}

.card-container {
    display: flex;
    background-color: white;
    flex-wrap: wrap;
    justify-content: space-between;
    gap: 20px;
    max-width: 400px;
    padding: 100px 100px 100px 100px;
    margin-bottom: 20px;
    border-radius: 10px;
}

.card {
  flex: 0 0 calc(33.33% - 20px);
  min-width: 170px;
  background-color: #ffffff;
  color:black;
  border-radius: 10px;
  box-shadow: 0 4px 6px hsl(0, 0%, 42%);
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 10px;
}

.battery-card {
  background-color: hsl(0, 0%, 95%);
  /* color: white; */
}

.spray-can-card {
  background-color: hsl(0, 0%, 95%);
  /* color: white; */
}

.bottom-card {
  background-color: hsl(0, 0%, 95%);
  display: flex;
  flex-direction: column;
  justify-content: center;
  padding: 20px;
  border-radius: 10px;
  box-shadow: 0 4px 6px hsl(0, 0%, 42%);
  align-items: center;
  margin-left: 35px;
  margin-right: 35px;
}

.buttons-container {
  display: flex;
  flex-direction: row;
  justify-content: center;
  margin-bottom: 10px;
} 

button {
  margin: 5px;
  padding: 10px 20px;
  background-color: #4285F4;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
}

.input-container {
  display: flex;
  flex-direction: row;
  align-items: center;
  margin-top: 10px;
}

label {
  margin-bottom: 5px;
}

input {
  padding: 10px;
  border: 1px solid #ccc;
  border-radius: 5px;
  margin-bottom: 5px;
  width: 100%;
  max-width: 350px;
}

.save-button {
  padding: 10px 20px;
  background-color: #4CAF50 !important;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  width: 100%;
  max-width: 90px;
}

.reset-button {
  padding: 10px 20px;
  background-color: red;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  width: 100%;
  max-width: 90px;
}

  </style>
  <nav class="navbar">
    <div class="left-item">AIR REFRESHER</div>
    <div class="right-item">
      <ul>
        <li>
          <button class="reset-button" type="reset" onclick="resetWiFi()" title="reset credentials">reset</button>
        </li>
        <li>
          <div class="status-indicator status-off" id="connectionStatus"></div>
        </li>
      </ul>
      
    </div>
  </nav>

  <div class="main-content">
    <div class="card-container">
      <div class="card battery-card">
        <h2>Battery Level</h2>
        <p id="batteryPercentage">0</p>
      </div>
      <div class="card spray-can-card" >
        <h2>Spray Level</h2>
        <p id="sprayCanLevel">0</p>
      </div>
    

    <div class="bottom-card">
      <div class="buttons-container">
        <button type="button" name="sprayingTime">10 min</button>
        <button type="button" name="sprayingTime">30 min</button>
        <button type="button" name="sprayingTime">60 min</button>
      </div>
      <div class="input-container">
        <label for="inputField"></label>
        <input type="text" id="inputField" placeholder="Enter spraying time">
        <button class="save-button" type="submit" onclick="setSprayingTime()">Save</button>
      </div>
  </div>
</div>
</div>
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
    int rawValue = analogRead(BATTERY_PIN);
    //Map the raw value to the battery voltage range
    float voltage = (rawValue / 4095.0) * 3.3 * (R1 + R2) / R2; //Convert the raw value to voltage using the voltage divider rule

    //Calculate the battery percentage
    int batteryPercentage = map(voltage *1000, 3300, 4200, 0 , 100); //Map the voltage to percentage ( assuming battery voltage range between 3.3V and 4.2V)
    //batteryPercentage = constrain(batteryPercentage, 0, 100);  //Make sure the battery percentage is within 0-100 range

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
      unsigned long currentMillis = millis();
      unsigned long sprayDurationMillis = SPRAY_TIME_MIN * 60000;

      if (currentMillis - startMillis >= sprayDurationMillis){
        for (int pos = 0; pos <= 45; pos += 1){
          sprayServo.write(pos);
          delay(10);
        }

        for (int pos = 45; pos >= 0; pos -= 1){
          sprayServo.write(pos);
          delay(10);
        }
        startMillis = currentMillis; //Update the previous value for the next Spray cycle
      }
      int SPRAY_TIME_MIN = request->getParam("sprayingTime", true)->value().toInt();
      // Set spraying time
      // startMillis = millis();
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
  // Handle HTTP POST request to set spraying time
  server.on("/setSprayingTime", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("sprayingTime", true)) {
      unsigned long currentMillis = millis();
      unsigned long sprayDurationMillis = SPRAY_TIME_MIN * 60000;

      if (currentMillis - startMillis >= sprayDurationMillis){
        for (int pos = 0; pos <= 45; pos += 1){
          sprayServo.write(pos);
          delay(10);
        }

        for (int pos = 45; pos >= 0; pos -= 1){
          sprayServo.write(pos);
          delay(10);
        }
        startMillis = currentMillis; //Update the previous value for the next Spray cycle
      }
      int SPRAY_TIME_MIN = request->getParam("sprayingTime", true)->value().toInt();
      // Set spraying time
      // startMillis = millis();
      deviceState = true;
    }
    request->send(200);
  });
  // Serial.println(SPRAY_TIME_MIN);


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
    
        //Value read of the spray can level
        int sprayCanContent = loadCell.read();
        
    int rawValue = analogRead(BATTERY_PIN);
    //Map the raw value to the battery voltage range
    float voltage = (rawValue / 4095.0) * 3.3 * (R1 + R2) / R2; //Convert the raw value to voltage using the voltage divider rule

    //Calculate the battery percentage
    int batteryPercentage = map(voltage *1000, 3300, 4200, 0 , 100); //Map the voltage to percentage ( assuming battery voltage range between 3.3V and 4.2V)

  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.print(" V | Battery Percentage: ");
  Serial.print(batteryPercentage);
  Serial.print("%");
  Serial.print("SprayLevel: ");
  Serial.print(sprayCanContent);
  Serial.print("TIMESET(m): ");
  Serial.println(SPRAY_TIME_MIN);

  display.print("BATTERY VOLTAGE: ");
  display.print(batteryPercentage);
  display.println("%");

  display.print("SPRAY: ");
  display.print(sprayCanContent);
  display.println("%");
  display.println();

  display.print("TIMESET(m): ");
  display.println(SPRAY_TIME_MIN);
  

  delay(5000);  // Wait for 5 seconds before taking the next reading
}
