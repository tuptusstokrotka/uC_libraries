#ifndef FASTBUTTONS_H
#define FASTBUTTONS_H
#pragma once

#include <avr/io.h>

/**
 * @brief this library will let you create buttons as an objects.
 * Button has 4 states - RELEASED, PRESSED, HOLDING and UNKNOWN
 *
 * Define Button using port letter and port bit number
 * holding time is by default 400ms
 *
 * @note UKNONWN is only used in NO REPEAT condition
 *
 */
#define PULLUP

#ifdef PULLUP
  #define PRESSED  0              // PRESSED STATE IS HIGH
  #define RELEASED 1              // RELEASED STATE IS LOW
#else
  #define PRESSED  1              // PRESSED STATE IS HIGH
  #define RELEASED 0              // RELEASED STATE IS LOW
#endif

#define HOLD_MS_DEFAULT 400
/**
 * CONFIGURATION FOR THE DEFAULT GetStateRepeat()
 * REPEAT 0 - means that the state will be registered ONCE
 * REPEAT 1 - will return state as long as there is no new state
 *
 * NO REPEAT:
 * // ---------------------------------------------------
 *               PRESS               /\ HOLD
 *              /\                  /  \
 * __RELEASE___/  \                /    \
 *                 \__MIDSTATE____/      \___MIDSTATE____
 *
 *
 * REPEAT:
 * // ---------------------------------------------------
 *                                    _______MIDSTATE________
 *         PRESS _____MIDSTATE_______/ HOLD
 *              /
 * __RELEASE___/
 *
 * @note if you decided to choose none to repeat, use default GetState() instead
 * Creating 2 functions was on purpose to let user use 2 methods in 1 projects
 */

#define PRESS_REPEAT 1
#define HOLD_REPEAT 1

enum State{
  _unknown    = -1,
  _release    = 0,
  _press      = 1,
  _hold       = 2
};

class FastButtons{
private:
  volatile uint8_t* ddr;          // Data Direction Register
  volatile uint8_t* port;         // Data Direction Register
  volatile uint8_t* pinReg;       // Input Pin Register
  uint8_t pin;                    // Pin Bit

  unsigned long hold_ms;          // Time required to trigger HOLD

  unsigned long press_time = 0;   // Press time in ms

  State state = _release;         // Last Switch state

public:
    FastButtons(uint8_t portLetter, uint8_t pin, int hold = HOLD_MS_DEFAULT);
    ~FastButtons();

    State GetState();
    State GetStateRepeat();
    unsigned long GetHoldingTime();
};

#endif