#include "timer.h" 

static const uint32_t timer_base_address_table[TIMER_COUNT] = {
    [TIMER_TIM1]  = TIMER1_BASE_ADDRESS,
    [TIMER_TIM2]  = TIMER2_BASE_ADDRESS,
    [TIMER_TIM3]  = TIMER3_BASE_ADDRESS,
    [TIMER_TIM4]  = TIMER4_BASE_ADDRESS,
    [TIMER_TIM5]  = TIMER5_BASE_ADDRESS,
    [TIMER_TIM6]  = TIMER6_BASE_ADDRESS,
    [TIMER_TIM7]  = TIMER7_BASE_ADDRESS,
    [TIMER_TIM8]  = TIMER8_BASE_ADDRESS,
    [TIMER_TIM9]  = TIMER9_BASE_ADDRESS,
    [TIMER_TIM10] = TIMER10_BASE_ADDRESS,
    [TIMER_TIM11] = TIMER11_BASE_ADDRESS,
    [TIMER_TIM12] = TIMER12_BASE_ADDRESS,
    [TIMER_TIM13] = TIMER13_BASE_ADDRESS,
    [TIMER_TIM14] = TIMER14_BASE_ADDRESS,
};

static system_result_t timer_configure_channel(volatile TIMx_registers_t* timer, channel_config_req_t configs)
{   
    if (!timer) {
        return RESULT_INVALID;
    }
    if (configs.channel_direction > TIMER_CCx_CONFIG_INPUT_TRC) {
        return RESULT_INVALID;
    }

    // Capture compare selection position
    uint32_t ccxs_pos;

    // Capture compare register
    volatile uint32_t*  ccr_reg;
    // Capture compare mode register
    volatile uint32_t*  ccmr_reg;
    
    channel_io_req_t input_output_cfg;

    uint32_t ccxe_pos;

    switch (configs.config_channel)
    {
        case TIMER_CHANNEL_1:
            ccr_reg = &timer->TIMx_CCR1;
            ccmr_reg = &timer->TIMx_CCMR1;
            ccxs_pos = TIMx_CCMR1_OUT_CC1S_POS;
            ccxe_pos = TIMx_CCER_CC1E_POS;
            if (configs.channel_direction == TIMER_CCx_CONFIG_OUTPUT) {
                input_output_cfg.output_cfg.ocxm_pos = TIMx_CCMR1_OUT_OC1M_POS;
                input_output_cfg.output_cfg.ocxpe_mask = TIMx_CCMR1_OUT_OC1PE_MASK;
            }
            else {
                input_output_cfg.input_cfg.icxf_pos = TIMx_CCMR1_INP_IC1F_POS;
                input_output_cfg.input_cfg.icxpsc_pos = TIMx_CCMR1_INP_IC1PSC_POS; 
            }
             
            break;
        
        case TIMER_CHANNEL_2:
            ccr_reg = &timer->TIMx_CCR2;
            ccmr_reg = &timer->TIMx_CCMR1;
            ccxs_pos = TIMx_CCMR1_OUT_CC2S_POS;
            ccxe_pos = TIMx_CCER_CC2E_POS;
            if (configs.channel_direction == TIMER_CCx_CONFIG_OUTPUT) {
                input_output_cfg.output_cfg.ocxm_pos = TIMx_CCMR1_OUT_OC2M_POS;
                input_output_cfg.output_cfg.ocxpe_mask = TIMx_CCMR1_OUT_OC2PE_MASK; 
            }
            else {
                input_output_cfg.input_cfg.icxf_pos = TIMx_CCMR1_INP_IC2F_POS;
                input_output_cfg.input_cfg.icxpsc_pos = TIMx_CCMR1_INP_IC2PSC_POS; 
            }
            break;

        case TIMER_CHANNEL_3:
            ccr_reg = &timer->TIMx_CCR3;
            ccmr_reg = &timer->TIMx_CCMR2; 
            ccxs_pos = TIMx_CCMR2_OUT_CC3S_POS;
            ccxe_pos = TIMx_CCER_CC3E_POS;
            if (configs.channel_direction == TIMER_CCx_CONFIG_OUTPUT) {
                input_output_cfg.output_cfg.ocxm_pos = TIMx_CCMR2_OUT_OC3M_POS;
                input_output_cfg.output_cfg.ocxpe_mask = TIMx_CCMR2_OUT_OC3PE_MASK; 
            }
            else {
                input_output_cfg.input_cfg.icxf_pos = TIMx_CCMR2_INP_IC3F_POS;
                input_output_cfg.input_cfg.icxpsc_pos = TIMx_CCMR2_INP_IC3PSC_POS; 
            }
            break;
        
        case TIMER_CHANNEL_4:
            ccr_reg = &timer->TIMx_CCR4;
            ccmr_reg = &timer->TIMx_CCMR2;
            ccxs_pos = TIMx_CCMR2_OUT_CC4S_POS;
            ccxe_pos = TIMx_CCER_CC4E_POS;
            if (configs.channel_direction == TIMER_CCx_CONFIG_OUTPUT) {
                input_output_cfg.output_cfg.ocxm_pos = TIMx_CCMR2_OUT_OC4M_POS;
                input_output_cfg.output_cfg.ocxpe_mask = TIMx_CCMR2_OUT_OC4PE_MASK; 
            }
            else {
                input_output_cfg.input_cfg.icxf_pos = TIMx_CCMR2_INP_IC4F_POS;
                input_output_cfg.input_cfg.icxpsc_pos = TIMx_CCMR2_INP_IC4PSC_POS; 
            }
            break;
        
        default:
            return RESULT_INVALID;
    }

    // Disable channel before changing mode.
    timer->TIMx_CCER &= ~TIMx_CCER_CCxE_MASK(ccxe_pos);

    if (configs.channel_direction == TIMER_CCx_CONFIG_OUTPUT) {
        *ccr_reg = configs.capture_compare_cfg.out.compare_value;
    }

    uint32_t ccmr_val = *ccmr_reg;
    ccmr_val &= ~TIMx_CCMRx_OUT_CCxS_MASK(ccxs_pos);
    ccmr_val |= TIMx_CCMRx_OUT_CCxS_SET(configs.channel_direction, ccxs_pos);

    if (configs.channel_direction == TIMER_CCx_CONFIG_OUTPUT) {
        ccmr_val &= ~TIMx_CCMRx_OUT_OCxM_MASK(input_output_cfg.output_cfg.ocxm_pos);
        ccmr_val |= TIMx_CCMRx_OUT_OCxM_SET(configs.capture_compare_cfg.out.compare_mode, input_output_cfg.output_cfg.ocxm_pos);

        // Output compare preload enable
        ccmr_val |= input_output_cfg.output_cfg.ocxpe_mask;
    }
    else {
        ccmr_val &= ~TIMx_CCMRx_INP_ICxPSC_MASK(input_output_cfg.input_cfg.icxpsc_pos);
        ccmr_val &= ~TIMx_CCMRx_INP_ICxF_MASK(input_output_cfg.input_cfg.icxf_pos);

        ccmr_val |= TIMx_CCMRx_INP_ICxPSC_SET(configs.capture_compare_cfg.inp.input_prescaler, input_output_cfg.input_cfg.icxpsc_pos);
        // Input capture filter - frequency used to sample TI1 input and length of the digital filter
        ccmr_val |= TIMx_CCMRx_INP_ICxF_SET(configs.capture_compare_cfg.inp.input_filter, input_output_cfg.input_cfg.icxf_pos);
    }
    
    *ccmr_reg = ccmr_val;

    // Enable selected output channel
    timer->TIMx_CCER |= TIMx_CCER_CCxE_MASK(ccxe_pos);
    return RESULT_SUCCESS;
}


