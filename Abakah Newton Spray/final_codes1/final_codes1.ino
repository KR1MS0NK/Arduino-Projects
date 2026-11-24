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
 /* Common styles for all screen sizes */
//body {
//  font-family: Arial, sans-serif;
//  margin: 0;
//  padding: 0;
//}
//
//.taskbar {
//  background-color: #333;
//  color: #fff;
//  display: flex;
//  justify-content: space-between;
//  align-items: center;
//  padding: 10px;
//}
//
//.title {
//  font-size: 20px;
//}
//
//.reset-icon {
//  font-size: 18px;
//  cursor: pointer;
//}
//
//.main-card {
//  background-color: #fff;
//  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
//  border-radius: 5px;
//  padding: 20px;
//  margin: 10px;
//  min-width: 300px;
//}
//
//.opposite-cards {
//  display: flex;
//  flex-direction: column;
//}
//
//.card {
//  background-color: #f1f1f1;
//  border: 1px solid #ccc;
//  border-radius: 5px;
//  padding: 10px;
//  margin: 10px 0;
//  min-width: 140px;
//}
//
//.card p {
//  font-size: 16px;
//  margin: 0;
//}
//
//.card button {
//  margin: 5px;
//  padding: 8px 12px;
//  background-color: #333;
//  color: #fff;
//  border: none;
//  border-radius: 4px;
//  cursor: pointer;
//}
//
//.input-group {
//  display: flex;
//  flex-direction: column;
//  align-items: flex-start;
//  margin-top: 10px;
//}
//
//.input-group label {
//  margin-bottom: 5px;
//}
//
//.input-group input {
//  width: 100%;
//  padding: 8px;
//  margin-bottom: 5px;
//  border: 1px solid #ccc;
//  border-radius: 4px;
//}
//
//.save-btn {
//  background-color: #4CAF50;
//  color: #fff;
//  border: none;
//  border-radius: 4px;
//  cursor: pointer;
//}
//
///* Additional styling for the reset icon */
//.reset-icon {
//  cursor: pointer;
//}
//
///* Media Queries for Responsive Design */
//
///* For devices with a maximum width of 768px (e.g., tablets and mobile phones) */
//@media (max-width: 768px) {
//  .taskbar {
//    flex-direction: column;
//    align-items: flex-start;
//    padding: 20px;
//  }
//
//  .title {
//    margin-bottom: 10px;
//  }
//
//  .reset-icon {
//    order: -1; /* Move the reset icon to the top */
//  }
//
//  .main-card {
//    min-width: auto;
//    width: 100%;
//  }
//
//  .opposite-cards {
//    flex-direction: row;
//  }
//
//  .card {
//    flex: 1;
//    margin: 10px;
//  }
//
//  .input-group {
//    flex-direction: row;
//    align-items: center;
//    width: 100%;
//  }
//
//  .input-group label {
//    margin-right: 10px;
//  }
//}


    </style>
  <div class="taskbar">
    <div class="title">AIR REFRESHER</div>
    <button class="reset-button" type="reset" onclick="resetWiFi()" title="reset credentials">reset</button>
  </div>

  <div class="main-card">
    <p>AIR REFRESHER</p>
    <div class="opposite-cards">
      <div class="card">
        <h2>Battery Level</h2>
        <p id= "batteryPercentage">0</p>
      </div>
      <div class="card">
        <h2>Spray Level</h2>
        <p id="sprayCanLevel">0</p>
      </div>
    </div>

    <div class="card">
      <p>Card with 3 buttons and input field</p>
      <button type="button" name="sprayingTime">10 min</button>
      <button type="button" name="sprayingTime">30 min</button>
      <button type="button" name="sprayingTime">60 min</button>
      <div class="input-group">
        <label for="input-data">Input data:</label>
        <input type="text" id="input-data" placeholder="Enter sprayingTime">
        <button class="save-button" type="submit" onclick="setSprayingTime()">Save</button>
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

  WiFi.mode (WIFI_STA);

  esp_wifi_set_ps(WIFI_PS_NONE);
  
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
