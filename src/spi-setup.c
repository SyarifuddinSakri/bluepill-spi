#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
// pinA3 - reset
// pinA4 - chip select
// pinA5 - spi clock
// pinA6 - input from slave (MISO)
// pinA7 - output to the slave (MOSI)
void spi_conf(){
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5 | GPIO7);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6);
    gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_PUSHPULL, GPIO3 | GPIO4);

    //set to high to deselect the SPI by default 
    gpio_set(GPIOA, GPIO4);

    spi_init_master(SPI1,
                    SPI_CR1_BAUDRATE_FPCLK_DIV_64,  /* Adjust speed as needed */
                    SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_1,
                    SPI_CR1_DFF_8BIT,
                    SPI_CR1_MSBFIRST);

    spi_enable_ss_output(SPI1); /* Enable software slave management */
    spi_enable(SPI1);           /* Enable SPI1 */
}

void select_w5500(){
    gpio_clear(GPIOA, GPIO4);
}

void deselect_w5500(){
    gpio_set(GPIOA, GPIO4);
}