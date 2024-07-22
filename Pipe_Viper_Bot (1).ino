//Program for the bot
//Written for Pipe Viper, a pipe inspection robot for AMKA Services.
//Written by Tyler Widener, tylerwidenerlm@gmail.com, assisted by ChatGPT.

//Libraries
#include <SPI.h>
#include "printf.h"
#include <RF24.h>
#include <string.h>

//Variables and Initializations
#define CE_PIN 7
#define CSN_PIN 8

const int aMotorPin1 = 6;
const int aMotorPin2 = 9;
const int aenablePin = 2;  // Connect to arduino enable pin for motor A
const int bMotorPin1 = 3;  //
const int bMotorPin2 = 5;
const int bEnablePin = 4;  // Connect to the arduino enable pin for motor B

RF24 radio(CE_PIN, CSN_PIN);  // CE, CSN
const int buttonPin = 6;
uint8_t addresses[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };
int radioNumber = 1; //NOTE: Change this variable to reflect the anchor number. 0 is home, and the end receiver cannot be greater than 6.
bool role = false;
int payload = 0; //String length of 20 characters

//Main
void setup() {
  // put your setup code here, to run once:
  pinMode(aMotorPin1, OUTPUT);
  pinMode(aMotorPin2, OUTPUT);
  pinMode(aenablePin, OUTPUT);
  pinMode(bMotorPin1, OUTPUT);
  pinMode(bMotorPin2, OUTPUT);
  pinMode(bEnablePin, OUTPUT);

  digitalWrite(bMotorPin1, LOW);
  digitalWrite(bMotorPin2, LOW);
  digitalWrite(aMotorPin1, LOW);
  digitalWrite(aMotorPin2, LOW);
  digitalWrite(aenablePin, LOW);  // Initialize with motor off
  digitalWrite(bEnablePin, LOW);
  Serial.begin(115200);
  radioSetup("listen", radio, 0, 2, addresses);
  /*
  //while (!Serial) { //While loop waiting for serial comms removed for mobile system

  //}
  if (!radio.begin()) {
    Serial.println(F("Radio Not Responding!"));
    while(1) {}
  }
  Serial.println("Pipe Viper Anchor " + String(radioNumber));
  
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(sizeof("0&0000&0000"));
  radio.setDataRate(RF24_250KBPS); // ADDED setDataRate() for increased range
  radio.openWritingPipe(address[radioNumber]);

  radio.openReadingPipe((radioNumber - 1), address[(radioNumber - 1)]); // Open reading pipe for the radio before and after this one
  radio.openReadingPipe((radioNumber + 1), address[(radioNumber + 1)]);

  /* Old Code for opening all reading pipes
  radio.openReadingPipe(0, address[0]);
  radio.openReadingPipe(1, address[1]);
  radio.openReadingPipe(3, address[3]);
  radio.openReadingPipe(4, address[4]);
  radio.openReadingPipe(5, address[5]);
  */
/*
  pinMode(buttonPin, INPUT); //Add the button as an input pin

  if (role) {
    radio.stopListening();
  }
  else {
    radio.startListening();
  }*/
}

