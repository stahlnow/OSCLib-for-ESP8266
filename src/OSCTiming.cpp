/*
 Written by Adrian Freed, The Center for New Music and Audio Technologies,
 University of California, Berkeley.  Copyright (c) 2013, The Regents of
 the University of California (Regents).
 
 Permission to use, copy, modify, distribute, and distribute modified versions
 of this software and its documentation without fee and without a signed
 licensing agreement, is hereby granted, provided that the above copyright
 notice, this paragraph and the following two paragraphs appear in all copies,
 modifications, and distributions.
 
 IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
 BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
 HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
 MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 
 For bug reports and feature requests please email me at yotam@cnmat.berkeley.edu
 */


#include "OSCTiming.h"
#include "OSCBoards.h"


extern volatile uint32_t systick_millis_count;
extern volatile uint32_t timer0_millis_count;
    

static uint32_t savedcount, savedmicros;


static void latchOscTime()
{
    noInterrupts();
    //cli();
    savedcount = millis();
    savedmicros = micros();
    interrupts();
    //sei();
}

osctime_t computeOscTime()
{ //4,294,967,296
    osctime_t t;
       savedmicros %= 1000000UL;
    //        t.fractionofseconds = (67108864ULL * (uint64_t)savedmicros) / 15625ULL ; // 2^32/1000000
       t.fractionofseconds= (67108864UL * savedmicros)/ 15625ULL ; // 2^32/1000000
    t.seconds  = savedcount/1000;
    return t;

    
    
}

osctime_t oscTime()
{
    latchOscTime();
    return computeOscTime();
    
}

int adcRead(int pin, osctime_t *t)
{
    latchOscTime();
    
    int v =analogRead(pin);
    *t = oscTime();
    return v;
}
#ifdef BOARD_HAS_CAPACITANCE_SENSING
int capacitanceRead(int pin, osctime_t *t)
{
    latchOscTime();
    int v =  touchRead(pin);
    
    *t = oscTime();
    return v;
}
#endif
int inputRead(int pin, osctime_t *t)
{
    
    int v =digitalRead(pin);
    *t = oscTime();
    
    return v;
}