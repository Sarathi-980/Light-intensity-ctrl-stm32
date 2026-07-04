#include "adc.h"
 
static bool adc_enabled = false;  

system_result_t adc1_init(gpio_config_req_t adc_gpio_config) {
    if (adc_enabled) {
        return RESULT_NOT_ALLOWED;
    }
    system_result_t result = RCC_adc_config_clock();
    if (result != RESULT_SUCCESS) {
        return result;
    }

    // ADC base register address
    volatile ADCx_registers_t *adc1_base = ADC1_REGISTERS;

    // ADC power on. wakes up the ADC from power down mode
    adc1_base->ADC_CR2 |= ADC_CR2_ADON_MASK;

    delay(MS_TO_TICKS(10));   // delay for ADC to stable

    // Resetting calibration data registers
    adc1_base->ADC_CR2 |= ADC_CR2_RSTCAL_MASK;  // Initialize reset calibration
    while (adc1_base->ADC_CR2 & ADC_CR2_RSTCAL_MASK);  // Wait until hardware clears. 

    // ADC self calibration to reduce accuracy error
    adc1_base->ADC_CR2 |= ADC_CR2_CAL_MASK;         // Enable calibration
    while (adc1_base->ADC_CR2 & ADC_CR2_CAL_MASK);  // Calibration finished set by hardware

    // Select alignment of data stored after conversion. 0 - right, 1 - left
    adc1_base->ADC_CR2 &= ~ADC_CR2_ALIGN_MASK;

    // Conversion type 0 - single conversion, 1 - continous conversion
    adc1_base->ADC_CR2 &= ~ADC_CR2_CONT_MASK;   // Enable single conversion mode

    /*  Since we selected single conversion mode, we should indicate when 
        the conversion must be started. 
    */
    adc1_base->ADC_CR2 &= ~ADC_CR2_EXTSEL_MASK;
    adc1_base->ADC_CR2 |= (ADC_EXTSEL_SWSTART << ADC_CR2_EXTSEL_POS);     // trigger adc by software

    adc1_base->ADC_CR2 |= ADC_CR2_EXTTRIG_MASK;     // Enable conversion on external event
 
    result = gpio_configure(adc_gpio_config);
    if (result != RESULT_SUCCESS) {
        return result;
    }

    adc_enabled = true;
    return RESULT_SUCCESS;
}


system_result_t adc1_read(adc_channel_t channel, gpio_config_req_t adc_gpio, uint16_t *adc_value)
{
    if (!adc_value) {
        return RESULT_INVALID;
    }

    if (!adc_enabled) {
        if (adc1_init(adc_gpio) != RESULT_SUCCESS) {
            return RESULT_FAIL;
        }
    }

    if (channel > ADC_CHANNEL_9) {
        return RESULT_INVALID;
    }

    volatile ADCx_registers_t *adc1_base = ADC1_REGISTERS;

    //  select the sample time individually for a channel
    adc1_base->ADC_SMPR2 &= ~ADC_SMPR2_SMP_MASK(channel);
    adc1_base->ADC_SMPR2 |= ADC_SMPR2_SMP_SET(LDR_ADC_SAMPLE_TIME, channel); // f103x has 10 external channels upto PB1 only

    // Regular sequence length = 1 conversion. 0000 means 1 conversion
    adc1_base->ADC_SQR1 &= ~ADC_SQR1_L_MASK;

    // First conversion in regular sequence, setting channel
    adc1_base->ADC_SQR3 &= ~ADC_SQR3_SQ1_MASK;
    adc1_base->ADC_SQR3 |= ((uint32_t)channel << ADC_SQR3_SQ1_POS);

    // Clearing End of conversion before starting ADC
    adc1_base->ADC_SR &= ~ADC_SR_EOC_MASK;

    // Start ADC
    adc1_base->ADC_CR2 |= ADC_CR2_SWSTART_MASK;

    // Checking conversion is complete. It is set by hardware
    while (!(adc1_base->ADC_SR & ADC_SR_EOC_MASK));

    // ADC1 data [15 : 0]
    *adc_value = (uint16_t)(adc1_base->ADC_DR & 0xFFFFU);

    return RESULT_SUCCESS;
}