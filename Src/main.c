#include "stm32f4xx.h"

void SystemClock_Config(void);
void GPIO_Init(void);
void Delay(volatile uint32_t count);

int main(void) {
    // Configure system clock
    SystemClock_Config();

    // Initialize GPIO (PE13 as output)
    GPIO_Init();

    while (1) {
        GPIOE->ODR ^= GPIO_ODR_OD13;  // Toggle PE13
        Delay(1000000);               // Delay for blinking
    }
}

void SystemClock_Config(void) {
    // Enable HSE (High-Speed External Clock)
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));  // Wait for HSE to be ready

    // Configure PLL: PLL_M = 25, PLL_N = 336, PLL_P = 2, PLL_Q = 7
    RCC->PLLCFGR = (25 << RCC_PLLCFGR_PLLM_Pos) |  // PLLM = 25
                   (336 << RCC_PLLCFGR_PLLN_Pos) | // PLLN = 336
                   (0 << RCC_PLLCFGR_PLLP_Pos) |   // PLLP = 2 (00)
                   (7 << RCC_PLLCFGR_PLLQ_Pos) |   // PLLQ = 7
                   RCC_PLLCFGR_PLLSRC_HSE;         // Use HSE as PLL source

    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));  // Wait for PLL to lock

    // Set Flash Latency and Enable Prefetch Buffer
    FLASH->ACR |= FLASH_ACR_LATENCY_5WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

    // Set system clock to PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));  // Wait for PLL to be used as SYSCLK

    // Set AHB = 168 MHz, APB1 = 42 MHz, APB2 = 84 MHz
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;    // AHB Prescaler = 1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;   // APB1 Prescaler = 4 (42 MHz)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;   // APB2 Prescaler = 2 (84 MHz)
}

void GPIO_Init(void) {
    // Enable GPIOE clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

    // Set PE13 as output mode
    GPIOE->MODER &= ~(3 << (13 * 2));  // Clear mode bits
    GPIOE->MODER |= (1 << (13 * 2));   // Set as output (01)

    // Set output speed to high
    GPIOE->OSPEEDR &= ~(3 << (13 * 2));  // Clear speed bits
    GPIOE->OSPEEDR |= (3 << (13 * 2));   // Set High speed

    // No pull-up/pull-down
    GPIOE->PUPDR &= ~(3 << (13 * 2));

    // Set initial state to LOW
    GPIOE->ODR &= ~GPIO_ODR_OD13;
}

void Delay(volatile uint32_t count) {
    while (count--) {
        __asm("nop");
    }
}
