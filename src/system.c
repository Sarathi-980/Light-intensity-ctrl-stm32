#include "system.h"
#include "system_exceptions.h"

/*  FLASH_ACR is used to enable/disable cpu prefetch, half cycle access
    and control CPU flash access time according to CPU frequency. 
    when boosting CPU frequency we should match the instruction fetch,
    other wise data will be corrupted. 
    we fix this by modifying latency wait states and enabling prefetch. 
    prefetch will match the wait cycle.
*/
void flash_interface_set_latency(flash_access_latency_t latency) {
    uint32_t reg_value =  FLASH_ACR_REG;

    reg_value &= ~FLASH_ACR_LATENCY_MASK;
    reg_value |= (FLASH_ACR_PREFETCH_BUF_EN_MASK | latency);
    
    FLASH_ACR_REG = reg_value;
}

/* The external oscillator frequency is 8 MHz, multiplying by 9 gives 72 MHz.
    PLL shouldn't exceed 72 MHz.
    We should prescale the APB1 frequency because 36 MHz is maximum, we divide 72 MHz by 2.
*/
void rcc_pll_multiply(pll_clk_mul_t multiple, apb1_clk_ctrl_t apb1_clk) { 
    uint32_t reg_value = RCC_REGISTERS_ADDR->RCC_CFGR;
    // Enable HSE as a PLL source clock
    reg_value |= RCC_CFG_PLL_SRC_HSE_EN;                

    reg_value &= ~(RCC_CFG_PLL_MUL_MASK | RCC_CFG_PPRE1_MASK);
    reg_value |= (multiple << RCC_CFG_PLL_MUL_POS) | (apb1_clk << RCC_CFG_PPRE1_POS);
 
    RCC_REGISTERS_ADDR->RCC_CFGR = reg_value;
}

system_result_t system_clk_init() 
{
    // Enable HSE clock
    RCC_REGISTERS_ADDR->RCC_CR |= RCC_CR_HSE_ENABLE_MASK;      

    // waiting for HSE stable and ready
    while (!(RCC_REGISTERS_ADDR->RCC_CR & RCC_CR_HSE_READY_FLAG_MASK)); 
    // Set latency before jumping the clock to 72 MHz   
    flash_interface_set_latency(TWO_WAIT_LATENCY);

    // Configure PLL multiplier and APB1 scaler as 36 MHz
    // we ignore division for apb2 and ahb they will be same as system clock(72 MHz)
    rcc_pll_multiply(PLL_INPUT_MUL_9, APB1_DIVISION_2);
    // Enable PLL clock
    RCC_REGISTERS_ADDR->RCC_CR |= RCC_PLL_CLOCK_EN_MASK;

    // wait for PLL to become stable and ready
    while ((RCC_REGISTERS_ADDR->RCC_CR & RCC_CR_PLL_READY_MASK) != RCC_CR_PLL_READY_MASK); 

    RCC_REGISTERS_ADDR->RCC_CFGR &= ~RCC_CFG_SW_MASK;
    // Enable PLL as a system clock
    RCC_REGISTERS_ADDR->RCC_CFGR |= PLL_SYSTEM_CLOCK;      

    // Verify PLL selected as system clock
    while ((RCC_REGISTERS_ADDR->RCC_CFGR & RCC_CFG_SWS_MASK) != (PLL_SYSTEM_CLOCK << 2));

    return RESULT_SUCCESS;
}

system_result_t system_tick_init() {
    // Disabling systick timer
    SYSTICK_CONTROL_STATUS_REG_ADDR  &= ~SYSTICK_TIMER_ENABLE_MASK;
    // Systick interrupt will trigger per 10 ms
    SYSTICK_RELOAD_REG_ADDR = (CPU_FREQUENCY / SYSTICK_HZ) - 1;     // 72 000 000 / 100 => 720 000
    // Clear the ticks current value
    SYSTICK_CURRENT_VAL_REG_ADDR = 0;
    // Using core clock for systick and enabling systick timer
    SYSTICK_CONTROL_STATUS_REG_ADDR |= (SYSTICK_CLK_SOURCE_MASK | SYSTICK_INTERRUPT_ENABLE_MASK | SYSTICK_TIMER_ENABLE_MASK);
    return RESULT_SUCCESS;
}

system_result_t system_init() {
    system_result_t status = system_clk_init();
    if (status != RESULT_SUCCESS) {
        return status;
    }

    status = system_tick_init();
    if (status != RESULT_SUCCESS) {
        return status;
    }
    return RESULT_SUCCESS;
}


void delay(uint32_t delay_ms)
{
    uint32_t start = get_systick_count();
    uint32_t current = get_systick_count();
    while ((current - start) < delay_ms) {
        current = get_systick_count();
        __asm volatile ("wfi");
    }
}

uint32_t systick_get_ms() {
    uint32_t l_systick = get_systick_count();
    // Based how many interrupt fired, we can get how many 10 milli-seconds passed
    // next based on cycles passed we get milli-second passed
    // return l_systick * (1000 / SYSTICK_HZ) + 
    //     (CPU_FREQUENCY / SYSTICK_HZ - (SYSTICK_CURRENT_VAL_MASK(SYSTICK_CURRENT_VAL_REG_ADDR))) / (CPU_FREQUENCY / 1000);
    return l_systick;
}

system_result_t AHB_enable_clock(ahb_enable_t enable) {
    if (enable == 0 || !CHECK_AHB_CONFIG_VALID(enable)) {
        return RESULT_INVALID;
    }
    
    RCC_REGISTERS_ADDR->RCC_AHBENR |= enable;
    return RESULT_SUCCESS;
}

system_result_t APB1_enable_clock(apb1_enable_t enable) {
    if (enable == 0 || !CHECK_APB1_CONFIG_VALID(enable)) {
        return RESULT_INVALID;
    }
    
    RCC_REGISTERS_ADDR->RCC_APB1ENR |= enable;
    return RESULT_SUCCESS;
}

system_result_t APB2_enable_clock(apb2_enable_t enable) {
    if (enable == 0 || !CHECK_APB2_CONFIG_VALID(enable)) {
        return RESULT_INVALID;
    }
    
    RCC_REGISTERS_ADDR->RCC_APB2ENR |= enable;
    return RESULT_SUCCESS;
}

system_result_t RCC_adc_config_clock() {
    // Enable adc clock peripheral
    apb2_enable_t enable_clk = ADC1_CLOCK_ENABLE;
    system_result_t result = APB2_enable_clock(enable_clk);
    if (result != RESULT_SUCCESS) {
        return RESULT_INVALID;
    }

    // The ADC input clock is generated from PCLK2 clock divided by prescaler
    // It must not exceed 14 MHz
    uint32_t reg_value = RCC_REGISTERS_ADDR->RCC_CFGR;
    reg_value &= ~RCC_CFG_ADCPRE_MASK;
    reg_value |= (ADC_SYSCLK_DIV_6 << RCC_CFG_ADCPRE_POS);      // 72 / 6 -> 12 MHz signal to adc

    RCC_REGISTERS_ADDR->RCC_CFGR = reg_value;
    return RESULT_SUCCESS;
}