void loop() {
  payload = radioReceive(9, radio);
  if (payload == 0) { //Rest
    digitalWrite(aenablePin, LOW);
    digitalWrite(bEnablePin, LOW);
  }
  else if (payload == 1) { //Forward
    digitalWrite(aenablePin, HIGH);
    digitalWrite(bEnablePin, HIGH);
    analogWrite(aMotorPin1, 0);
    analogWrite(aMotorPin2, 255);
    analogWrite(bMotorPin1, 255);
    analogWrite(bMotorPin2, 0);
  }
  else if (payload == 2) { //Reverse
    digitalWrite(aenablePin, HIGH);
    digitalWrite(bEnablePin, HIGH);
    analogWrite(aMotorPin1, 255);
    analogWrite(aMotorPin2, 0);
    analogWrite(bMotorPin1, 0);
    analogWrite(bMotorPin2, 255);
  }
  else if (payload == 3) { //Right
    digitalWrite(aenablePin, HIGH);
    digitalWrite(bEnablePin, HIGH);
    analogWrite(aMotorPin1, 0);
    analogWrite(aMotorPin2, 125);
    analogWrite(bMotorPin1, 0);
    analogWrite(bMotorPin2, 125);
  }
  else if (payload == 4) { //Left
    digitalWrite(aenablePin, HIGH);
    digitalWrite(bEnablePin, HIGH);
    analogWrite(aMotorPin1, 125);
    analogWrite(aMotorPin2, 0);
    analogWrite(bMotorPin1, 125);
    analogWrite(bMotorPin2, 0);
  }
  payload = 0;
  /*
  if (radioReceive(9, radio) == 9) {
    //Forward full speed
    digitalWrite(aenablePin, HIGH);
    digitalWrite(bEnablePin, HIGH);
    analogWrite(aMotorPin1, 255);
    analogWrite(aMotorPin2, 0);
    analogWrite(bMotorPin1, 255);
    analogWrite(bMotorPin2, 0);

    delay(1000);

    digitalWrite(aenablePin, LOW);
    digitalWrite(bEnablePin, LOW);

    delay(500);

    //Reverse full speed
    digitalWrite(aenablePin, HIGH);
    digitalWrite(bEnablePin, HIGH);
    analogWrite(aMotorPin1, 0);
    analogWrite(aMotorPin2, 255);
    analogWrite(bMotorPin1, 0);
    analogWrite(bMotorPin2, 255);

    delay(1000);

    digitalWrite(aenablePin, LOW);
    digitalWrite(bEnablePin, LOW);

    while(1){}
  }*/
}
  /*
  while(digitalRead(buttonPin) == LOW) { //While loop to wait to transmit until button is pressed
      //wait for button to be pressed
      network.update();
      mesh.update();
  }

  unsigned long start_timer = micros();                // start the timer
  //bool report = radio.write(&payload, sizeof(payload));  // transmit & save the report
  bool report = network.write(header, &payload, sizeof(payload));
  unsigned long end_timer = micros();                  // end the timer

  if (report) {
    Serial.print(F("Transmission successful! "));  // payload was delivered
    Serial.print(F("Time to transmit = "));
    Serial.print(end_timer - start_timer);  // print the timer result
    Serial.print(F(" us. Sent: "));
    Serial.println(payload);  // print payload sent
  } else {
    Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
  }

  uint8_t pipe;
  if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
    radio.read(&payload, bytes);             // fetch payload from FIFO
    Serial.print(F("Received "));
    Serial.print(bytes);  // print the size of the payload
    Serial.print(F(" bytes on pipe "));
    Serial.print(pipe);  // print the pipe number
    Serial.print(F(": "));
    Serial.println(payload);  // print the payload's value

    unsigned long start_timer = micros();                // start the timer
    bool report = radio.write(&payload, sizeof(payload));  // transmit & save the report
    unsigned long end_timer = micros();                  // end the timer

    radio.stopListening(); // Stop listening while transmission is being sent.
    if (report) {
      Serial.print(F("Transmission successful! "));  // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);  // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println(payload);  // print payload sent
    } else {
      Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
    }
    delay(1000);
    radio.startListening(); // Start listening after transmission is sent.
  }

  // to make this example readable in the serial monitor
  //delay(1000);  // slow transmissions down by 1 second*/

// Pipe Viper Library, for testing translating joystick output into and out of a string, and sending it via radio signals.
// Written for Pipe Viper, a pipe inspection robot made for AMKA Services.
// Written by Tyler Widener, tylerwidenerlm@gmail.com

