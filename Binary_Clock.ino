/*
  Binary Real Time Clock with time change capability

   Having done no major Microcontroller projects before except simple LED lighting, this was something 
   that I have wanted to do for a while.  Originally, the project started as a DIY remake of
   a ferrofluid clock that I saw online.  When I realized I did not have the resources possible to 
   make this project, I adapted and came out with something to be proud of.

   The code is based on the use of a 74HC595 shift register to display the minutes in binary for the 
   clock.  Other LEDs are lit by output pins from the board itself and a 555 timer.

   Hardware used:
    - Arduino Nano (https://www.amazon.com/Arduino-Emakefun-board-ATmega328P-Micro-controller/dp/B0775XQXRB/ref=sr_1_5?s=pc&ie=UTF8&qid=1529448465&sr=1-5&keywords=arduino+nano&dpID=51QZfWzFNJL&preST=_SY300_QL70_&dpSrc=srch)
    - 555 Timer 
    - Shift Register
    - Varying resistors, capacitors, and LEDs
    - 2x Four pin Push Buttons

   Hardware Setup:
    - Twin pushButtons were set up as default logic level HIGH with 10 kOhm Pull-up resistors
    - The 74HC595 Shift register was set up and programmed using modified code (https://learn.adafruit.com/adafruit-arduino-lesson-4-eight-leds/arduino-code)
    - The 555 Timer was set up using the design from Instructables.com (http://www.instructables.com/id/555-Timer/)

  All other software and hardware was designed by me for my own purposes.  While I know this program can be
  improved, for the sake of simplicity, I am content with its current iteration.
  As a designer I feel as though the design should be made with the user in mind.  I am the user for this
  project and believe that it is simple, clean, and effective.  Therefore, it is complete.

  Created: 19 June 2018
  by Nicholas James Posey
  --Github will be included when I post it there--
 
 */


//Bytes correlating to a three digit time telling bit representation
//the way that these bytes are written means that the hour will be told by a 'single place' of 4 leds representing the bits
//the minutes will be represented by 'two places' of four bits each
//the two nibbles that make up the minutes will represent two different four bit numbers, so 0b00010011 will represent 13
byte zero =            0b00000000;
byte one =             0b00000001;
byte two =             0b00000010;
byte three =           0b00000011;
byte four =            0b00000100;
byte five =            0b00000101;
byte six =             0b00000110;
byte seven =           0b00000111;
byte eight =           0b00001000;
byte nine =            0b00001001;
byte ten =             0b00001010;
byte eleven =          0b00001011;
byte twelve =          0b00001100;
byte thirteen =        0b00001101;
byte fourteen =        0b00001110;
byte fifteen =         0b00001111;
byte sixteen =         0b00010000;
byte seventeen =       0b00010001;
byte eighteen =        0b00010010;
byte nineteen =        0b00010011;
byte twenty =          0b00010100;
byte twentyOne =       0b00010101;
byte twentyTwo =       0b00010110;
byte twentyThree =     0b00010111;
byte twentyFour =      0b00011000;
byte twentyFive =      0b00011001;
byte twentySix =       0b00011010;
byte twentySeven =     0b00011011;
byte twentyEight =     0b00011100;
byte twentyNine =      0b00011101;
byte thirty =          0b00011110;
byte thirtyOne =       0b00011111;
byte thirtyTwo =       0b00100000;
byte thirtyThree =     0b00100001;
byte thirtyFour =      0b00100010;
byte thirtyFive =      0b00100011;
byte thirtySix =       0b00100100;
byte thirtySeven =     0b00100101;
byte thirtyEight =     0b00100110;
byte thirtyNine =      0b00100111;
byte forty =           0b00101000;
byte fortyOne =        0b00101001;
byte fortyTwo =        0b00101010;
byte fortyThree =      0b00101011;
byte fortyFour =       0b00101100;
byte fortyFive =       0b00101101;
byte fortySix =        0b00101110;
byte fortySeven =      0b00101111;
byte fortyEight =      0b00110000;
byte fortyNine =       0b00110001;
byte fifty =           0b00110010;
byte fiftyOne =        0b00110011;
byte fiftyTwo =        0b00110100;
byte fiftyThree =      0b00110101;
byte fiftyFour =       0b00110110;
byte fiftyFive =       0b00110111;
byte fiftySix =        0b00111000;
byte fiftySeven =      0b00111001;
byte fiftyEight =      0b00111010;
byte fiftyNine =       0b00111011;

