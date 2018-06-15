#include "console.h"
int main(void)
{
    console_init(0, 38400);
    unsigned long baud = console_baud();
    console_printf("baud -> %ul\r\n", baud); 
    return 0;
}