/*
----------DOCUMENTATION----------
  
encryptData()
  Takes the verification number, and the two joystick values as arguments.
  It then formats all of those values into a string, which the function returns.
  Incorrect verification numbers will cause the function to return a 1.

decryptVNum()
  Takes the encrypted string as an argument.
  It then decrypts and returns the verification number as an integer.

decryptUpDown()
  Takes the encrypted string as an argument.
  It then decrypts and returns the joystick's up/down value as an integer.

decryptLeftRight()
  Takes the encrypted string as an argument.
  It then decrypts and returns the joystick's left/right value as an integer.

radioSetup()
  Takes an argument to determine how the radio should be set up, as a string, a second argument for the radio object, a third for the verification number,
       a fourth for the radio number, and a fifth for the array of addresses.
  Should be called during setup().
  If the function does not connect to the radio, it will trap the program in an INFINITE LOOP.
  "transmit" means the radio should send signals, and will initially be set up to send signals and NOT listen for them.
  "listen" means the radio should only listen for signals, and will initially be set up to listen to signals only.
  "switch" means that the radio will not be set up for either mode, and will be reliant upon other functions to switch modes for it.

radioTransmit()
  Takes two arguments, the first is the string to transmit, the second is for the radio object.
  Just transmits the string, and prints a message to the Serial Monitor if the transmission was successful.

radioReceive()
  Takes one argument for the radio object, and a second for the verification number.
  ONLY recieves data IF the verification number in the received message matches the one given in the function's argument. If it doesn't match, the function ends.
  Returns the received data as a string.
  Print the received data and diagnostic info to the Serial Monitor.


----STRING TRANSLATION----

<verificationNum>&<joyUp/Down>&<joyLeft/Right>

<verificationNum> is the verification number to prevent repeated signals, single digit only. Can be 0-9, looping through.

& notifies the code the joystick UpDown number is the next few digits up until the next &.

<joyUp/Down> is the raw number received from the joystick's up and down value.

& notifies the code the joystick LeftRight number is the next few digits up until the end of the string.

<joyLeft/Right> is the raw number received from the joystick's left and right value.


----JOYSTICK OUTPUT KEY----
With wires facing down:
Up/Down at Rest:507
Down is at 0
Up is at 1023
Left/Right at Rest 499
Left is at 0
Right is at 1023


----SETUP REQUIRED----

A radio object built with the RF24 library.

A verification number variable, which will need to be incremented by the user every time a transmission is sent. Just a basic integer works fine.

A radio number which defines the radio's position. 0-5 is available to use. Again, a simple integer works fine.

An addresses variable, which contains the addresses for the different radios. The following line of code is recommended for this:
  uint8_t address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };

A Serial Connection, preferably at 115200 baud.

The following libraries:
  #include <SPI.h> // For SPI comms to the radio and back.
  #include "printf.h" // For print statement formatting.
  #include <RF24.h> // For using the NRF24 Radios.
  #include <string.h> // For using Strings.

*/



//Functions
String encryptData(int vfNum, int stickUpDown, int stickLeftRight) {  // Function that encrypts data
  if (vfNum > 9 || vfNum < 0) {                                       // Check for an invalid verification number, must be between 0-9
    Serial.println("ERROR: Invalid Verification Number!");
    return "1";
  }
  Serial.println("Encrypting Data...");
  String encryptedData = "";                                                                   // Create the string to convert into
  encryptedData = (String(vfNum) + "&" + String(stickUpDown) + "&" + String(stickLeftRight));  // Embed the data into the string
  Serial.println("Encryption Finished!");
  Serial.println("Encrypted Data: " + encryptedData);
  return encryptedData;  // Return the encrypted string
}

int decryptVNum(String msg) {  // Function that decrypts the verification number
  Serial.println("Decrypting Verification Number...");
  return int(msg[0]);  // Return the first character of the string which is the single-digit verification number
}

int decryptUpDown(String msg) {  // Function that decrypts the up/down value
  Serial.println("Decrypting Up/Down Value...");
  String decryptedValue = "";                    // Create the string to convert into
  int pos = 2;                                   // Integer to track the current position the code is at in the string
  char currChar = msg[pos];                      // Char to track the current character the program has selected
  while (currChar != '&') {                      // Loop until the program reaches an & symbol
    decryptedValue = decryptedValue + msg[pos];  // Add the currently selected character to the decryption string
    pos++;                                       // Increment the position
    currChar = msg[pos];                         // Set the new value for the current character for while loop evaluation
  }
  Serial.println("Decryption Finished!");
  Serial.println("Decrypted Up/Down Value: " + decryptedValue);
  return decryptedValue.toInt();  // Return the decrypted value as an integer
}

