// Register-Definitionen für STM32WLE5
#define RCC_AHB2ENR           (*(volatile unsigned int *)0x5800104C) 
#define RCC_APB2ENR           (*(volatile unsigned int *)0x58001060) 
#define GPIOB_MODER           (*(volatile unsigned int *)0x48000400) 
#define GPIOB_OTYPER          (*(volatile unsigned int *)0x48000404) 
#define GPIOB_OSPEEDR         (*(volatile unsigned int *)0x48000408) 
#define GPIOB_PUPDR           (*(volatile unsigned int *)0x4800040C) 
#define GPIOB_ODR             (*(volatile unsigned int *)0x48000414) 
#define GPIOB_AFRL            (*(volatile unsigned int *)0x48000420) 
#define USART1_CR1            (*(volatile unsigned int *)0x40013800)
#define USART1_BRR            (*(volatile unsigned int *)0x4001380C)
#define USART1_ISR            (*(volatile unsigned int *)0x4001381C)
#define USART1_TDR            (*(volatile unsigned int *)0x40013828)

extern unsigned int _estack; // Kommt aus dem Linker-Skript
void Reset_Handler(void);
int main(void);

// 1. Die minimale Vektortabelle für den Cortex-M4 Bootloader
__attribute__((section(".vector_table")))
const unsigned int * VectorTable[] = {
    (unsigned int *) &_estack,        // 1. Initialer Stack Pointer
    (unsigned int *) Reset_Handler    // 2. Reset Vektor (Hier startet die CPU)
};

// 2. Der Reset Handler (wird direkt nach dem Einschalten aufgerufen)
void Reset_Handler(void) {
    main(); // Springe direkt in die main-Schleife
    while(1);
}

void delay(volatile int count) {
    while (count--) { __asm__("nop"); }
}

void uart_putc(char c) {
    while (!(USART1_ISR & (1 << 7))); // Warten auf TXE
    USART1_TDR = c;
}

void uart_puts(char *s) {
    while (*s) uart_putc(*s++);
}

int main(void) {
    // Takt einschalten
    RCC_AHB2ENR |= (1 << 1);  // GPIOB
    RCC_APB2ENR |= (1 << 14); // USART1

    // LED an PB5 konfigurieren
    GPIOB_MODER &= ~(3 << (5 * 2)); 
    GPIOB_MODER |= (1 << (5 * 2));  
    GPIOB_OTYPER &= ~(1 << 5);      

    // UART-TX an PB6 konfigurieren
    GPIOB_MODER &= ~(3 << (6 * 2)); 
    GPIOB_MODER |= (2 << (6 * 2));  
    GPIOB_OSPEEDR |= (3 << (6 * 2)); 
    GPIOB_PUPDR &= ~(3 << (6 * 2));
    GPIOB_PUPDR |= (1 << (6 * 2));  
    
    GPIOB_AFRL &= ~(0xF << (6 * 4));
    GPIOB_AFRL |= (7 << (6 * 4));  

    // Baudrate: 9600 Baud bei 4 MHz Standard-MSI-Takt
    USART1_BRR = 416; 
    USART1_CR1 |= (1 << 0) | (1 << 3); // UE=1, TE=1

    delay(50000);

    while (1) {
        GPIOB_ODR ^= (1 << 5); // LED Blinken
        uart_puts("Wio E5 Bare-Metal lebt!\r\n");
        delay(300000); 
    }
}
