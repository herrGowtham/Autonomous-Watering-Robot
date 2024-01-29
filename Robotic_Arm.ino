#include <ESP32Servo.h>
#include <WiFi.h>

const int servopin1 = 26;
const int servopin2 = 32;
const int servopin3 = 33;

Servo base_servo;
Servo shoulder_servo;
Servo elbow_servo;

const char* ssid     = "Airel_9841069735";
const char* password = "rbaedu77";

WiFiServer server(80);

String header;

String valueString = String(5);
String valueString1 = String(5);
String valueString2 = String(5);
String valueString3 = String(5);

String idString = String(5);


int pos1 = 0;
int pos2 = 0;
int pos3 = 0;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {

  Serial.begin(115200);
  base_servo.attach(servopin1);
  shoulder_servo.attach(servopin2);
  elbow_servo.attach(servopin3);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  // base_servo.write(90);
  // shoulder_servo.write(90);
  // forearm_servo.write(90);
}

void loop() {
  
  WiFiClient client = server.available();   // Listen for incoming clients

   if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            // Web Page
            client.println("</head><body><h1>Autonomous Watering Robot Control</h1>");
            
            client.println("<h2>Base Servo</h2>");
            client.println("<p>Position: <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value,1)\" value=\""+valueString1+"\"/>");
            
            client.println("<h2>Shoulder Servo</h2>");
            client.println("<p>Position: <span id=\"servoPos2\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider2\" onchange=\"servo(this.value,2)\" value=\""+valueString2+"\"/>");
            
            client.println("<h2>Elbow Servo</h2>");
            client.println("<p>Position: <span id=\"servoPos3\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider3\" onchange=\"servo(this.value,3)\" value=\""+valueString3+"\"/>");
            
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var slider2 = document.getElementById(\"servoSlider2\");");
            client.println("var slider3 = document.getElementById(\"servoSlider3\");");

            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("var servoP2 = document.getElementById(\"servoPos2\"); servoP2.innerHTML = slider2.value;");
            client.println("var servoP3 = document.getElementById(\"servoPos3\"); servoP3.innerHTML = slider3.value;");

            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("slider2.oninput = function() { slider2.value = this.value; servoP2.innerHTML = this.value; }");
            client.println("slider3.oninput = function() { slider3.value = this.value; servoP3.innerHTML = this.value; }");
            
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos,i) { ");
            client.println("console.log(i);");
            client.println("$.get(\"/?value=\" + pos + \"&\" + i); {Connection: close};}</script>");

            // client.println("$.ajaxSetup({timeout:1000}); function servo2(pos) { ");
            // client.println("$.get(\"/?2value=\" + pos + \"&\"); {Connection: close};}</script>");
           
            client.println("</body></html>");

            //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              idString = header.substring(pos2+1);

              int id = idString.toInt();

              if (id == 1)
              {
                valueString1 = valueString;
                base_servo.write(valueString1.toInt());
                Serial.println(idString);
                Serial.println(valueString1); 
              }
              else if (id == 2)
              {
                valueString2 = valueString;
                shoulder_servo.write(valueString2.toInt());
                Serial.println(idString);
                Serial.println(valueString2); 
              }
              else if (id == 3)
              {
                valueString3 = valueString;
                elbow_servo.write(valueString3.toInt());
                Serial.println(idString);
                Serial.println(valueString3);
              }
            
            }         
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }

    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