int decryptLeftRight(String msg) {  // Function that decrypts the left/right value
  Serial.println("Decrypting Left/Right Value...");
  String decryptedValue = "";  // Create the string to convert into
  int pos = 2;                 // Integer to track the current position the code is at in the string
  char currChar = msg[pos];    // Char to track to current character the program has selected
  while (currChar != '&') {    // Loop until the program reaches an & symbol
    pos++;                     // Increment position
    currChar = msg[pos];       // Set new current character
  }
  pos++;                                         // Update the position once, since the while loop stopped on the & symbol
  while (pos < decryptedValue.length()) {        // Loop until the end of the string
    decryptedValue = decryptedValue + msg[pos];  // Add the currently selected character to the decryption string
    pos++;                                       // Increment position
    currChar = msg[pos];                         // Set new current character
  }
  Serial.println("Decryption Finished!");
  Serial.println("Decrypted Left/Right Value: " + decryptedValue);
  return decryptedValue.toInt();  // Return the decrypted value as an integer
}

void radioSetup(String mode, RF24& radio, int vfNum, int radioNum, uint8_t address[][6]) {  // Function for setting up a radio
  Serial.println("Initializing Radio...");

  if (!radio.begin()) {
    Serial.println(F("Radio Not Responding!"));
    while (1) {}
  }
  Serial.println("Pipe Viper Radio " + String(radioNum));

  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(sizeof("0&0000&0000"));
  radio.setDataRate(RF24_250KBPS);  // ADDED setDataRate() for increased range
  radio.openWritingPipe(address[radioNum]);

  if (radioNum == 0) {
    radio.openReadingPipe(1, address[1]);
    radio.openReadingPipe(0, address[0]);
  } else if (radioNum == 5) {
    radio.openReadingPipe(4, address[4]);
  } else {
    radio.openReadingPipe((radioNum - 1), address[(radioNum - 1)]);
    radio.openReadingPipe((radioNum + 1), address[(radioNum + 1)]);
    radio.openReadingPipe(radioNum, address[radioNum]);
  }

  if (mode == "transmit") {
    radio.stopListening();
  } else if (mode == "listen") {
    radio.startListening();
  } else {
  }

  Serial.println("Radio Initialized");
}

void radioTransmit(String payload, RF24 radio) {  // Function for sending a transmission
  radio.stopListening();                          // Stop listening while transmission is being sent.

  unsigned long start_timer = micros();                  // start the timer
  bool report = radio.write(&payload, sizeof(payload));  // transmit & save the report
  unsigned long end_timer = micros();                    // end the timer

  if (report) {
    Serial.print(F("Transmission successful! "));  // payload was delivered
    Serial.print(F("Time to transmit = "));
    Serial.print(end_timer - start_timer);  // print the timer result
    Serial.print(F(" us. Sent: "));
    Serial.println(payload);  // print payload sent
  } else {
    Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
  }
  delay(1000);
}

int radioReceive(int vfNum, RF24 radio) {  // Function for receiving a transmission
  radio.startListening();
  uint8_t pipe;
  int payload = 0;
  if (radio.available(&pipe) && pipe != 7) {              // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
    //radio.read(&payload, bytes);             // fetch payload from FIFO
    radio.read(&payload, sizeof(payload));
    //int i = decryptVNum(payload);
    Serial.print("Verification Number: ");
    //Serial.println(i);
    if (1) {
      Serial.print(F("Received "));
      Serial.print(bytes);  // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);  // print the pipe number
      Serial.print(F(": "));
      Serial.println(payload);  // print the payload's value
      return payload;
    }
  }
}