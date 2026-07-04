#ifndef SYSTEM_H
#define SYSTEM_H

#include "board_config.h"
#include "f103c6t6_regs.h"
#include "stdint.h"
#include "stdbool.h"

#define CPU_FREQUENCY				BOARD_CPU_FREQUENCY
#define SYSTICK_HZ					(100U)		// 100 for systick interupt per 10 milli-second

#define ONE_MHZ_FREQUENCY			(1000000U)
#define SYSTEM_72MHZ_FREQUENCY		(72000000U)

#define MS_TO_TICKS(ms)				(((ms) + 5) / (1000/100))			

typedef enum {
	RESULT_SUCCESS,
	RESULT_FAIL,
	RESULT_INVALID,
	RESULT_NOT_ALLOWED,
	RESULT_TIMEOUT
} system_result_t;

typedef enum {
	ZERO_WAIT_LATENCY,	// If 0 < SYSCLK < 24 MHz
	ONE_WAIT_LATENCY,	// If 24 MHz < SYSCLK < 48 MHz
	TWO_WAIT_LATENCY	// If 48 MHz < SYSCLK < 72 MHz
}	flash_access_latency_t;

typedef enum {
	PLL_INPUT_MUL_4 = 2,
	PLL_INPUT_MUL_5,
	PLL_INPUT_MUL_6,
	PLL_INPUT_MUL_7,
	PLL_INPUT_MUL_8,
	PLL_INPUT_MUL_9,
}	pll_clk_mul_t;

typedef enum {
    DMA1_CLOCK_ENABLE        = RCC_AHB_DMA1_EN_MASK,      // DMA1 enable
    DMA2_CLOCK_ENABLE        = RCC_AHB_DMA2_EN_MASK,      // DMA2 enable
    SRAM_CLOCK_ENABLE        = RCC_AHB_SRAM_EN_MASK,      // SRAM interface clock enable during sleep
    FLITF_CLOCK_ENABLE       = RCC_AHB_FLITF_EN_MASK,     // FLASH interface clock enable during sleep
    CRC_CLOCK_ENABLE         = RCC_AHB_CRC_EN_MASK,       // CRC clock
    FSMC_CLOCK_ENABLE        = RCC_AHB_FSMC_EN_MASK,      // Flexible Static Memory Controller
    SDIO_CLOCK_ENABLE        = RCC_AHB_SDIO_EN_MASK,      // SDIO clock

    AHB_ALL_CONFIGS          =
                                DMA1_CLOCK_ENABLE  |
                                DMA2_CLOCK_ENABLE  |
                                SRAM_CLOCK_ENABLE  |
                                FLITF_CLOCK_ENABLE |
                                CRC_CLOCK_ENABLE   |
                                FSMC_CLOCK_ENABLE  |
                                SDIO_CLOCK_ENABLE

}	ahb_enable_t;

#define CHECK_AHB_CONFIG_VALID(x)		(((x) & ~AHB_ALL_CONFIGS) == 0U)	// Checking AHB configs valid or not 

