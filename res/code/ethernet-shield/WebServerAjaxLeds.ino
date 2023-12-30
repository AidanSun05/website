#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// MAC address (dependent on your network)
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 20); // IP address is 192.168.0.20

EthernetServer server(80); // Port 80 is default for HTTP

const int requestLen = 20; // Size of HTTP request array
char request[requestLen];  // HTTP request stored in char array to save memory
int reqIdx           = 0;  // Current index of request

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

  // Configure digital 2 as output
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW); // LED off on start
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
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          if (contains(request, "GET / ") || contains(request, "GET /index.htm")) {
            // Request to index.htm
            File f = SD.open("index.htm");

            // Send HTML to client
            if (f) {
              while (f.available()) {
                client.write(f.read());
              }
              f.close();
            } else {
              // Unable to open file
              client.println("Unable to open file");
            }
          } else if (contains(request, "GET /led")) {
            // Request from AJAX server to change LED state
            Serial.print("LED state received: ");

            if (contains(request, "state=on")) {
              Serial.println("On");
              digitalWrite(2, HIGH);
            } else if (contains(request, "state=off")) {
              Serial.println("Off");
              digitalWrite(2, LOW);
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
