// CONTROL DE LED POR WI-FI

// Librería de Wi-Fi
#include <ESP8266WiFi.h>

// Información de red
const char* ssid     = "INFINITUM4602_2.4";
const char* password = "EWeuT2C9hT";

// El puerto 80 es el más utilizado para Web
WiFiServer server(8888);

// Variable para redireccionar el URL
String header;

// Valores iniciales de los estados de los LED
String amarilloState = "off";
String verdeState = "off";
String rojoState = "off";
String azulState = "off";

// Asignación de nombres a los números de pines a usar
const int amarillo = 5;
const int verde = 4;
const int rojo = 14;
const int azul = 12;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200); // Esta parte se refiere a la cantidad de símbolos por segundo que se pueden transmitir, se mide en baudios
  // Inicializa las variables como salidas
  pinMode(amarillo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(azul, OUTPUT);
  // Pone las salidas en alto, porque de esta manera, los LED se apagan (los LED amarillo y verde están conectados a un relevador y los LED rojo y azul son de un LED RBG)
  digitalWrite(amarillo, HIGH);
  digitalWrite(verde, HIGH);
  digitalWrite(rojo, HIGH);
  digitalWrite(azul, HIGH);

  // Conectar a la red
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Imprime la dirección IP e inicia la Web
  Serial.println("");
  Serial.println("WiFi connected.");
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(5000);
  Serial.print("Go to: http://");
  Serial.print(WiFi.localIP());
  Serial.println(":8888");
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
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

            Serial.print(header);
            
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:

            if(header.indexOf("YWxiZXJ0OmNobQ==") >= 0) {

              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              if(header.indexOf("GET / HTTP/1.1") >= 0) {
                Serial.println("Main Web Page");
              }
              else if (header.indexOf("GET /amarillo/on") >= 0) {
                Serial.println("LED amarillo on");
                amarilloState = "on";
                digitalWrite(amarillo, LOW);
              } else if (header.indexOf("GET /amarillo/off") >= 0) {
                Serial.println("LED amarillo off");
                amarilloState = "off";
                digitalWrite(amarillo, HIGH);
              } else if (header.indexOf("GET /verde/on") >= 0) {
                Serial.println("LED verde on");
                verdeState = "on";
                digitalWrite(verde, LOW);
              } else if (header.indexOf("GET /verde/off") >= 0) {
                Serial.println("LED verde off");
                verdeState = "off";
                digitalWrite(verde, HIGH);
              } else if (header.indexOf("GET /rojo/on") >= 0) {
                Serial.println("LED rojo on");
                rojoState = "on";
                digitalWrite(rojo, LOW);
              } else if (header.indexOf("GET /rojo/off") >= 0) {
                Serial.println("LED rojo off");
                rojoState = "off";
                digitalWrite(rojo, HIGH);
              } else if (header.indexOf("GET /azul/on") >= 0) {
                Serial.println("LED azul on");
                azulState = "on";
                digitalWrite(azul, LOW);
              } else if (header.indexOf("GET /azul/off") >= 0) {
                Serial.println("LED azul off");
                azulState = "off";
                digitalWrite(azul, HIGH);
              }
            
              // Página wrb
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS to style the on/off buttons 
              // Feel free to change the background-color and font-size attributes to fit your preferences
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #77878A;}</style></head>");
              
              // Cabecera de la página
              client.println("<body><h2>Control de LED por Wi-Fi</h2>");
              
              // Muestra el estado del LED amarillo
              client.println("<p>LED amarillo - State " + amarilloState + "</p>");   
              if (amarilloState=="off") {
                client.println("<p><a href=\"/amarillo/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/amarillo/off\"><button class=\"button button2\">OFF</button></a></p>");
              } 
                 
              // Muestra el estado del LED verde 
              client.println("<p>LED verde - State " + verdeState + "</p>");     
              if (verdeState=="off") {
                client.println("<p><a href=\"/verde/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/verde/off\"><button class=\"button button2\">OFF</button></a></p>");
              }
  
              // Muestra el estado del LED rojo
              client.println("<p>LED rojo - State " + rojoState + "</p>");     
              if (rojoState=="off") {
                client.println("<p><a href=\"/rojo/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/rojo/off\"><button class=\"button button2\">OFF</button></a></p>");
              }
  
              // Muestra el estado del LED azul
              client.println("<p>LED azul - State " + azulState + "</p>");    
              if (azulState=="off") {
                client.println("<p><a href=\"/azul/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/azul/off\"><button class=\"button button2\">OFF</button></a></p>");
              }
              client.println("</body></html>");
            }
            else {
              client.println("HTTP/1.1 401 Unauthorized");
              client.println("WWW-Authenticate: Basic realm=\"Secure\"");
              client.println("Content-Type: text/html");
              client.println();
              client.println("<html>Authentication failed</html>");
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
