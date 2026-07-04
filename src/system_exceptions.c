#include "system_exceptions.h" 

// external ISR will change the tick count, so volatile is required.
volatile static uint32_t system_tick_count;   
volatile uint32_t seconds;

void Default_Handler() {
    while (1);
}

// NMI (Non-Maskable Interrupt) is handled by this ISR (Interrupt Service Routine).
// It cannot be disabled by processor, high priority next to Reset Interrupt.
// for example CSS(Clock Security System) triggers an NMI, when HSE is not stable.
void NMI_handler(void) {
    while (1);
}

// Hard fault exception occurs any fatal error happens in chip like illegal memory access
// Instruction execution error, try to switch to ARM instruction (in cortex M series arm instructions not supported)
void HardFault_handler(void) {
    while (1);    
}

void MemoryManage_handler() {
    while (1);
}
 
void BusFault_handler() {
    while (1);
}
 
void UsageFault_handler() { 
}
 
void SVC_handler() { 
}
 
void DebugMonitor_handler() { 
}
 
void PendSVC_handler() { 
}

void system_tick_handler() {
    system_tick_count++;
    if (system_tick_count >= 100) {
        seconds++;
        system_tick_count = 0;
    }
}
 
void default_exception_handler() { 
}

uint32_t get_systick_count() {
    return system_tick_count;
}

uint32_t get_seconds_passed() {
    return seconds;
}

void set_seconds(uint32_t value) {
    seconds = value;
}