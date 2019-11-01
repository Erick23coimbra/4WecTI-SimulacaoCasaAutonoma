#include <Servo.h>

/**
   The MIT License (MIT)

   Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
   Copyright (c) 2018 by Fabrice Weinberg

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal//
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

   ThingPulse invests considerable time and money to develop these open source libraries.
   Please support us by buying our products (and not the clones) from
   https://thingpulse.com

*/


#include <ShiftRegister74HC595.h>  //INCLUSÃO DE BIBLIOTECA


#include <dht.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
#include "SH1106Brzo.h"
#include "images.h"
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

SH1106Brzo  display(0x3c, D3, D5);
dht DHT;

#define DHT11_PIN D1

#define DEMO_DURATION 3000

#define FIREBASE_HOST "node-mcu-firebase-b65a6.firebaseio.com"
#define FIREBASE_AUTH "dXP7zWrEEIwfl3v3BvfzhNJTZvcPBFX1yTklQBUe"
#define WIFI_SSID "COIMBRA"
#define WIFI_PASSWORD "abcd80298421"

typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

Servo motorzinho1;
Servo motorzinho2;
Servo motorzinho3;
int banheiroChuveiro, banheiroLed, salaLed1, salaLed2, salaLed3, salaLed4, suiteLed, salaTv, garagemPortao, salaPorta, suiteJanela;


ShiftRegister74HC595 sr (2, 2, 16, 4); //OBJETO DO TIPO "ShiftRegister74HC595" sr (QUANTIDADE DE SHIFT REGISTER, PINO DATA, PINO CLOCK, PINO LATCH)



