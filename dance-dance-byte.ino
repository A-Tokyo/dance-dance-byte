#include <TimerOne.h>

//wave shield pins
#define CLK  8
#define DAT  9

//ultrasonic pins
#define trigPin 23
#define echoPin 22

//leds pins
#define led1 53
#define led2 52
#define led3 51
#define led4 50
#define led5 49
#define led6 48
#define led7 47
#define led8 46
#define led9 45  //game status

//buttons pins
#define btn1 44
#define bnt2 43
#define btn3 42
#define btn4 41
#define btn5 40
#define btn6 39
#define btn7 38
#define btn8 37

//LCD pins
/*The circuit:
  LCD RS pin to digital pin 27
  LCD Enable pin to digital pin 26
  LCD D4 pin to digital pin 22
  LCD D5 pin to digital pin 23
  LCD D6 pin to digital pin 24
  LCD D7 pin to digital pin 25\
  LCD R / W pin to ground
  LCD VSS pin to ground
  LCD VCC pin to 5V
  10K resistor:
  ends to + 5V and ground
  wiper to LCD VO pin (pin 3)
*/
#include <LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(27, 26, 22, 23, 24, 25);

int game_started = false;
volatile int game_duration = 240; //4 min game
bool player_detected = true;
bool processing_mode = false;  //Danger !! if true it will write to EEPROM, and we have 100,000 W/E cycles only
int address = 0;
int score = 0;

void setup() {
  //wave shield settings
  pinMode(CLK, OUTPUT);
  pinMode(DAT, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  send(0xfff0);//set voice volumn to 0
  delay(300);

  //ultrasonic settings
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //LCD settings
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Dancing Floor!");
  delay(1000);

  if (processing_mode) {
    lcd.clear();
    lcd.print("Processing");
  }
  //LEDs settings
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);

   //btns settings
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(btn5, INPUT_PULLUP);
  pinMode(btn6, INPUT_PULLUP);
  pinMode(btn7, INPUT_PULLUP);
  pinMode(btn8, INPUT_PULLUP);

  //initializing the timer to setup the game duration
  Timer1.initialize(1000000);         // initialize timer1, and set 1 second period

  Serial.begin(9600);
}

void loop() {

  if (processing_mode) {

    while (Serial.available() > 0)
    {
      delay(10);
      char header = Serial.read();

      if (header == 'a') {
        delay(10);
        int beat = Serial.read();

        //write the beat to EEPROM
        EEPROM.write(address, beat);
        address++;

        if (beat > 0) {
          digitalWrite(led9, HIGH);
          delay(10);
          digitalWrite(led9, LOW);
        }

      }
      else if (header == 'b') {
        lcd.clear();
        lcd.print("Finshed Processing");
        delay(3000);
        processing_mode = false;
        break;
      }
    }

  }
  else if (!player_detected) {

    long duration, distance;
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1;
    if (distance > 40 && distance < 60) {
      lcd.clear();
      lcd.print("want to play?");
      delay(1000);
      player_detected = true;
    }
    delay(500);

  } else if (!game_started) {

    send(0000); //file name
    delay(300);
    send(0xfff6);//set voice volumn to 4
    delay(300);
    game_started = true;
    lcd.clear();
    lcd.print("!!Have Fun!!");
    delay(1000);

    game_duration = 240;
    address = 0;
    score = 0;

    lcd.clear();
    lcd.print("TIME: ");
    lcd.print(game_duration);
    lcd.setCursor(0, 1); //(col,row)
    lcd.print("SCORE: ");
    lcd.print(score);

    Timer1.attachInterrupt(duration);

    digitalWrite(led9, HIGH);

  }
  else { //game logic

    if (game_duration > 0) {
      turn_on();
      button_logic();
      delay(2000); // on for about 2 sec
      turn_off();// off for about 3 sec
      delay(2000);
      address += 4; //to sync the leds with the song
    }
    else {
      end_game();
      Timer1.detachInterrupt(); //timer is off
      lcd.clear();
      lcd.print("Time's Up!");
      delay(2000);
      turn_off();

      lcd.clear();
      lcd.print("Final score is: ");
      lcd.print(score);
      delay(6000);

      player_detected = false;
      game_started = false;
    }
  }
}
void end_game() {
  digitalWrite(led9, LOW);

  digitalWrite(led1, HIGH);
  delay(100);
  digitalWrite(led2, HIGH);
  delay(100);
  digitalWrite(led3, HIGH);
  delay(100);
  digitalWrite(led4, HIGH);
  delay(100);
  digitalWrite(led5, HIGH);
  delay(100);
  digitalWrite(led6, HIGH);
  delay(100);
  digitalWrite(led7, HIGH);
  delay(100);
  digitalWrite(led8, HIGH);
  delay(100);

}
void button_logic(){
  // if the button is pressed while the LED is on, increment score with 10
  if(digitalRead(btn1) && digitalRead(pin1)){
    score+=10;
  }
  if(digitalRead(btn2) && digitalRead(pin2)){
    score+=10;
  }
  if(digitalRead(btn3) && digitalRead(pin3)){
    score+=10;
  }
  if(digitalRead(btn4) && digitalRead(pin4)){
    score+=10;
  }
  if(digitalRead(btn5) && digitalRead(pin5)){
    score+=10;
  }
  if(digitalRead(btn6) && digitalRead(pin6)){
    score+=10;
  }
  if(digitalRead(btn7) && digitalRead(pin7)){
    score+=10;
  }
  if(digitalRead(btn8) && digitalRead(pin8)){
    score+=10;
  }
}
void turn_on() {
  int beat = EEPROM.read(address);

  //turn on some leds
  if (beat & (1 << 0)) {
    digitalWrite(led1, HIGH);
  }
  if (beat & (1 << 1)) {
    digitalWrite(led2, HIGH);
  }
  if (beat & (1 << 2)) {
    digitalWrite(led3, HIGH);
  }
  if (beat & (1 << 3)) {
    digitalWrite(led4, HIGH);
  }
  if (beat & (1 << 4)) {
    digitalWrite(led5, HIGH);
  }
  if (beat & (1 << 5)) {
    digitalWrite(led6, HIGH);
  }
  if (beat & (1 << 6)) {
    digitalWrite(led7, HIGH);
  }
  if (beat & (1 << 7)) {
    digitalWrite(led8, HIGH);
  }
}
void turn_off() {

  //turn off all leds
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led7, LOW);
  digitalWrite(led8, LOW);

}
void duration() {
  if (game_duration >= 0) {
    lcd.clear();
    lcd.print("TIME: ");
    lcd.print(game_duration);
    lcd.setCursor(0, 1); //(col,row)
    lcd.print("SCORE: ");
    lcd.print(score);
  }
  game_duration--;
}
void send(int data)
{
  digitalWrite(CLK, LOW);
  delay(2);
  for (int i = 15; i >= 0; i--)
  {
    delayMicroseconds(50);
    if ((data >> i) & 0x0001 > 0)
    {
      digitalWrite(DAT, HIGH);
      //Serial.print(1);
    }
    else
    {
      digitalWrite(DAT, LOW);
      // Serial.print(0);
    }
    delayMicroseconds(50);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(50);

    if (i > 0)
      digitalWrite(DAT, LOW);
    else
      digitalWrite(DAT, HIGH);
    delayMicroseconds(50);

    if (i > 0)
      digitalWrite(CLK, LOW);
    else
      digitalWrite(CLK, HIGH);
  }
  delay(20);
}
