#ifndef GPIO_H
#define GPIO_H

#include "system.h"
#include "f103c6t6_regs.h"


typedef struct 
{
    volatile uint32_t CRL;     // Port configuration register low pins [0..7]. reset value: 0x44444444
    volatile uint32_t CRH;     // Port configuration register high pins [8..15]. reset value: 0x44444444
    volatile uint32_t IDR;     // Input data register
    volatile uint32_t ODR;     // Output data register
    volatile uint32_t BSRR;    // Bit set and reset register
    volatile uint32_t BRR;     // Bit reset register
    volatile uint32_t LCKR;    // Configuration lock register 
}   GPIOx_typedef;

#define GPIOx_config_regs(gpio_base)      ((GPIOx_typedef *) (gpio_base))

/*
    each pin in both CRL and CRH registers (low [0 to 7] high [8 to 15]) has configuraion bits cnf[1:0] to configure corresponding I/O port. 
    each pin in both CRL and CRH register (low [0 to 7] high [8 to 15]) has mode bits mode[1:0] to select input and output mode.
    each pin has 4 bits for this configuration.
*/
#define GPIOx_PIN_CFG_MASK(pin)         (0xFU << ((pin) << 2))
#define GPIOx_PIN_CFG_SET(cfg, pin)     (((uint32_t)(cfg) & 0xFU) << ((pin) << 2))                       

#define GPIO_CFG_LOW_REG_MAX_PIN        (8U)
#define GPIO_CFG_HIGH_REG_MAX_PIN       (16U)

#define GPIO_LED_ERROR_BLINK_MS         (7500U)

typedef enum {
    GPIO_PORT_A_ENABLE = (1 << 0),
    GPIO_PORT_B_ENABLE = (1 << 1),
    GPIO_PORT_C_ENABLE = (1 << 2),
    GPIO_ALL_ENABLE = (GPIO_PORT_A_ENABLE | GPIO_PORT_B_ENABLE | GPIO_PORT_C_ENABLE)
}   gpio_enable_t;

typedef enum {
    GPIO_INPUT,
    GPIO_OUTPUT_10MHz,
    GPIO_OUTPUT_2MHz,
    GPIO_OUTPUT_50MHz
} gpio_mode_t;

typedef enum {
    GPIO_INPUT_ANALOG = 0,
    GPIO_INPUT_FLOATING = 1,
    GPIO_INPUT_PULLUP_PULLDOWN = 2,
    GPIO_OUTPUT_PUSH_PULL = 0,
    GPIO_OUTPUT_OPEN_DRAIN = 1,
    AFO_OUTPUT_PUSH_PULL = 2,
    AFO_OUTPUT_OPEN_DRAIN = 3
}   port_config_t;

typedef enum {
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C
}   gpio_port_t;

typedef enum {
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15
}   gpio_num_t;

typedef struct 
{
    gpio_port_t port;
    gpio_num_t pin;
    port_config_t pin_config;
    gpio_mode_t mode;
}  gpio_config_req_t;


system_result_t gpio_configure(gpio_config_req_t cfg_req);
#endif