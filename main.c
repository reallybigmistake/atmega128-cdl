#include "test_config.h"
int main(void){
    struct atmega128_uart* p_uart;
    p_uart->id = 0;
    p_uart->u2x = 0;
    uart_init(p_uart, 38400, NO_PARITY, 8, 1);

    
    return 0;
}