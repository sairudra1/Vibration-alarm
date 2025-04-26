#include <WiFi.h>
#include <WebServer.h>
#include "time.h"
const char* ssid = "TP-Link_1CC6";
const char* password = "Gudur@12";


const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // IST offset (5 hours 30 minutes)
const int daylightOffset_sec = 0; // No daylight savings in IST

WebServer server(80);
int hour1 = -1, minute1 = -1, hour2 = -1, minute2 = -1; // Initial unset values for alarms
const int buzzerPin = 13; // GPIO pin for the buzzer
/*
void handleRoot() {
  String html = "<html><head>";
  html += "<style>body {font-family: Arial, sans-serif; text-align: center;}";
  html += ".container {display: inline-block; text-align: left; margin-top: 50px; padding: 20px;}";
  html += "input[type='range'] {width: 100%;}";
  html += "input[type='submit'] {padding: 10px 20px; background-color: #4CAF50; color: white;}</style>";
  html += "<script>function updateSliderValue(sliderId, valueId) {";
  html += "var slider = document.getElementById(sliderId);";
  html += "var value = document.getElementById(valueId);";
  html += "value.innerHTML = slider.value;}</script>";
  html += "</head><body>";
  html += "<div class='container'><h1>Set Alarms</h1>";
  html += "<form action='/submit' method='get'>";
  html += "Hour1: <input type='range' name='hour1' id='hour1' min='0' max='23' onchange=\"updateSliderValue('hour1', 'hour1Value')\">";
  html += "<span id='hour1Value'>0</span><br>Minute1: <input type='range' name='minute1' id='minute1' min='0' max='59' onchange=\"updateSliderValue('minute1', 'minute1Value')\">";
  html += "<span id='minute1Value'>0</span><br>";
  html += "Hour2: <input type='range' name='hour2' id='hour2' min='0' max='23' onchange=\"updateSliderValue('hour2', 'hour2Value')\">";
  html += "<span id='hour2Value'>0</span><br>Minute2: <input type='range' name='minute2' id='minute2' min='0' max='59' onchange=\"updateSliderValue('minute2', 'minute2Value')\">";
  html += "<span id='minute2Value'>0</span><br><br>";
  html += "<input type='submit' value='Submit'></form></div></body></html>";
  server.send(200, "text/html", html);
}*/
void handleRoot() {
  String html = "<html><head>";
  html += "<style>body {font-family: Arial, sans-serif; background-color: #f2f2f2; margin: 0; padding: 0;}";
  html += "h1 {color: #333333;}";
  html += ".container {max-width: 400px; margin: 50px auto; background-color: white; padding: 20px; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);}";
  html += ".slider-label {font-size: 18px; margin-bottom: 10px; color: #666666;}";
  html += "input[type='range'] {width: 100%; margin-bottom: 20px;}";
  html += ".slider-value {font-weight: bold; color: #333333;}";
  html += "input[type='submit'] {padding: 10px 20px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; font-size: 16px; cursor: pointer; transition: background-color 0.3s ease;}";
  html += "input[type='submit']:hover {background-color: #45a049;}";
  html += ".info {margin-top: 10px; font-size: 14px; color: #999999;}";
  html += "</style>";
  html += "<script>function updateSliderValue(sliderId, valueId) {";
  html += "var slider = document.getElementById(sliderId);";
  html += "var value = document.getElementById(valueId);";
  html += "value.innerHTML = slider.value;}</script>";
  html += "</head><body>";
  html += "<div class='container'><h1>Set Alarms</h1>";
  html += "<form action='/submit' method='get'>";
  html += "<div class='slider-label'>Hour 1: <span id='hour1Value' class='slider-value'>0</span></div>";
  html += "<input type='range' name='hour1' id='hour1' min='-1' max='23' onchange=\"updateSliderValue('hour1', 'hour1Value')\">";
  html += "<div class='slider-label'>Minute 1: <span id='minute1Value' class='slider-value'>0</span></div>";
  html += "<input type='range' name='minute1' id='minute1' min='-1' max='59' onchange=\"updateSliderValue('minute1', 'minute1Value')\">";
  html += "<div class='slider-label'>Hour 2: <span id='hour2Value' class='slider-value'>0</span></div>";
  html += "<input type='range' name='hour2' id='hour2' min='-1' max='23' onchange=\"updateSliderValue('hour2', 'hour2Value')\">";
  html += "<div class='slider-label'>Minute 2: <span id='minute2Value' class='slider-value'>0</span></div>";
  html += "<input type='range' name='minute2' id='minute2' min='-1' max='59' onchange=\"updateSliderValue('minute2', 'minute2Value')\">";
  html += "<br><br><input type='submit' value='Set Alarms'></form>";
  html += "<div class='info'>Please ensure your device is connected to the network.</div>";
  html += "<div class='info'>if selected -1 then it mean alarm is deactivated.</div>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}


void handleSubmit() {
  if (server.hasArg("hour1") && server.hasArg("minute1") && server.hasArg("hour2") && server.hasArg("minute2")) {
    hour1 = server.arg("hour1").toInt();
    minute1 = server.arg("minute1").toInt();
    hour2 = server.arg("hour2").toInt();
    minute2 = server.arg("minute2").toInt();

    Serial.println("Alarm 1 set to " + String(hour1) + ":" + String(minute1));
    Serial.println("Alarm 2 set to " + String(hour2) + ":" + String(minute2));

    server.send(200, "text/plain", "Alarms set successfully!");
  } else {
    server.send(400, "text/plain", "Bad Request - Missing parameters");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Initialize time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Setup web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/submit", HTTP_GET, handleSubmit);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();

  // Get the current time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  int currentHour = timeinfo.tm_hour;
  int currentMinute = timeinfo.tm_min;
  Serial.print("Current time: ");
  Serial.print(currentHour);
  Serial.print(":");
  Serial.println(currentMinute);

  // Check if it's time for Alarm 1
  if (hour1 == currentHour && minute1 == currentMinute) {
    Serial.println("Ringing Alarm 1!");
    ringBuzzer();
  }

  // Check if it's time for Alarm 2
  if (hour2 == currentHour && minute2 == currentMinute) {
    Serial.println("Ringing Alarm 2!");
    ringBuzzer();
  }
  
  delay(2500);
  Serial.println("");
  Serial.print("Alarm 1 at:-  ");
  Serial.print(hour1);
  Serial.print(" : ");
  Serial.print(minute1);

  Serial.println("");
  Serial.print("Alarm 2 at:-  ");
  Serial.print(hour2);
  Serial.print(" : ");
  Serial.print(minute2);

  Serial.println("");

  //delay(1000); // Check the time every second
}

void ringBuzzer() {
  digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
  delay(5000);                   // Keep the buzzer on for 5 seconds
  digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
}


/*
void handleRoot() {
  String html = "<html><head>";
  html += "<style>";
  html += "body {font-family: Arial, sans-serif; text-align: center;}"; 
  html += ".container {display: inline-block; text-align: left; margin-top: 50px; border: 1px solid #ddd; padding: 20px; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);}";
  html += "h1, h2, h3 {margin: 10px 0;}"; 
  html += "input[type='range'] {width: 100%;}"; 
  html += "input[type='submit'] {padding: 10px 20px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer;}"; 
  html += "input[type='submit']:hover {background-color: #45a049;}"; 
  html += "span {margin-left: 10px; font-weight: bold;}"; 
  html += "</style>";
  
  html += "<script>";
  html += "function updateSliderValue(sliderId, valueId) {"; 
  html += "var slider = document.getElementById(sliderId);"; 
  html += "var value = document.getElementById(valueId);"; 
  html += "value.innerHTML = slider.value;"; 
  html += "}";
  html += "</script>";
  
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<h1>IPD Project</h1>";
  html += "<h2>In 24-hour format</h2>";
  
  html += "<form action='/submit' method='get'>";
  
  // Alarm 1 sliders
  html += "<h3>Set Time Intervals (Modifiable Alarm 1) (0-23 Hours, 0-59 Minutes)</h3>";
  html += "Hour1: <input type='range' name='hour1' id='hour1' min='0' max='23' onchange=\"updateSliderValue('hour1', 'hour1Value')\">";
  html += "<span id='hour1Value'>0</span><br><br>";
  html += "Minute1: <input type='range' name='minute1' id='minute1' min='0' max='59' onchange=\"updateSliderValue('minute1', 'minute1Value')\">";
  html += "<span id='minute1Value'>0</span><br><br>";
  
  // Alarm 2 sliders
  html += "<h3>Set Time Intervals (Modifiable Alarm 2) (0-23 Hours, 0-59 Minutes)</h3>";
  html += "Hour2: <input type='range' name='hour2' id='hour2' min='0' max='23' onchange=\"updateSliderValue('hour2', 'hour2Value')\">";
  html += "<span id='hour2Value'>0</span><br><br>";
  html += "Minute2: <input type='range' name='minute2' id='minute2' min='0' max='59' onchange=\"updateSliderValue('minute2', 'minute2Value')\">";
  html += "<span id='minute2Value'>0</span><br><br>";
  
  html += "<input type='submit' value='Submit'>";
  html += "</form></div>";
  
  html += "</body></html>";
  server.send(200, "text/html", html);
}

*/