/**
 * Function enables timer and configures channel for input/output.
 * Before calling this function, we must enable the corresponding peripheral clock.
 * GPIO configuration must be "alternate function output push-pull" for PWM output.
 * 
 * @param   id          Timer ID need to be configure
 * @param   channel     Timer channel need to be configure
 * @param   configs     gpio configuration need to be configure
 */
system_result_t general_timer_init(timer_id_t id, channel_config_req_t timer_configs, gpio_config_req_t gpio_configs) 
{
    apb1_enable_t enable_clk;
    switch (id)
    {
        case TIMER_TIM2:
            enable_clk = TIMER2_CLOCK_ENABLE;
            break;
        case TIMER_TIM3:
            enable_clk = TIMER3_CLOCK_ENABLE;
            break;
        case TIMER_TIM4:
            enable_clk = TIMER4_CLOCK_ENABLE;
            break;
        case TIMER_TIM5:
            enable_clk = TIMER5_CLOCK_ENABLE;
            break;
        default:
            return RESULT_INVALID; 
    }
    system_result_t result = APB1_enable_clock(enable_clk);
    if (result != RESULT_SUCCESS) {
        return result;
    }

    result = gpio_configure(gpio_configs);
    if (result != RESULT_SUCCESS) {
        return result;
    }
    
    uint32_t timer_base = timer_base_address_table[id];
    if (timer_base == 0U) {
        return RESULT_INVALID;
    }

    volatile TIMx_registers_t* timer = GENERAL_TIMER_REGISTERS(timer_base);

    // STM32 Timer frequency is 72 MHz, PWM frequency is 72 MHz / ((PSC + 1) * (ARR + 1)). So it is based on cpu frequency
    /*
        Timer frequency = timer clock / ((Prescaler + 1) * (Auto reload + 1))
        If timer clock is 72 MHz and PSC is 71 then timer counter clock is 1 MHz.
    */
    timer->TIMx_PSC = (SYSTEM_72MHZ_FREQUENCY / ONE_MHZ_FREQUENCY) - 1U;
    timer->TIMx_ARR = TIMER_INIT_AUTO_RELOAD_VAL;
    
    // Enable auto-reload enable
    timer->TIMx_CR1 |= TIMx_CR1_ARPE_MASK;

    result = timer_configure_channel(timer, timer_configs);
    if (result != RESULT_SUCCESS) {
        return result;
    }

    // Update event to load registers
    timer->TIMx_EGR = TIMx_EGR_UG_MASK;

    // Enable counter
    timer->TIMx_CR1 |= TIMx_CR1_CEN_MASK;

    return RESULT_SUCCESS;
}

system_result_t timer_set_channel_duty(timer_id_t timer_id, timer_channel_t channel, uint32_t duty)
{
    if (timer_id >= TIMER_COUNT) {
        return RESULT_INVALID;
    }

    uint32_t timer_base = timer_base_address_table[timer_id];
    if (timer_base == 0U) {
        return RESULT_INVALID;
    }

    volatile TIMx_registers_t *timer = GENERAL_TIMER_REGISTERS(timer_base);

    // Duty cycle should not be greater than Auto-reload register. because we need compare aginst it.
    if (duty > timer->TIMx_ARR) {
        duty = timer->TIMx_ARR;
    }

    switch (channel)
    {
        case TIMER_CHANNEL_1:
            timer->TIMx_CCR1 = duty;
            break;

        case TIMER_CHANNEL_2:
            timer->TIMx_CCR2 = duty;
            break;

        case TIMER_CHANNEL_3:
            timer->TIMx_CCR3 = duty;
            break;

        case TIMER_CHANNEL_4:
            timer->TIMx_CCR4 = duty;
            break;

        default:
            return RESULT_INVALID;
    }

    return RESULT_SUCCESS;
}