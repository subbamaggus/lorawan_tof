// Absolute Register-Adressen für STM32WLE5
#define RCC_CR                (*(volatile unsigned int *)0x58001000)
#define RCC_CFGR              (*(volatile unsigned int *)0x58001008)
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

// SCB_VTOR Register (Vector Table Offset Register) - ZWINGEND ERFORDERLICH
#define SCB_VTOR              (*(volatile unsigned int *)0xE000ED08)

extern unsigned int _estack;
void Reset_Handler(void);
int main(void);

// Vektortabelle exakt an den Flash-Anfang (0x08000000) zwingen
__attribute__((section(".vector_table")))
const unsigned int *vector_table[] = {
    (unsigned int *)&_estack,
    (unsigned int *)Reset_Handler
};

// Der allererste Einstiegspunkt nach dem Einschalten
void Reset_Handler(void) {
    // 1. Dem Prozessor sagen, wo die Vektortabelle im Flash liegt (0x08000000)
    SCB_VTOR = 0x08000000;
    
    // 2. Direkt in die Hauptfunktion springen
    main();
    while(1);
}

void delay(volatile int count) {
    while (count--) { __asm__("nop"); }
}

void uart_putc(char c) {
    while (!(USART1_ISR & (1 << 7))); 
    USART1_TDR = c;
}

void uart_puts(char *s) {
    while (*s) uart_putc(*s++);
}

int main(void) {
    // 1. CPU stabil auf den internen MSI-Takt (4 MHz Standard-Werkstakt) bringen
    RCC_CR |= (1 << 0); // MSION aktivieren
    while(!(RCC_CR & (1 << 1))); // Warten bis MSIRDY
    RCC_CFGR &= ~3; // Systemtakt auf MSI umstellen
    
    // 2. Peripherietakt aktivieren (GPIOB und USART1)
    RCC_AHB2ENR |= (1 << 1);  
    RCC_APB2ENR |= (1 << 14); 

    // 3. LED an Pin PB5 konfigurieren (Wio E5 mini User-LED)
    GPIOB_MODER &= ~(3 << (5 * 2)); 
    GPIOB_MODER |= (1 << (5 * 2));  // Output Modus
    GPIOB_OTYPER &= ~(1 << 5);      // Push-Pull
    GPIOB_ODR &= ~(1 << 5);         // LED einschalten (Active-Low!)

    // 4. UART-TX an Pin PB6 konfigurieren (USB-Schnittstelle)
    GPIOB_MODER &= ~(3 << (6 * 2)); 
    GPIOB_MODER |= (2 << (6 * 2));  // Alternate Function Modus
    GPIOB_OSPEEDR |= (3 << (6 * 2)); // High Speed
    GPIOB_PUPDR &= ~(3 << (6 * 2));
    GPIOB_PUPDR |= (1 << (6 * 2));  // Pull-Up aktivieren
    
    GPIOB_AFRL &= ~(0xF << (6 * 4));
    GPIOB_AFRL |= (7 << (6 * 4));   // AF7 zuweisen (USART1_TX)

    // 5. 9600 Baud einstellen bei 4 MHz Werkstakt (4.000.000 / 9600 = 416)
    USART1_BRR = 416; 
    USART1_CR1 |= (1 << 0) | (1 << 3); // UE=1 (UART Enable), TE=1 (Transmitter Enable)

    delay(100000);
    uart_puts("\r\n*** Wio E5 HARDCORE BARE METAL ONLINE ***\r\n");

    while (1) {
        // LED umschalten
        GPIOB_ODR ^= (1 << 5); 
        
        // Text ausgeben
        uart_puts("Blink!\r\n");
        
        delay(400000); 
    }
}
