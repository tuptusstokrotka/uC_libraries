#include "FastButtons.h"
#include <Arduino.h>

FastButtons::FastButtons(uint8_t portLetter, uint8_t pin, int hold){
    pinMode(pin , INPUT_PULLUP);
    this->pin = pin;
    this->hold_ms = hold;

    switch (portLetter) {
        case 'B':
            this->ddr     = &DDRB;
            this->port    = &PORTB;
            this->pinReg  = &PINB;
            break;
        case 'C':
            this->ddr     = &DDRC;
            this->port    = &PORTC;
            this->pinReg  = &PINC;
            break;
        case 'D':
            this->ddr     = &DDRD;
            this->port    = &PORTD;
            this->pinReg  = &PIND;
            break;
        default:
            // Handle error: invalid port letter
            break;
    }

    *this->ddr &= ~(1 << this->pin);                              // Clear the bit to set as input

    #ifdef PULLUP
        *this->port |= (1 << this->pin);                          // Enable pull-up resistor (optional)
    #endif
}

FastButtons::~FastButtons(){

}

State FastButtons::GetState(){
    /* BUTTON RELEASED */
    if((*this->pinReg & (1 << this->pin)) ? 1 : 0 == RELEASED){   // BUTTON RELEASED
        state = _release;
        return _release;
    }

    /* BUTTON PRESSED */
    if (state == _release) {                                      // Pressed but was released
        press_time = millis();
        state = _press;                                           // JUST A PRESS :C
        return _press;
    }
    else if ((millis() - press_time) > hold_ms && state != _hold) {
      state = _hold;                                              // UPDATE STATE
      return _hold;                                               // RETURN HOLD
    }

    return _unknown;                                              // STATE REGISTERED ALREADY
}

State FastButtons::GetStateRepeat(){
    /* BUTTON RELEASED */
    if((*this->pinReg & (1 << this->pin)) ? 1 : 0 == RELEASED){   // BUTTON RELEASED
        state = _release;
        return _release;
    }

    /* BUTTON PRESSED */
    if (state == _release) {                                      // Pressed but was released
        press_time = millis();
        state = _press;                                           // JUST A PRESS :C
        return _press;
    }
    else if ((millis() - press_time) > hold_ms) {
      #if HOLD_REPEAT
        return _hold;                                             // REPEAT LAST STATE
      #else
        if(state != _hold){
          state = _hold;                                          // UPDATE STATE
          return state;                                           // RETURN HOLD
        }
        else
          return _unknown;                                        // STATE REGISTERED ALREADY
      #endif
    }

    #if PRESS_REPEAT
      return state;                                               // REPEAT LAST STATE
    #else
      return _unknown;                                            // STATE REGISTERED ALREADY
    #endif
}

unsigned long FastButtons::GetHoldingTime(){
    //TODO: Replace millis() with the function specified for the uC
    return millis()-press_time;
}