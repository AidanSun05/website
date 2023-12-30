// Include Ethernet and SPI libraries
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// MAC address (dependent on your network)
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 20); // IP address is 192.168.0.20

EthernetServer server(80); // Port 80 is default for HTTP

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
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // Send HTTP resonse header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();

          // Send the page's HTML
          File f = SD.open("index.htm"); // The file to read from

          if (f) {
            while (f.available()) {
              // Send file to client
              client.write(f.read());
            }
            f.close(); // Close file
          } else {
            // Unable to open file
            client.println("Unable to open: index.htm");
          }
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
