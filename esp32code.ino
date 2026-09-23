#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>



WebServer server(80);

// ===== SERVO OBJECTS =====
Servo servo1;
Servo servo2;

// ===== SERVO PINS =====
int servo1Pin = 33;
int servo2Pin = 27;

// ===== PASSWORDS =====
String password1 = "box1";
String password2 = "box2";

// ===== LOGIN PAGE =====
String loginPage = R"rawliteral(
<!DOCTYPE html>
<html>

<head>

<title>ESP32 Smart Box</title>

<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

body{
  background:#111827;
  color:white;
  text-align:center;
  font-family:Arial;
  margin-top:80px;
}

input{
  width:230px;
  height:45px;
  font-size:20px;
  border:none;
  border-radius:10px;
  padding-left:10px;
}

button{
  width:180px;
  height:50px;
  font-size:20px;
  border:none;
  border-radius:10px;
  margin-top:20px;
  background:green;
  color:white;
}

</style>

</head>

<body>

<h1>SMART DOCUMENT BOX</h1>

<input type="password" id="pass" placeholder="Enter Password">

<br>

<button onclick="login()">
LOGIN
</button>

<script>

function login(){

  var p = document.getElementById("pass").value;

  if(p == "box1"){
    window.location.href="/box1";
  }

  else if(p == "box2"){
    window.location.href="/box2";
  }

  else{
    alert("Wrong Password");
  }
}

</script>

</body>
</html>
)rawliteral";

// ===== BOX 1 PAGE =====
String box1Page = R"rawliteral(
<!DOCTYPE html>
<html>

<head>

<title>Box 1 Control</title>

<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

body{
  background:#0f172a;
  color:white;
  text-align:center;
  font-family:Arial;
  margin-top:80px;
}


}

.openBtn{
  background:green;
}

.closeBtn{
  background:red;
}

</style>

</head>

<body>

<h1>BOX 1 CONTROL</h1>

<button class="openBtn" onclick="fetch('/open1')">
OPEN
</button>

<button class="closeBtn" onclick="fetch('/close1')">
CLOSE
</button>

</body>
</html>
)rawliteral";

// ===== BOX 2 PAGE =====
String box2Page = R"rawliteral(
<!DOCTYPE html>
<html>

<head>

<title>Box 2 Control</title>

<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

body{
  background:#111827;
  color:white;
  text-align:center;
  font-family:Arial;
  margin-top:80px;
}



.openBtn{
  background:blue;
}

.closeBtn{
  background:orange;
}

</style>

</head>

<body>

<h1>BOX 2 CONTROL</h1>

<button class="openBtn" onclick="fetch('/open2')">
OPEN
</button>

<button class="closeBtn" onclick="fetch('/close2')">
CLOSE
</button>

</body>
</html>
)rawliteral";

// ===== ROUTES =====

void handleRoot() {
  server.send(200, "text/html", loginPage);
}

void handleBox1() {
  server.send(200, "text/html", box1Page);
}

void handleBox2() {
  server.send(200, "text/html", box2Page);
}

// ===== SERVO 1 =====

void open1() {

  Serial.println("BOX 1 OPEN");

  servo1.write(90);

  server.send(200, "text/plain", "Box1 Open");
}

void close1() {

  Serial.println("BOX 1 CLOSE");

  servo1.write(0);

  server.send(200, "text/plain", "Box1 Close");
}

// ===== SERVO 2 =====

void open2() {

  Serial.println("BOX 2 OPEN");

  servo2.write(90);

  server.send(200, "text/plain", "Box2 Open");
}

void close2() {

  Serial.println("BOX 2 CLOSE");

  servo2.write(0);

  server.send(200, "text/plain", "Box2 Close");
}

void setup() {

  Serial.begin(115200);

  // ===== SERVO SETUP =====
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);

  servo1.attach(33, 500, 2400);
  servo2.attach(27, 500, 2400);

  servo1.write(0);
  servo2.write(0);

  // ===== WIFI =====
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println("");

  Serial.println("WiFi Connected");

  Serial.print("IP Address: ");

  Serial.println(WiFi.localIP());

  // ===== SERVER ROUTES =====
  server.on("/", handleRoot);

  server.on("/box1", handleBox1);
  server.on("/box2", handleBox2);

  server.on("/open1", open1);
  server.on("/close1", close1);

  server.on("/open2", open2);
  server.on("/close2", close2);

  server.begin();

  Serial.println("Server Started");
}

void loop() {

  server.handleClient();
}