typedef enum {
	DAC_CLOCK_ENABLE 			=	RCC_APB1_DAC_EN_MASK,		// Digital to Analog clock enable
	PWR_CLOCK_ENABLE 			=	RCC_APB1_POWER_EN_MASK,		// Power clock enable
	BACKUP_CLOCK_ENABLE 		=	RCC_APB1_BKP_EN_MASK,		// Backup clock 
	CAN_CLOCK_ENABLE			=	RCC_APB1_CAN_EN_MASK,		// CAN clock
	USB_CLOCK_ENABLE			=	RCC_APB1_USB_EN_MASK,
	I2C2_CLOCK_ENABLE			=	RCC_APB1_I2C2_EN_MASK,
	I2C1_CLOCK_ENABLE			=	RCC_APB1_I2C1_EN_MASK,
	UART5_CLOCK_ENABLE			=	RCC_APB1_UART5_EN_MASK,
	UART4_CLOCK_ENABLE			=	RCC_APB1_UART4_EN_MASK,
	USART3_CLOCK_ENABLE			=	RCC_APB1_USART3_EN_MASK,
	USART2_CLOCK_ENABLE			=	RCC_APB1_USART2_EN_MASK,
	SPI3_CLOCK_ENABLE			=	RCC_APB1_SPI3_EN_MASK,
	SPI2_CLOCK_ENABLE			=	RCC_APB1_SPI2_EN_MASK,
	WATCHDOG_CLOCK_ENABLE		=	RCC_APB1_WWDGEN_MASK,
	TIMER14_CLOCK_ENABLE		=	RCC_APB1_TIMER14_EN_MASK,
	TIMER13_CLOCK_ENABLE		=	RCC_APB1_TIMER13_EN_MASK,
	TIMER12_CLOCK_ENABLE		=	RCC_APB1_TIMER12_EN_MASK,
	TIMER7_CLOCK_ENABLE			=	RCC_APB1_TIMER7_EN_MASK,
	TIMER6_CLOCK_ENABLE			=	RCC_APB1_TIMER6_EN_MASK,
	TIMER5_CLOCK_ENABLE			=	RCC_APB1_TIMER5_EN_MASK,
	TIMER4_CLOCK_ENABLE			=	RCC_APB1_TIMER4_EN_MASK,
	TIMER3_CLOCK_ENABLE			=	RCC_APB1_TIMER3_EN_MASK,
	TIMER2_CLOCK_ENABLE			=	RCC_APB1_TIMER2_EN_MASK,
	APB1_ALL_CONFIGS 			=								// It is used to check whether the APB1 config is valid or not
									DAC_CLOCK_ENABLE      |
									PWR_CLOCK_ENABLE      |
									BACKUP_CLOCK_ENABLE   |
									CAN_CLOCK_ENABLE      |
									USB_CLOCK_ENABLE      |
									I2C2_CLOCK_ENABLE     |
									I2C1_CLOCK_ENABLE     |
									UART5_CLOCK_ENABLE    |
									UART4_CLOCK_ENABLE    |
									USART3_CLOCK_ENABLE   |
									USART2_CLOCK_ENABLE   |
									SPI3_CLOCK_ENABLE     |
									SPI2_CLOCK_ENABLE     |
									WATCHDOG_CLOCK_ENABLE |
									TIMER14_CLOCK_ENABLE  |
									TIMER13_CLOCK_ENABLE  |
									TIMER12_CLOCK_ENABLE  |
									TIMER7_CLOCK_ENABLE   |
									TIMER6_CLOCK_ENABLE   |
									TIMER5_CLOCK_ENABLE   |
									TIMER4_CLOCK_ENABLE   |
									TIMER3_CLOCK_ENABLE   |
									TIMER2_CLOCK_ENABLE

}	apb1_enable_t;

#define CHECK_APB1_CONFIG_VALID(x)		(((x) & ~APB1_ALL_CONFIGS) == 0U)	// Checking APB1 configs valid or not 

typedef enum {
    AFIO_CLOCK_ENABLE        = RCC_APB2_AFIO_EN_MASK,
    GPIOA_CLOCK_ENABLE       = RCC_APB2_GPIOA_EN_MASK,
    GPIOB_CLOCK_ENABLE       = RCC_APB2_GPIOB_EN_MASK,
    GPIOC_CLOCK_ENABLE       = RCC_APB2_GPIOC_EN_MASK,
    GPIOD_CLOCK_ENABLE       = RCC_APB2_GPIOD_EN_MASK,
    GPIOE_CLOCK_ENABLE       = RCC_APB2_GPIOE_EN_MASK,
    GPIOF_CLOCK_ENABLE       = RCC_APB2_GPIOF_EN_MASK,
    GPIOG_CLOCK_ENABLE       = RCC_APB2_GPIOG_EN_MASK,

    ADC1_CLOCK_ENABLE        = RCC_APB2_ADC1_EN_MASK,
    ADC2_CLOCK_ENABLE        = RCC_APB2_ADC2_EN_MASK,

    TIMER1_CLOCK_ENABLE      = RCC_APB2_TIMER1_EN_MASK,
    TIMER8_CLOCK_ENABLE      = RCC_APB2_TIMER8_EN_MASK,

    SPI1_CLOCK_ENABLE        = RCC_APB2_SPI1_EN_MASK,
    USART1_CLOCK_ENABLE      = RCC_APB2_USART1_EN_MASK,

    APB2_ALL_CONFIGS         =
                                AFIO_CLOCK_ENABLE     |
                                GPIOA_CLOCK_ENABLE    |
                                GPIOB_CLOCK_ENABLE    |
                                GPIOC_CLOCK_ENABLE    |
                                GPIOD_CLOCK_ENABLE    |
                                GPIOE_CLOCK_ENABLE    |
                                GPIOF_CLOCK_ENABLE    |
                                GPIOG_CLOCK_ENABLE    |
                                ADC1_CLOCK_ENABLE     |
                                ADC2_CLOCK_ENABLE     |
                                TIMER1_CLOCK_ENABLE   |
                                TIMER8_CLOCK_ENABLE   |
                                SPI1_CLOCK_ENABLE     |
                                USART1_CLOCK_ENABLE

} apb2_enable_t;

