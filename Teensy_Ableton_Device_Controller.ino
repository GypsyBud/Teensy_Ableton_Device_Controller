#include <Bounce.h>

#include <RotaryEncoder.h> // See http://www.mathertel.de/Arduino/RotaryEncoderLibrary.aspx

// the MIDI channel number to send messages
// Ableton wants sliders and buttons on seperate channels
const int buttonchannel = 1;
const int sliderchannel = 2;

// These are the cc "control" numbers that will receive the midi control change data from this controller
int buttonCCs [8] = {1, 2, 3, 4, 5, 6, 7, 8};
int sliderCCs [8] = {1, 2, 3, 4, 5, 6, 7, 8};

// Create Bounce objects for each button.
// Adjust bounce time if your buttons are jittery

int bounceTime = 50;

Bounce button1 = Bounce(16, bounceTime);
Bounce button2 = Bounce(17, bounceTime);
Bounce button3 = Bounce(18, bounceTime);
Bounce button4 = Bounce(19, bounceTime);
Bounce button5 = Bounce(20, bounceTime);
Bounce button6 = Bounce(21, bounceTime);
Bounce button7 = Bounce(22, bounceTime);
Bounce button8 = Bounce(23, bounceTime);

// Declare and setup encoders
RotaryEncoder deviceEncoder1(0, 1);
RotaryEncoder deviceEncoder2(2, 3);
RotaryEncoder deviceEncoder3(4, 5);
RotaryEncoder deviceEncoder4(6, 7);
RotaryEncoder deviceEncoder5(8, 9);
RotaryEncoder deviceEncoder6(10, 11);
RotaryEncoder deviceEncoder7(12, 13);
RotaryEncoder deviceEncoder8(14, 15);

// Reverse option for some encoders
bool reverse = true;

int reverseOrNot(bool reverse, int initialValue) {
  if (reverse) {
    // Reverse midi data
    return 127 - initialValue;

  } else {
    // Do not reverse midi data
    return initialValue;
  }
};

////////////////////////
// ENCODER READING AND MIDI CONVERSION
////////////////////////

int value;

// This will read an encoder and send it's data off as midi data
int encoderToMidi (RotaryEncoder& encoder, int& oldPosition, int control) {

  encoder.tick();

  // Get encoders current position and reverse the data if the reverse variable is set to true
  value = reverseOrNot(reverse, encoder.getPosition());

  // If the encoder has clicked beyond 127, reset it
  if (value > 127) {
    encoder.setPosition(127);
    
    value = 127;

  }

  // If it is below 0, reset it to 0
  else if (value < 0) {
    
    encoder.setPosition(0);
    
    value = 0;
  
  }

  // Constrain to midi values
  //value = constrain(value, 0, 127);

  if (value != oldPosition) {
    oldPosition = value;

    //Serial.print(value);

      usbMIDI.sendControlChange(control, value, sliderchannel);
  }

  return (oldPosition);
};


void setup() {

  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);  

  Serial.begin(9600);
}

int oldPosition1  = -999; // Set initial position of encoder1
int oldPosition2  = -999; // Set initial position of encoder2
int oldPosition3  = -999; // Set initial position of encoder3
int oldPosition4  = -999; // Set initial position of encoder4
int oldPosition5  = -999; // Set initial position of encoder5
int oldPosition6  = -999; // Set initial position of encoder6
int oldPosition7  = -999; // Set initial position of encoder7
int oldPosition8  = -999; // Set initial position of encoder8


void loop() {
  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();
  button6.update();
  button7.update();
  button8.update();

  
  // Check each button for "falling" edge.
  // Send a MIDI Note On message when each button is pressed
  if (button1.fallingEdge()) {
    usbMIDI.sendControlChange(buttonCCs[0], 127, buttonchannel);  // control, value, channel
  }
  if (button2.fallingEdge()) {
    usbMIDI.sendControlChange(buttonCCs[1], 127, buttonchannel);
  }
  if (button3.fallingEdge()) {
    usbMIDI.sendControlChange(buttonCCs[2], 127, buttonchannel);
  }
  if (button4.fallingEdge()) {
    usbMIDI.sendControlChange(buttonCCs[3], 127, buttonchannel);
  }
  if (button5.fallingEdge()) {
    usbMIDI.sendControlChange(buttonCCs[4], 127, buttonchannel);
  }
  if (button6.fallingEdge()) {
    usbMIDI.sendControlChange(buttonCCs[5], 127, buttonchannel);
  }
  if (button7.fallingEdge()) {
    usbMIDI.sendControlChange(buttonCCs[6], 127, buttonchannel);
  }
  if (button8.fallingEdge()) {
    usbMIDI.sendControlChange(buttonCCs[7], 127, buttonchannel);
  }
  

  // Read encoders and convert the data to midi. And return current position to the oldPosition variable
  oldPosition1 = encoderToMidi(deviceEncoder1, oldPosition1, sliderCCs[0]);
  oldPosition2 = encoderToMidi(deviceEncoder2, oldPosition2, sliderCCs[1]);
  oldPosition3 = encoderToMidi(deviceEncoder3, oldPosition3, sliderCCs[2]);
  oldPosition4 = encoderToMidi(deviceEncoder4, oldPosition4, sliderCCs[3]);
  oldPosition5 = encoderToMidi(deviceEncoder5, oldPosition5, sliderCCs[4]);
  oldPosition6 = encoderToMidi(deviceEncoder6, oldPosition6, sliderCCs[5]);
  oldPosition7 = encoderToMidi(deviceEncoder7, oldPosition7, sliderCCs[6]);
  oldPosition8 = encoderToMidi(deviceEncoder8, oldPosition8, sliderCCs[7]);

  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}

