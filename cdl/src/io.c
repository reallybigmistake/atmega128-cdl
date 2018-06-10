#include "io.h"

void blink(int ms){
    DDRE = 0xff;
    PORTE = 0xff;
    
    while(1)
    {
        PORTE = PORTE -1;
        _delay_ms(ms);
    }

}