#define CHECK_APB2_CONFIG_VALID(x)		(((x) & ~APB2_ALL_CONFIGS) == 0U)	// Checking APB2 configs valid or not 


typedef enum {
	APB1_NO_DIVISION,
	APB1_DIVISION_2 = 4,
	APB1_DIVISION_4,
	APB1_DIVISION_8,
	APB1_DIVISION_16
}	apb1_clk_ctrl_t;

typedef enum {
	APB2_NO_DIVISION,
	APB2_DIVISION_2,
	APB2_DIVISION_4,
	APB2_DIVISION_8,
	APB2_DIVISION_16
}	apb2_clk_ctrl_t;

typedef enum {
	HSI_SYSTEM_CLOCK,
	HSE_SYSTEM_CLOCK,
	PLL_SYSTEM_CLOCK,
	INVALID_CLOCK
}	system_clk_t;

typedef enum {
	ADC_SYSCLK_DIV_2,
	ADC_SYSCLK_DIV_4,
	ADC_SYSCLK_DIV_6,
	ADC_SYSCLK_DIV_8
}	adc_pre_t;	// RCC CFG register adc prescaler config

/********************************************************************************
 * @brief Initializes the system clock configuration.
 * @note  Initially, the hardware will use an 8MHz Internal oscillator (HSI) for easy startup.
 * 		  It will also be used when device in stop or standby mode, and in case of failure
 * 		  of External High-speed oscillator (HSE). 
 * @note  We control and enable HSE using RCC register, because we need HSE for UART, SPI 
 * 		  and other communications.
 * @return RESULT_SUCCESS - Indicating system clock initialized sucessfully.
 ********************************************************************************/
system_result_t system_clk_init(void);

system_result_t system_init(void);

system_result_t system_tick_init(void);

uint32_t systick_get_ms(void);

void delay(uint32_t delay_ms);

/************************************************************************************
 *  This function enables clock AHB (advanced high performance peripheral bus)
 *  based configurations like DMA, SRAM, FLITF, etc.
 *  it checks whether enable configs is valid or not. checking corresponding bit
 *  positions are valid to write or not.
 * 
 * @param enable - This variable contains ahb clock enable configurations.
 * 
 * @return  RESULT_SUCESS : If enable configurations are valid and values written successfully.
 *          
 * 			RESULT_INVALID : If enable configurations is invalid. this will returned if any one bit position is invalid.
 */
system_result_t AHB_enable_clock(ahb_enable_t enable);

/************************************************************************************
 *  This function enables clock APB1 (advanced peripheral bus) - low speed peripheral
 *  based configurations like DAC, USB, I2C, timers, etc.
 *  it checks whether enable configs is valid or not. checking corresponding bit
 *  positions are valid to write or not.
 * 
 * @param enable - This variable contains apb1 clock enable configurations.
 * 
 * @return  RESULT_SUCESS : If enable configurations are valid and values written successfully.
 *          
 * 			RESULT_INVALID : If enable configurations is invalid. this will returned if any one bit position is invalid.
 */
system_result_t APB1_enable_clock(apb1_enable_t enable);

/************************************************************************************
 *  This function enables clock APB2 (advanced peripheral bus) - high speed peripheral
 *  based configurations like AFIO, GPIO, ADC, SPI, UART, etc.
 *  it checks whether enable configs is valid or not. checking corresponding bit
 *  positions are valid to write or not.
 * 
 * @param enable - This variable contains apb2 clock enable configurations.
 * 
 * @return  RESULT_SUCESS : If enable configurations are valid and values written successfully.
 *          
 * 			RESULT_INVALID : If enable configurations is invalid. this will returned if any one bit position is invalid.
 */
system_result_t APB2_enable_clock(apb2_enable_t enable);

system_result_t RCC_adc_config_clock();

#endif
