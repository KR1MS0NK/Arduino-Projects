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
AsyncWebServer server(80);
WiFiManager wifiManager;

bool deviceState = false;
unsigned long startMillis = 0;
const int SPRAY_TIME_MIN = 5;
const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
 <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet"
    integrity="sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM" crossorigin="anonymous">
  <link href="/docs/5.3/dist/css/bootstrap.min.css" rel="stylesheet"
  integrity="sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM" crossorigin="anonymous">
    <link  rel="stylesheet" href='https://unpkg.com/boxicons@2.1.4/css/boxicons.min.css' >
  <meta charset="utf-8">
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-4bw+/aepP/YC94hEpVNVgiZdgIC5+VKNBQNGCHeKRQN+PtmoHDEXuppvnDJzQIu9" crossorigin="anonymous">
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-HwwvtgBNo3bZJJLYd8oVXjrBZt8cqVSpeBNS5n7C8IVInixGAoxmnlMuBnhbgrkm" crossorigin="anonymous"></script>
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

h1 {
  font-family: fangsong;
}

.navbar-expand-lg .navbar-collapse {
  flex-direction: row-reverse;
}

nav ul li a {
    display: block;
    position: relative;
    font-family: 'Segoe UI', 'serif';
    text-decoration: none;
    /*text-transform: uppercase;*/
    color: hsl(0, 0%, 0%);
    padding: 15px 15px 15px 13px;
    transition: 0.5s;
}

nav ul li a:hover{
    color:  hsl(120, 4%, 48%);
}

#dropdown-item {
  font-size: medium;
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


  </style>
  <div class="modal modal-sheet position-static d-block bg-body-secondary p-4 py-sm-5 align-items-center" tabindex="-1"
    role="dialog" id="modalSheet">

    <nav class="navbar navbar-expand-lg bg-body-tertiary">
      <div class="container-fluid">
        <div class="modal-title text-center">
          <h1 class="display-5 fw-bold text-center">Air Refresher</h1>
        </div>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNavDropdown" aria-controls="navbarNavDropdown" aria-expanded="false" aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNavDropdown">
          <ul class="navbar-nav">
              <li>            
                <div class="status-indicator status-off" id="connectionStatus"></div>
                <p class="status-label" id="deviceStatusLabel">Device is OFF</p>
                
                <div class="form-check form-switch text-center">
                    <input class="form-check-input" type="checkbox" id="toggleSwitch" onchange="toggleDeviceState()">
                    <label class="form-check-label" for="toggleSwitch">Device ON/OFF</label>
                </div>
            </li>
            </li>
            <li class="nav-item">
              <i class='bx bx-reset' onclick="resetWiFi()" title="reset credentials " ></i>
            </li>
          </ul>
        </div>
      </div>
    </nav>

    <div class="modal-dialog row" role="document">
      <div class="modal-dialog col" role="document">
        <div class="modal-content rounded-4 shadow">
          <div class="modal-header border-bottom-0">
            <h2 class="modal-title text-centre fs-5 text-center">Spray Percentage</h2>
          </div>
          <div class="modal-footer flex-column align-items-stretch w-100 gap-2 pb-3 border-top-0">
            <p><span id="sprayCanLevel">0</span></p>
          </div>
        </div>
      </div>
      <div class="modal-dialog col" role="document">
        <div class="modal-content rounded-4 shadow">
          <div class="modal-header border-bottom-0">
            <h2 class="modal-title text-centre fs-5 text-center">Battery Percentage</h2>
          </div>
          <div class="modal-footer flex-column align-items-stretch w-100 gap-2 pb-3 border-top-0">
            <p><span id="batteryPercentage">0</span>%</p>
          </div>
        </div>
      </div>
      <div class="modal-dialog" role="document">
        <div class="modal-content rounded-4 shadow">
          <div class="modal-header border-bottom-0">
            <h2 class="modal-title text-centre fs-5 text-center">Spray Interval</h2>
          </div>
          <div class="modal-footer flex-column align-items-stretch w-100 gap-2 pb-3 border-top-0">
            <div class="row mb-3 text-center">
              <div class="col-sm-4 themed-grid-col">
                <button type="button" class="btn btn-lg btn-primary" name="sprayingTime">10 min</button></div>
              <div class="col-sm-4 themed-grid-col">
                <button type="button" class="btn btn-lg btn-primary" name="sprayingTime">30 min</button></div>
              <div class="col-sm-4 themed-grid-col">
                <button type="button" class="btn btn-lg btn-primary" name="sprayingTime">60 min</button></div>
            </div>
            <div class="row mb-3 text-center">
              <div class="col-sm-8 themed-grid-col">
                <div class="form-floating mb-3">
                  <input class="form-control rounded-3" id="customTime" placeholder="spray time">
                  <label for="floatingInput">Spray Time</label></div>
              </div>
              <div class="col-sm-4 themed-grid-col">
                <button type="submit" class="btn btn-success" onclick="setSprayingTime()">Save</button></div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</body>
</html>
)=====";

    
void setup() {
  pinMode(LED_PIN, OUTPUT);
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

  // Handle HTTP GET request to retrieve battery percentage and spray can level
  server.on("/spray-can-level", HTTP_GET, [](AsyncWebServerRequest *request){
    int sprayCanContent = loadCell.read();
    int batteryPercentage = analogRead(35);
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
    if(deviceState && (millis() - startMillis >= SPRAY_TIME_MIN * 60000)) {
        sprayServo.write(0);
        delay(1000);
        sprayServo.write(180);
        delay(1000);

        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);

        deviceState = false;
    }
}
