#include "gpio.h"

/*=================================================================================

@param enable - It is a bitmask indicating which GPIO ports should be enabled.
@return     RESULT_SUCCESS  -   Ports are initialized successfully.
===================================================================================*/
system_result_t gpio_configure(gpio_config_req_t cfg_req) 
{
    if (cfg_req.pin > GPIO_PIN_15) {
        return RESULT_INVALID;
    }

    uint32_t port_reg;
    switch (cfg_req.port)
    {
        case GPIO_PORT_A:
        {
            port_reg = GPIOA_BASE;
            break;
        }
        case GPIO_PORT_B:
        {
            port_reg = GPIOB_BASE;
            break;
        }
        case GPIO_PORT_C:
        {
            port_reg = GPIOC_BASE;
            break;
        }
        default:
            return RESULT_INVALID;
    }
  
    // Low pins config register offset is 0x0, High pins config register offset is 0x4
    volatile uint32_t *config_reg; 
    if (cfg_req.pin < GPIO_CFG_LOW_REG_MAX_PIN) {
        config_reg = &GPIOx_config_regs(port_reg)->CRL; 
    }
    else {
        config_reg = &GPIOx_config_regs(port_reg)->CRH;
        cfg_req.pin -= GPIO_CFG_LOW_REG_MAX_PIN;
    }
 
    *config_reg = ((*config_reg & ~GPIOx_PIN_CFG_MASK(cfg_req.pin)) | 
                        GPIOx_PIN_CFG_SET(((cfg_req.pin_config << 2) | cfg_req.mode), cfg_req.pin));
    return RESULT_SUCCESS;
}


