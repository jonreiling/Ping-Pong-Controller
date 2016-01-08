

#include <Arduino.h>
#include <SPI.h>
#include <avr/sleep.h>

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"
#include "OneButton.h"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
OneButton player1Button(12, false);
OneButton player2Button(11, false);

void setup(void)
{
//  while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);

  /* Initialise the module */

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn\"t find Bluefruit, make sure it\"s in CoMmanD mode & check wiring?"));
  }

  Serial.println( F("OK!") );

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();


  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  ble.setMode(BLUEFRUIT_MODE_DATA);

  player1Button.attachClick(player1SingleClick);
  player1Button.attachDoubleClick(player1DoubleClick);
  player1Button.attachLongPressStart(player1LongPress);
  player1Button.setClickTicks(300);
  
  player2Button.attachClick(player2SingleClick);
  player2Button.attachDoubleClick(player2DoubleClick);
  player2Button.attachLongPressStart(player2LongPress);
  player2Button.setClickTicks(300);

}

void loop(void)
{
  player1Button.tick();
  player2Button.tick();
}

void player1SingleClick() {
 // sleep();
  sendCommand("{\"e\":\"single\",\"p\":1}");
}

void player1DoubleClick() {
  sendCommand("{\"e\":\"double\",\"p\":1}");
}

void player1LongPress() {
  sendCommand("{\"e\":\"long\",\"p\":1}");
}

void player2SingleClick() {
  sendCommand("{\"e\":\"single\",\"p\":2}");
}

void player2DoubleClick() {
  sendCommand("{\"e\":\"double\",\"p\":2}");
}

void player2LongPress() {
  sendCommand("{\"e\":\"long\",\"p\":2}");
}

void sendCommand(String command) {
  Serial.println(command);
  ble.print(command); 
}

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void wakeUp(){
//  wasJustWokenUp = true;
} //Not doing anything with this for now...


void sleep() {
  
  delay( 1000 ); //Just give everything a chance to finish up. May not be necessary.
  Serial.println("Sleep");

  //Set up sleep mode.
  set_sleep_mode( SLEEP_MODE_PWR_DOWN );
  sleep_enable();
  
  attachInterrupt( 0 , wakeUp , LOW);
  attachInterrupt( 1 , wakeUp , LOW);
    
  //Actually sleep.
  sleep_mode();
  
  //zzzzz
  
  //Code resumes here on interrupt.
  
  sleep_disable();
  detachInterrupt( 0 );
  detachInterrupt( 1 );
  Serial.println("Done sleeping");

  //Remove our timeout.
 // timeoutEvent = -1;

}


