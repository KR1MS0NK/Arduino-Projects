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
    <title>AIRREFRESHNER</title>
    <style>
        /* Existing CSS styles */

        /* Custom CSS styles */
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            background-color: #f0f0f0;
        }

        .container {
            max-width: 600px;
            margin: 20px;
            background-color: #ffffff;
            box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.2);
            padding: 20px;
            border-radius: 10px;
        }

        h2 {
            margin-top: 0;
        }

        .btn-success, .btn-danger {
            background-color: #4CAF50;
            border: none;
            color: white;
            padding: 10px 20px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 4px;
            transition: background-color 0.3s;
        }

        .btn-success:hover, .btn-danger:hover {
            background-color: #45a049;
        }

        .btn-group {
            display: flex;
            justify-content: center;
            margin-top: 10px;
        }

        .btn-group label {
            margin: 0 5px;
        }

        .status-container {
            text-align: center;
            margin-top: 20px;
        }

        .status-indicator {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            margin-right: 10px;
        }

        .status-on {
            background-color: green;
        }

        .status-off {
            background-color: red;
        }
    </style>
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
    </script>
</head>
<body>
    <div class="container">
        <header>
            <h1>AIRREFRESHNER</h1>
        </header>
        <section>
            <div class="column">
                <h2>Battery Percentage:</h2>
                <p><span id="batteryPercentage">0</span>%</p>
            </div>
            <div class="column">
                <h2>Spray Can Level:</h2>
                <p><span id="sprayCanLevel">0</span></p>
            </div>
        </section>
        <section>
            <h3>Select Spraying Time:</h3>
            <div class="row">
                <div class="btn-group">
                    <input type="radio" id="5min" name="sprayingTime" value="5" checked>
                    <label for="5min">5 minutes</label>
                    <input type="radio" id="30min" name="sprayingTime" value="30">
                    <label for="30min">30 minutes</label>
                    <input type="radio" id="1hour" name="sprayingTime" value="60">
                    <label for="1hour">1 hour</label>
                    <input type="radio" id="custom" name="sprayingTime" value="">
                    <label for="custom">Custom:</label>
                    <input type="text" id="customTime">
                </div>
            </div>
        </section>
        <section>
            <button class="btn-success" onclick="setSprayingTime()">Set Spraying Time</button>
        </section>
        <section>
            <button class="btn-danger" onclick="resetWiFi()">Reset WiFi Credentials</button>
        </section>
        <section class="status-container">
            <div class="status-indicator status-off"></div>
            <div class="status-label">Device is OFF</div>
        </section>
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
