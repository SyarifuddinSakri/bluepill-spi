#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

int main(){
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO13);

    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO7 | GPIO5 | GPIO6 | GPIO3 | GPIO4);

    while(1){
        for(int i=0; i<100000; i++){
            __asm__("nop");
        }

        gpio_toggle(GPIOA, GPIO7);
        gpio_toggle(GPIOA, GPIO6);
        gpio_toggle(GPIOA, GPIO5);
        gpio_toggle(GPIOA, GPIO4);
        gpio_toggle(GPIOA, GPIO3);
        gpio_toggle(GPIOC, GPIO13);
    }
}