void setup() {
  motorzinho1.attach(15);
  motorzinho2.attach(13);
  motorzinho3.attach(12);
  Serial.begin(115200); //INICIALIZA A SERIAL

  Serial.begin(9600);
  Serial.print(DHT_LIB_VERSION);

  Serial.begin(115200);
  Serial.println();
  Serial.println();


  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("conectado: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);



}

void drawFontFaceDemo() {
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //display.setFont(ArialMT_Plain_10);
  //display.drawString(0, 0, "Grupo da Mirian");
  display.setFont(ArialMT_Plain_16);
  //display.drawString(0, 10, "Grupo da ");
  //display.setFont(ArialMT_Plain_24);
  display.drawString(0, 26, "Galo vingador");
}

void temperaturacasa() {

  int chk = DHT.read11(DHT11_PIN);
  String temperatura = String (Firebase.getInt("banheiroTemperatura"));
  Serial.println(temperatura + " ºC");

  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //display.setFont(ArialMT_Plain_10);
  //display.drawString(0, 0, "Grupo da Mirian");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, temperatura + "°C");
  //display.setFont(ArialMT_Plain_24);
  //display.drawString(0, 26, "Grupo da Mirian");
}


void drawTextFlowDemo() {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawStringMaxWidth(0, 0, 128,
                             "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
}

void drawTextAlignmentDemo() {
  // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}


void drawRectDemo() {
  // Draw a pixel at given position
  for (int i = 0; i < 10; i++) {
    display.setPixel(i, i);
    display.setPixel(10 - i, i);
  }
  display.drawRect(12, 12, 20, 20);

  // Fill the rectangle
  display.fillRect(14, 14, 17, 17);

  // Draw a line horizontally
  display.drawHorizontalLine(0, 40, 20);

  // Draw a line horizontally
  display.drawVerticalLine(40, 0, 20);
}

void drawCircleDemo() {
  for (int i = 1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(32, 32, i * 3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(96, 32, 32 - i * 3);
  }
}

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

void drawImageDemo() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}
Demo demos1[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo};

//Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
int demoLength1 = (sizeof(demos1) / sizeof(Demo));
long timeSinceLastModeSwitch1 = 0;

//Demo demos2[] = {drawImageDemo};
Demo demos2[] = {temperaturacasa};

//Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
int demoLength2 = (sizeof(demos2) / sizeof(Demo));
long timeSinceLastModeSwitch2 = 0;

Demo demos3[] = {drawProgressBarDemo};

//Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
int demoLength3 = (sizeof(demos3) / sizeof(Demo));
long timeSinceLastModeSwitch3 = 0;


void loop() {

  salaLed1 = Firebase.getInt("salaLed1");
  salaLed2 = Firebase.getInt("salaLed2");
  salaLed3 = Firebase.getInt("salaLed3");
  salaLed4 = Firebase.getInt("salaLed4");
  banheiroChuveiro = Firebase.getInt("banheiroChuveiro");
  banheiroLed = Firebase.getInt("banheiroLed");
  suiteLed = Firebase.getInt("suiteLed");
  salaTv = Firebase.getInt("salaTv");
  garagemPortao = Firebase.getInt("garagemPortao");
  salaPorta = Firebase.getInt("salaPorta");
  suiteJanela = Firebase.getInt("suiteJanela");

  Serial.print("salaLed1: ");
  Serial.println(Firebase.getInt("salaLed1"));
  Serial.print("salaLed2: ");
  Serial.println(Firebase.getInt("salaLed2"));
  Serial.print("salaLed3: ");
  Serial.println(Firebase.getInt("salaLed3"));
  Serial.print("salaLed4: ");
  Serial.println(Firebase.getInt("salaLed4"));
  Serial.print("banheiroChuveiro: ");
  Serial.println(Firebase.getInt("banheiroChuveiro"));
  Serial.print("banheiroLed: ");
  Serial.println(Firebase.getInt("banheiroLed"));
  Serial.print("suiteLed: ");
  Serial.println(Firebase.getInt("suiteLed"));
  Serial.print("salaTv: ");
  Serial.println(Firebase.getInt("salaTv"));

  Serial.print("garagemPortao: ");
  Serial.println(Firebase.getInt("garagemPortao"));

  Serial.print("salaPorta: ");
  Serial.println(Firebase.getInt("salaPorta"));

  Serial.print("suiteJanela: ");
  Serial.println(Firebase.getInt("suiteJanela"));
  if (garagemPortao == 1) {
    //   motorzinho1.write(0);
    //  delay(1000);
    //  motorzinho1.write(30);
    //  delay(1000);
    //  motorzinho1.write(60);
    //  delay(1000);
    motorzinho1.write(120);
    delay(1000);
  }
  else {
    if (garagemPortao == 0) {
      // motorzinho1.write(90);
      //  delay(1000);
      //  motorzinho1.write(60);
      //  delay(1000);
      //  motorzinho1.write(30);
      //  delay(1000);
      motorzinho1.write(0);
      delay(1000);
    }
  }

  if (salaPorta == 1) {
    //    motorzinho2.write(0);
    //  delay(1000);
    //  motorzinho2.write(30);
    //  delay(1000);
    //  motorzinho2.write(60);
    //  delay(1000);
    motorzinho2.write(0);
    delay(1000);
  }
  else {
    if (salaPorta == 0) {
      //  motorzinho2.write(90);
      //  delay(1000);
      //  motorzinho2.write(60);
      //  delay(1000);
      //  motorzinho2.write(30);
      //  delay(1000);
      motorzinho2.write(90);
      delay(1000);
    }
  }

  if (suiteJanela == 1) {
    //    motorzinho3.write(0);
    //  delay(1000);
    //  motorzinho3.write(30);
    //  delay(1000);
    //  motorzinho3.write(60);
    //  delay(1000);
    motorzinho3.write(120);
    delay(1000);
  }
  else {
    if (suiteJanela == 0) {

      //              motorzinho3.write(90);
      //              delay(1000);
      //              motorzinho3.write(60);
      //              delay(1000);
      //              motorzinho3.write(30);
      //              delay(1000);
      motorzinho3.write(0);
      delay(1000);
    }
  }



  if (salaLed1 == 1) {
    sr.set(1, HIGH);
  }
  else if (salaLed1 == 0) {
    sr.set(1, LOW);
  }

  if (salaLed2 == 1) {
    sr.set(2, HIGH);
  }
  else if (salaLed2 == 0) {
    sr.set(2, LOW);
  }

  if (salaLed3 == 1) {
    sr.set(3, HIGH);
  }
  else if (salaLed3 == 0) {
    sr.set(3, LOW);
  }

  if (salaLed4 == 1) {
    sr.set(4, HIGH);
  }
  else if (salaLed4 == 0) {
    sr.set(4, LOW);
  }

  if (suiteLed == 1) {
    sr.set(5, HIGH);
  }
  else if (suiteLed == 0) {
    sr.set(5, LOW);
  }

  if (banheiroLed == 1) {
    sr.set(6, HIGH);
  }
  else if (banheiroLed == 0) {
    sr.set(6, LOW);
  }

  if (banheiroChuveiro == 1) {

    sr.set(8, HIGH);
    delay (2000);

    sr.set(8, LOW);
    sr.set(7, HIGH);
  }
  else {
    if (banheiroChuveiro == 2) {
      sr.set(7, LOW);
      sr.set(8, HIGH);
    } else {
      if (banheiroChuveiro == 0) {
        sr.set(7, LOW);
        sr.set(8, LOW);
      }

    }

  }



  //   motorzinho2.write(0);
  //  delay(1000);
  //  motorzinho2.write(90);
  //  delay(1000);
  //
  //   motorzinho3.write(0);
  //  delay(1000);
  //  motorzinho3.write(90);
  //  delay(1000);
  int chk = DHT.read11(DHT11_PIN);
  if (chk == DHTLIB_OK)
  {
    Serial.print("umidade: ");
    Serial.print(DHT.humidity);
    Serial.print("    Temperatura: ");
    Serial.println(DHT.temperature);
    Firebase.setInt("banheiroTemperatura", DHT.temperature);
  }

  if (salaTv == 0) {
    // clear the display
    display.clear();
  }
  if (salaTv == 1) {
    // clear the display
    display.clear();
    // draw the current demo method
    demos1[demoMode]();

    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(10, 128, String(millis()));
    // write the buffer to the display
    display.display();

    if (millis() - timeSinceLastModeSwitch1 > DEMO_DURATION) {
      demoMode = (demoMode + 1)  % demoLength1;
      timeSinceLastModeSwitch1 = millis();
    }
    counter++;
    delay(10);
  } else {

    if (salaTv == 2) {
      // clear the display
      display.clear();
      // draw the current demo method
      demos2[demoMode]();

      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      display.drawString(10, 128, String(millis()));
      // write the buffer to the display
      display.display();

      if (millis() - timeSinceLastModeSwitch2 > DEMO_DURATION) {
        demoMode = (demoMode + 1)  % demoLength2;
        timeSinceLastModeSwitch2 = millis();
      }
      counter++;
      delay(10);
    }

    else {


      if (salaTv == 3) {
        // clear the display
        display.clear();
        // draw the current demo method
        demos3[demoMode]();

        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(10, 128, String(millis()));
        // write the buffer to the display
        display.display();

        if (millis() - timeSinceLastModeSwitch3 > DEMO_DURATION) {
          demoMode = (demoMode + 1)  % demoLength3;
          timeSinceLastModeSwitch3 = millis();
        }
        counter++;
        delay(10);
      }
    }
  }
}