//minutes binary representation for use with shift register
byte minutesRegister[] = {zero, one, two, three, four, five, six, seven, eight, nine, ten, eleven, twelve, thirteen, fourteen, fifteen, sixteen, seventeen, eighteen, nineteen,
                    twenty, twentyOne, twentyTwo, twentyThree, twentyFour, twentyFive, twentySix, twentySeven, twentyEight, twentyNine, thirty,
                    thirtyOne, thirtyTwo, thirtyThree, thirtyFour, thirtyFive, thirtySix, thirtySeven, thirtyEight, thirtyNine, forty,
                    fortyOne, fortyTwo, fortyThree, fortyFour, fortyFive, fortySix, fortySeven, fortyEight, fortyNine, fifty,
                    fiftyOne, fiftyTwo, fiftyThree, fiftyFour, fiftyFive, fiftySix, fiftySeven, fiftyEight, fiftyNine};


//Variables for code simplification
int latchPin = 9;
int clockPin = 12;
int dataPin = 11;

//Variables for time keeping
int seconds = 0;
int minutes = 0;
int hours = 12;

//Variables to use to counteracto pushButton rebounce and single touch
int pinState3;
int pinState4;

void hourDisplay(){
  if(hours == 1){
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      }
    else if(hours == 2){
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      }
    else if(hours == 3){
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      }
    else if(hours == 4){
     digitalWrite(5, LOW);
     digitalWrite(6, LOW);
     digitalWrite(7, HIGH);
     digitalWrite(8, LOW);
      }
    else if(hours == 5){
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
     }
    else if(hours == 6){
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
     }
    else if(hours == 7){
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      }
    else if(hours == 8){
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
     }
    else if(hours == 9){
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
     }
    else if(hours == 10){
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
     }
    else if(hours == 11){
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
     }
    else if(hours == 12){
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      }
  else{
    hours = 1;
    digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
       }
  }
  
void minuteDisplay(){
  //display minutes on 74HC595 shift register
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, minutesRegister[minutes]);
   digitalWrite(latchPin, HIGH);
  }
  
void hourIncrease(){
  hours++;
  if(hours == 13)
    hours = 1;
  }

void minuteIncrease(){
  if(digitalRead(4) == LOW)
    hourIncrease();
  else
    minutes++;
    
  if(minutes == 60)
    minutes = 0;

    minuteDisplay();
    hourDisplay();
  }

//changes the LEDs for hours and minutes depending on internal time keeping variables
void changeDisplay(){
   minuteDisplay();
   hourDisplay();
}

//function to adjust time at each tick visually and in code
void updateShiftRegister()
{      
   //update LEDs
   changeDisplay();  
   //update time internally on the board
   seconds++;
   //Set to 62 instead of 60 due to inaccuracies with the 555 timer setup
   if(seconds == 62){
    minutes++;
    seconds = 0;
    if(minutes == 60){
      minutes = 0;
      hours++;
      if(hours == 13){
        hours = 1;
      }
    }
  }
}

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  //PushButton pins with default logic level LOW
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  //pins controlling the hours output
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  //on initial boot
  for(int i = 5; i < 9; i++){
    digitalWrite(i, HIGH);
    delay(1000);
    digitalWrite(i, LOW);
    }
  
  //Interrupt for time keeping
   attachInterrupt(digitalPinToInterrupt(2), updateShiftRegister, RISING);
  //Interrupt for use of time change
   attachInterrupt(digitalPinToInterrupt(3), minuteIncrease, FALLING);
}

void loop() {
//Intentionally left empty as loop is unnecessary due to reliance on interrupts
}
