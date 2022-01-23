/*
    
*/

int gpioR = 3;
int gpioT = 4;
int messageByte = 0;
int baudRateUART = 1;


void outputLow(int pinNo) {
  digitalWrite(pinNo, LOW);
}

void outputHigh(int pinNo) {
  digitalWrite(pinNo, HIGH);
}

void setup() {
  // Set the pins used as input and output
  pinMode(gpioR, INPUT);
  pinMode(gpioT, OUTPUT);
  // Use the serial monitor to receive the message
  Serial.begin(9600);
  outputHigh(gpioT);
}



void displayByte(int mess) {
  int mask = 1;
  // display the LSB first
  for (int i = 0; i < 8; i++) {
    Serial.print(mess&mask);
    Serial.print(' ');
    if ((mess & mask) != 0) {
      // if the bit is 1
      outputHigh(gpioT);
      delay(1000);
      outputLow(gpioT);
    }
    else {
      delay(1000);
    }
    mask *= 2;
  }
}

void sendPacket(int mess) {
  // Transfer the start bit
  int cycle = 1000 / baudRateUART;
  outputLow(gpioT);
  delay(cycle);
  int mask = 1;
  short parity = 0;
  // send the LSB first
  for (int i = 0; i < 8; i++) {
    if ((mess & mask) != 0) {
      parity++;
      Serial.print(1);
      Serial.print(' ');
      // if the bit is 1
      outputHigh(gpioT);
      delay(cycle);
    }
    else {
      Serial.print(0);
      Serial.print(' ');
      outputLow(gpioT);
      delay(cycle);
    }
    mask *= 2;
  }
  // Send the parity bit
  if (parity % 2 == 0) {
    outputLow(gpioT);
    delay(cycle);
  }
  else {
    outputHigh(gpioT);
    delay(cycle);
  }
  // send the stop bits: 2 bits at high
  outputHigh(gpioT);
  delay(2*cycle);
}

void loop() {
  if (Serial.available() > 0) {
    // If message received tranmit it
    messageByte = Serial.read();
    if (messageByte != 10) {
      // ignore the null character
      sendPacket(messageByte);
    }
  }
}
