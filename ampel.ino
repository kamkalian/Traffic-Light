

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdint.h>

volatile bool nextGreen = false;
volatile bool green = false;
volatile bool nextRed = false;
volatile bool red = false;
volatile bool counterGo = false;
volatile uint8_t counter = 0;


int main(void) {

  cli();

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PB2, OUTPUT);
  pinMode(PB3, OUTPUT);
  pinMode(PB4, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(PB0, INPUT);

  ledRedOff();
  ledYellowOff();
  ledGreenOff();

  ledRedOn();
  red = true;

  wdt_reset(); // Reset watchdog
  WDTCR = _BV(WDTIE) | _BV(WDP2) | _BV(WDP0); // Turn on WDT interrupt and cause an interrupt every 500 ms
  sei();

  while (1) {

    if (nextGreen) {
      
      if(!counterGo)counter = 0;
      counterGo = true;
      ledYellowOn();
      
      if (counter >= 2) {
        
        counterGo = false;
        ledYellowOff();
        ledRedOff();
        ledGreenOn();
        green = true;
        red = false;
        nextGreen = false;
                
      }
      
    }

    if (nextRed) {

      if(!counterGo)counter = 0;
      counterGo = true;
      ledYellowOn();
      ledGreenOff();
      
      if (counter >= 4) {
        
        counterGo = false;
        ledYellowOff();
        ledRedOn();
        red = true;
        green = false;
        nextRed = false;
        
      }
    }

  }
}



void ledRedOn() {

  digitalWrite(PB3, HIGH);

}

void ledYellowOn() {

  digitalWrite(PB4, HIGH);

}

void ledGreenOn() {

  digitalWrite(PB2, HIGH);

}

void ledRedOff() {

  digitalWrite(PB3, LOW);

}

void ledYellowOff() {

  digitalWrite(PB4, LOW);

}

void ledGreenOff() {

  digitalWrite(PB2, LOW);

}



ISR(WDT_vect)
{

  if (digitalRead(PB0) && red) nextGreen = true;
  if (!digitalRead(PB0) && green) nextRed = true;

  if (counterGo)counter++;
}



