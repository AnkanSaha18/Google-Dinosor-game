#include <Arduino.h>
#include <QTRSensors.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal.h>

/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 7                      
 * LCD Enable pin to digital pin 6                 
 * LCD D4 pin to digital pin 5                 
 * LCD D5 pin to digital pin 4                 
 * LCD D6 pin to digital pin 3           
 * LCD D7 pin to digital pin 2           
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VDD pin to 5V
 * 10K resistor:   (black wire -> +5v  and while wire-> GND)
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * A(anode)->+5v and k(cathod)->GND
 * 
 * switch configeration 
 *     +5----registance----|------switch------gnd    
 *                         |
 *                         |
 *                       input

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int input_pin1 = A10;

byte cus0[8] =
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
};

byte cus1[8] =
    {
        B00000,
        B00110,
        B00110,
        B01101,
        B10100,
        B00110,
        B01010,
        B10001,
};

byte cus2[8] =
    {
        B00000,
        B00110,
        B01110,
        B10101,
        B10100,
        B00110,
        B00101,
        B11001,
};
byte cus3[8] =
    {
        B11111,
        B11111,
        B01110,
        B01110,
        B00100,
        B00100,
        B00000,
        B00000,
};

byte cus4[8] =
    {
        B00000,
        B00000,
        B00100,
        B00100,
        B01110,
        B01110,
        B11111,
        B11111,
};

const int mapping_size = 150;
int arr[mapping_size];
int level = 0;
int speed = 50;
int lap = 0;
long long score = 0;

int c1 = 0;
int c2 = 0;

void check_environment();
void create_game_environment()
{
  for (int i = 0; i < 16; i++)
  {
    arr[i] = 0;
  }

  for (int i = 16; i < mapping_size; i++)
  {
    int r = rand() % 3;

    if (r == 1)
    {
      int r1 = rand() % 7;
      while (r1 < 1)
      {
        r1 = rand() % 7;
      }

      for (int j = 0; j < r1; j++, i++)
      {
        arr[i] = r;
        Serial.println(1);
      }
    }
    else if (r == 2)
    {
      int r2 = rand() % 7;
      while (r2 < 1)
      {
        r2 = rand() % 7;
      }

      for (int j = 0; j < r2; j++, i++)
      {
        arr[i] = r;
        Serial.println(2);
      }
    }
    else
    {
      int r0 = rand() % 3;
      for (int j = 0; j < r0; j++, i++)
      {
        arr[i] = r;
        Serial.println(0);
      }
    }

    for (int j = 0; j < 3; j++)
    {
      arr[i] = 0;
      i++;
      Serial.println(0);
    }
  }
  check_environment();
  return;
}

void check_environment()
{
  for (int i = 16; i < mapping_size - 3; i++)
  {
    if ((arr[i] == 1 && arr[i + 1] == 2) || (arr[i] == 2 && arr[i + 1] == 1))
    {
      while (arr[i + 1] != 0)
      {
        arr[i + 1] = 0;
        i++;
        if (i > mapping_size)
        {
          break;
        }
      }
    }
  }
  return;
}

void display();
void cheaker()
{
  int a = analogRead(input_pin1);
  Serial.println(a);
  delay(20);
  int b = analogRead(input_pin1);
  while (b < 100)
  {
    b = analogRead(input_pin1);
  }

  if (a < 100)
  {
    c1 = 1 - c1;
  }
  display();
}

void display()
{
  if (c1 == 0)
  {
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    delay(20);
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    delay(20);

    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    delay(20);
    lcd.setCursor(0, 0);
    lcd.write(byte(2));
    delay(20);
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    delay(20);
    lcd.setCursor(0, 1);
    lcd.write(byte(1));
    delay(20);

    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    delay(20);
    lcd.setCursor(0, 1);
    lcd.write(byte(2));
    delay(20);
  }
}

void special_delay(int time)
{
  long long entering_time = millis();
  while (millis() - entering_time < time)
  {
    cheaker();
  }
}

void game_over()
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("GAME OVER!");

  lcd.setCursor(2, 1);
  lcd.print("Score : ");
  int a = score;
  lcd.print(a);
  delay(3000);
  level = 0;
  speed = 50;
  lap = 0;
  score = 0;
}

int run_game()
{
  for (int i = 0; i < mapping_size - 16; i++)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.print(i);
    for (int j = 0; j < 16; j++)
    {
      if (arr[i + j] == 1)
      {
        lcd.setCursor(j, 0);
        lcd.write(byte(3));
      }
      if (arr[i + j] == 2)
      {
        lcd.setCursor(j, 1);
        lcd.write(byte(4));
      }
    }
    Serial.println(arr[i]);
    special_delay(350 - speed);
    score++;
    if (c1 == arr[i] - 1)
    {
      game_over();
      return 0;
    }
  }
  /*
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Run Function");
  delay(2000);
  */
  return 1;
}

void level_lap_display()
{
  lcd.clear();
  if (lap % 2 == 0)
  {
    lcd.setCursor(4, 0);
    lcd.print("LEVEL: ");
    lcd.print(lap / 2 + 1);
    speed += 70;

  }
  lcd.setCursor(5, 1);
  lcd.print("Lap: ");
  lcd.print(lap % 2 + 1);
  lap++;
  delay(2000);
}

void setup()
{
  Serial.begin(9600);

  pinMode(input_pin1, INPUT);

  lcd.begin(16, 2);
  lcd.createChar(0, cus0);
  lcd.createChar(1, cus1);
  lcd.createChar(2, cus2);
  lcd.createChar(3, cus3);
  lcd.createChar(4, cus4);

  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("hello, world!");
  delay(1000);
  lcd.clear();
}

void loop()
{
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("New Game");
  delay(2000);

  level_lap_display();

  create_game_environment();

  while (run_game())
  {
    level_lap_display();
    create_game_environment();
  }
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Well Played");
  delay(2000);
}