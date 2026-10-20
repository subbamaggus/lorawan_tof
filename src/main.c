// Typische Register-Definitionen für den STM32WLE5, um ohne HAL-Treiber direkt Ports zu schalten
#define PERIPH_BASE           ((unsigned int)0x40000000)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x00000400)
#define RCC_BASE              (AHB2PERIPH_BASE + 0x00001000)

#define RCC_AHB2ENR           (*(volatile unsigned int *)(RCC_BASE + 0x4C))
#define GPIOB_MODER           (*(volatile unsigned int *)(GPIOB_BASE + 0x00))
#define GPIOB_ODR             (*(volatile unsigned int *)(GPIOB_BASE + 0x14))

void delay(volatile int count) {
    while (count--) {
        __asm__("nop");
    }
}

int main(void) {
    // 1. Takt für Port B einschalten
    RCC_AHB2ENR |= (1 << 1); 

    // 2. Pin PB5 als Ausgang (Output) konfigurieren
    GPIOB_MODER &= ~(3 << (5 * 2)); // Bits löschen
    GPIOB_MODER |= (1 << (5 * 2));  // Auf "01" (General Purpose Output) setzen

    while (1) {
        // 3. Pin PB5 umschalten (Toggle)
        GPIOB_ODR ^= (1 << 5); 
        
        // Kleine Pause (ca. 500ms je nach internem CPU-Takt)
        delay(100000); 
    }
}
