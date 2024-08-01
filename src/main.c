#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

// Begin Decls
void select_w5500(void);
void deselect_w5500(void);
void reset_w5500(void);

int main(void) {
    // Init clock config
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_SPI2);  // Enable SPI2 clock
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

    // Initialize all the pins that will be used
    // gpio A
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8); // RESET W5500

    // gpio B
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO13 | GPIO15); // SCK | MOSI
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO14); // MISO
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12); // CS

    // gpio C
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13 | GPIO14);

    // Init master of the SPI2
    spi_init_master(SPI2,
                    SPI_CR1_BAUDRATE_FPCLK_DIV_64,  /* Adjust speed as needed */
                    SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_1,
                    SPI_CR1_DFF_8BIT,
                    SPI_CR1_MSBFIRST);
    
    spi_enable(SPI2);

    while (1) {
        for (int i = 0; i < 500000; i++) {
            __asm__("nop");
        }
        
        gpio_toggle(GPIOC, GPIO13);
        
        // Check if the transmit buffer is empty
        while (!(SPI_SR(SPI2) & SPI_SR_TXE)) {
            // Wait until TXE is set
        }
        
        // Send data
        spi_send(SPI2, 0xFFFF);
        
        // Wait until the transfer is complete
        while (!(SPI_SR(SPI2) & SPI_SR_RXNE)) {
            // Wait until RXNE is set
        }

        // Read the received data (to clear the RXNE flag)
        uint16_t received_data = spi_read(SPI2);
    }
}

void select_w5500() {
    gpio_clear(GPIOB, GPIO12);
}

void deselect_w5500() {
    gpio_set(GPIOB, GPIO12);
}

void reset_w5500() {
    gpio_clear(GPIOA, GPIO8);
    for (int i = 0; i < 10000000; i++) {
        __asm__("nop");
    }
    gpio_set(GPIOA, GPIO8);
}
