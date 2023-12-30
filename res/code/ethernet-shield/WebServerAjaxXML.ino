#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// MAC address (dependent on your network)
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 20); // IP address is 192.168.0.20

EthernetServer server(80); // Port 80 is default for HTTP

const int requestLen = 15; // Size of HTTP request array
char request[requestLen];  // HTTP request stored in char array to save memory
int reqIdx           = 0;  // Current index of request

int inputPins[] = {2, 3}; // Digital input pins used
int numPins     = sizeof(inputPins) / sizeof(int); // The number of pins used

void setup() {
  Serial.begin(9600); // Open a serial port
  Serial.println("Ethernet Web Server");

  // -------------- SD Card init start --------------

  // Disable Ethernet chip while SD card is being initialized
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  Serial.print("Initializing SD card... ");
  if (!SD.begin(4)) {
    Serial.println("Failed.");
    return;
  }
  Serial.println("Done.");

  // Check if index.htm exists
  Serial.print("Looking for index.htm... ");
  if (!SD.exists("index.htm")) {
    Serial.println("Failed.");
  }
  Serial.println("Done.");

  // -------------- SD Card init end --------------

  // -------------- Ethernet init start --------------

  Ethernet.begin(mac, ip); // Start Ethernet connection with IP and MAC

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    // The Ethernet shield is not attached
    Serial.println("Ethernet shield was not found");
    return; // Do nothing
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    // Ethernet cable is not connected
    Serial.println("Ethernet cable is not connected.");
  }

  // Start the server
  server.begin();

  // Print the IP address
  Serial.print("Server is at: ");
  Serial.println(Ethernet.localIP());

  // -------------- Ethernet init end --------------

  // Configure digital pins as input pullup
  for (int i = 0; i < numPins; i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // Listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // New client
    // HTTP requests end with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (reqIdx < requestLen) request[reqIdx] = c; // Append c to the request
        reqIdx++;

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // Send HTTP resonse header

          if (contains(request, "GET /inputs")) {
            // Send inputs in XML file
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/xml");
            client.println("Connection: close");
            client.println();
            sendXML(client); // Send XML data to client
          } else {
            // Request to index.htm

            // Open HTM file
            File f; // File to read from SD
            f = SD.open("index.htm");

            // Send HTML to client
            if (f) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.println();
              while (f.available()) {
                client.write(f.read());
              }
              f.close();
            } else {
              // Unable to open file
              client.println("Unable to open file");
            }
          }
          // Clear request and break
          clearRequest();
          break;
        }
        if (c == '\n') {
          // New line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // Character on current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
  }
}

// Clear all elements of the request array
void clearRequest() {
  for (int i = 0; i < requestLen; i++) request[i] = 0;
  reqIdx = 0; // Reset index to 0
}

// Check if string2 is in string1
bool contains(char* string1, char* string2) {
  int found = 0;
  int len = strlen(string1);

  if (strlen(string2) > len) return false;

  for (int i = 0; i < len; i++) {
    if (string1[i] == string2[found]) {
      found++;
      if (strlen(string2) == found) return true;
    }
    else found = 0;
  }

  return false;
}

// Send XML to client containing input data
void sendXML(EthernetClient client) {
  // Send XML data
  client.println("<?xml version='1.0' encoding='UTF-8'?>");
  client.println("<inputs>");

  // Read and send digital pin states
  for (int i = 0; i < numPins; i++) {
    client.print("<digital>");
    client.print(digitalRead(inputPins[i]));
    client.println("</digital>");
  }

  // Read and send A5 state
  client.print("<analog>");
  client.print(analogRead(A5));
  client.println("</analog>");

  client.println("</inputs>");
}
