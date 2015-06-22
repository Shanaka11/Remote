#include <SPI.h>
#include <WiFi.h>
#include <IRremote.h>

char ssid[] = "MIFI-B818";      //  your network SSID (name) 
char pass[] = "1234567890";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
IRsend irsend;

IRrecv irrecv(11);

decode_results results;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  irrecv.enableIRIn(); // Start the receiver

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
  Serial.println("WiFi shield not present"); 
   while(true);        // don't continue
   } 

   // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
     status = WiFi.begin(ssid, pass);
     // wait 10 seconds for connection:
     delay(10000);
   } 
  server.begin();                           // start the web server on port 80
  printWifiStatus();        // you're connected now, so print out the        status
  
}

int codeType = -1; // The type of code
unsigned long codeValue; // The code value if not raw
unsigned int rawCodes[RAWBUF]; // The durations if raw
int codeLen; // The length of the code
int toggle = 0; // The RC5/6 toggle state

void loop() {


  WiFiClient client = server.available();   // listen for incoming clients
  
  
    
    if(irrecv.decode(&results)){
        storeCode(&results);
        irrecv.resume(); // resume receiver
    }
    
    
    
    

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";      // make a String to hold incoming data from the client
    
    while (client.connected()) {            // loop while the client's connected
     if (client.available()) {             // if there's bytes to read from the client,
     char c = client.read();             // read a byte, then
     //Serial.write(c);                    // print it out the serial monitor
     if (c == '\n') {                    // if the byte is a newline character

      // if the current line is blank, you got two newline characters in a row.
      // that's the end of the client HTTP request, so send a response:
      if (currentLine.length() == 0) {  
        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
        // and a content-type so the client knows what's coming, then a blank line:    
        //client.println("HTTP/1.1 200 OK");
        //client.println("Content-type:text/html");
        //client.println();

        // the content of the HTTP response follows the header:
        //client.println("Working");
        
        // The HTTP response ends with another blank line:
        //client.println();
        // break out of the while loop:
        break;         
       } 
       else {      // if you got a newline, then clear currentLine:
         currentLine = "";
       }
     }     
     else if (c != '\r') {  // if you got anything else but a carriage return character,
      currentLine += c;      // add it to the end of the currentLine
    }
    if (currentLine.endsWith("/send")) {
      
       int len = currentLine.length();
       String st1 = currentLine.substring(5,len-5);
       String st2 = st1.substring(0,1);
       String st3 = st1.substring(2);
       
       char cArr[100];
       
       codeType = st2.toInt();
       
       if (codeType == SONY){
         st3 = "0x" + st3;
       }
       
       st3.toCharArray(cArr,100);
       
       codeValue = strtoul(cArr,0,16);
       
       Serial.println(st1);
       Serial.println(cArr);
       Serial.println(codeType);
       Serial.println(codeValue,HEX);
       
       sendCode(0);
       
       client.println("HTTP/1.1 200 OK");
       client.println("Content-type:text/html");
       client.println();
    }else if(currentLine.endsWith("/recv")){
      /*
      if(irrecv.decode(&results)){
        storeCode(&results);
        irrecv.resume(); // resume receiver
       }
      */
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println();
      Serial.println(codeType);
      client.println(codeType);
      client.println("<br>");
      Serial.println(codeValue,HEX);
      client.println(codeValue,HEX);
      client.println();
    }
   }
 }
// close the connection:
client.stop();
Serial.println("client disonnected");
irrecv.enableIRIn(); // Start the receiver
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
 Serial.println(WiFi.SSID());

 // print your WiFi shield's IP address:
 IPAddress ip = WiFi.localIP();
 Serial.print("IP Address: ");
 Serial.println(ip);

 // print the received signal strength:
 long rssi = WiFi.RSSI();
 Serial.print("signal strength (RSSI):");
 Serial.print(rssi);
 Serial.println(" dBm");
// print where to go in a browser:
Serial.print("To see this page in action, open a browser to http://");
Serial.println(ip);
 }
 
 void storeCode(decode_results *results) {
  codeType = results->decode_type;
  int count = results->rawlen;
  if (codeType == UNKNOWN) {
    Serial.println("Received unknown code, saving as raw");
    codeLen = results->rawlen - 1;
    // To store raw codes:
    // Drop first value (gap)
    // Convert from ticks to microseconds
    // Tweak marks shorter, and spaces longer to cancel out IR receiver distortion
    for (int i = 1; i <= codeLen; i++) {
      if (i % 2) {
        // Mark
        rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK - MARK_EXCESS;
        Serial.print(" m");
      } 
      else {
        // Space
        rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK + MARK_EXCESS;
        Serial.print(" s");
      }
      Serial.print(rawCodes[i - 1], DEC);
    }
    Serial.println("");
  }
  else {
    if (codeType == NEC) {
      Serial.print("Received NEC: ");
      if (results->value == REPEAT) {
        // Don't record a NEC repeat value as that's useless.
        Serial.println("repeat; ignoring.");
        return;
      }
    } 
    else if (codeType == SONY) {
      Serial.print("Received SONY: ");
    } 
    else if (codeType == RC5) {
      Serial.print("Received RC5: ");
    } 
    else if (codeType == RC6) {
      Serial.print("Received RC6: ");
    } 
    else {
      Serial.print("Unexpected codeType ");
      Serial.print(codeType, DEC);
      Serial.println("");
    }
    Serial.println(results->value, HEX);
    codeValue = results->value;
    codeLen = results->bits;
  }
}

void sendCode(int repeat) {
  if (codeType == NEC) {
    if (repeat) {
      irsend.sendNEC(REPEAT, codeLen);
      Serial.println("Sent NEC repeat");
    } 
    else {
      irsend.sendNEC(codeValue, codeLen);
      Serial.print("Sent NEC ");
      Serial.println(codeValue, HEX);
    }
  } 
  else if (codeType == SONY) {
    irsend.sendSony(codeValue, codeLen);
    Serial.print("Sent Sony ");
    Serial.println(codeValue, HEX);
  } 
  else if (codeType == RC5 || codeType == RC6) {
    if (!repeat) {
      // Flip the toggle bit for a new button press
      toggle = 1 - toggle;
    }
    // Put the toggle bit into the code to send
    codeValue = codeValue & ~(1 << (codeLen - 1));
    codeValue = codeValue | (toggle << (codeLen - 1));
    if (codeType == RC5) {
      Serial.print("Sent RC5 ");
      Serial.println(codeValue, HEX);
      irsend.sendRC5(codeValue, codeLen);
    } 
    else {
      irsend.sendRC6(codeValue, codeLen);
      Serial.print("Sent RC6 ");
      Serial.println(codeValue, HEX);
    }
  } 
  else if (codeType == UNKNOWN /* i.e. raw */) {
    // Assume 38 KHz
    irsend.sendRaw(rawCodes, codeLen, 38);
    Serial.println("Sent raw");
  }
}
