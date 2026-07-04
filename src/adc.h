#ifndef ADC_H
#define ADC_H

#include "system.h"
#include "gpio.h"
#include "system.h"
#include "f103c6t6_regs.h"

typedef enum {
    ADC_CHANNEL_0 = 0,  // PA0...
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,      // PA7
    ADC_CHANNEL_8,      // PB0
    ADC_CHANNEL_9       // PB1
} adc_channel_t;

/* ADC sample register - for STM32F103C6T6 it has only 10 channels
   Data accurate based on sample cycles count (direct propotional). */
typedef enum {
    ADC_SAMPLE_1_5_CYCLES = 0,
    ADC_SAMPLE_7_5_CYCLES,
    ADC_SAMPLE_13_5_CYCLES,
    ADC_SAMPLE_28_5_CYCLES,
    ADC_SAMPLE_41_5_CYCLES,
    ADC_SAMPLE_55_5_CYCLES,
    ADC_SAMPLE_71_5_CYCLES,
    ADC_SAMPLE_239_5_CYCLES
} adc_sample_time_t;

// external event select
typedef enum {
    ADC_EXTSEL_TIM1_CC1 = 0,  // Timer 1 CC1 event
    ADC_EXTSEL_TIM1_CC2 = 1,  // Timer 1 CC2 event
    ADC_EXTSEL_TIM1_CC3 = 2,  // Timer 1 CC3 event
    ADC_EXTSEL_TIM2_CC2 = 3,  // Timer 2 CC2 event
    ADC_EXTSEL_TIM3_TRGO = 4, // Timer 3 TRGO event
    ADC_EXTSEL_TIM4_CC4 = 5,  // Timer 4 CC4 event
    ADC_EXTSEL_EXTI11   = 6,  // EXTI line 11/TIM8_TRGO
    ADC_EXTSEL_SWSTART  = 7   // Software start
} adc_extsel_t;

// sequence length
typedef enum {
    ADC_SEQUENCE_LEN_1  = 0,
    ADC_SEQUENCE_LEN_2  = 1,
    ADC_SEQUENCE_LEN_3  = 2,
    ADC_SEQUENCE_LEN_4  = 3,
    ADC_SEQUENCE_LEN_5  = 4,
    ADC_SEQUENCE_LEN_6  = 5,
    ADC_SEQUENCE_LEN_7  = 6,
    ADC_SEQUENCE_LEN_8  = 7,
    ADC_SEQUENCE_LEN_9  = 8,
    ADC_SEQUENCE_LEN_10 = 9,
    ADC_SEQUENCE_LEN_11 = 10,
    ADC_SEQUENCE_LEN_12 = 11,
    ADC_SEQUENCE_LEN_13 = 12,
    ADC_SEQUENCE_LEN_14 = 13,
    ADC_SEQUENCE_LEN_15 = 14,
    ADC_SEQUENCE_LEN_16 = 15
} ADC_seq_length_t;



#define LDR_ADC_CHANNEL          ADC_CHANNEL_1
#define LDR_ADC_SAMPLE_TIME      ADC_SAMPLE_239_5_CYCLES


system_result_t adc1_read(adc_channel_t channel, gpio_config_req_t adc_gpio, uint16_t *adc_value);
#endif