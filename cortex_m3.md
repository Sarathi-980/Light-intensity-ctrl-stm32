# ARM Cortex M3 Processor


## Introduction

The ARM Cortex-M3 is a 32 bit processor core (Central Processing Unit), it was the first processor of the Cortex-M family, which is designed for embedded microcontroller systems.


> **Note:** ARM does not manufacture chips, it licences the design of the core to semiconductor companies who adds memory, peripherals to create a microcontrollers (MCU)

## Cortex Processor Profiles

| Profile   | Target Use                            |   Example              |
|-----------|---------------------------------------|------------------------|
|     A     | Application processors (OS, Phones)   | RK3568 (Cortex-A55)    |
|     R     | Real-time systems (automotive)        | TMS570 (Cortex-R4F)    |
|     M     | Microcontrollers (embedded)           | STM32F103 (Cortex-M3)  |

## Cortex M3 in MCU

- The Cortex M3 is 32 bit CPU core and its architecture is ARMv7-M.
- The Thumb-2 instruction set contains both 32 and 16 bit instructions. The Thumb-2 allows complex operations performed in Thumb state.
- It has separate instruction bus and data bus, thus instructions and data accesses can be take place at the same time.
- It has optional MPU and optional external cache. 
    
## Registers of CM3

| Register  |      Name                 |       Purpose                     |
|-----------|---------------------------|-----------------------------------|
| R0 - R7   | Low registers             | General purpose (data operations) |
| R8 - R12  | High registers            | General purpose                   |
| R13 (SP)  | Stack pointer             | Pointer to the top of the stack   |
| R14 (LR)  | Link Register             | Holds subroutine return address   |
| R15 (PC)  | Program counter           | Address of the current execution  |
| PSR       | Program status register   | Processor's current state         | 
| IMR       | Interrupt mask register   | Disable or restrict interrupts    |
| Control   | Control register          | selects processor privilege level | 

### R0 - R12: General purpose registers

- Used for data operations.
- R0 - R7 are low registers are accessible by all instructions.
- R8 - R12 are high registers are accessible by 32 bit instructions and only few 16 bit instructions (MOV, ADD, CMP).


### R13: Stack Pointer

R13 is **banked**: there are two physical stack pointers, but only one is visible at a time.

- **MSP** default after reset; used by os kernel and exception handlers
- **PSP** used by application tasks/threads

SP is always word aligned (multiple of 4) so last two bits are 0.

### R14: Link register

- when subroutine is called, the return address is stored in this register.
- During exception LR will hold the special value **EXC_RETURN**.

### R15: Program Counter:

- Holds the address of the current instruction.
- Reading this register will give current instruction + 4 due to pipeline.
- Writing this register changes the program flow (acts like a branch). 

### Program Status Registers:

- Provides arithmetic and logic processing flags (zero or carry), execution status and  current executing interrupt number.
- The PSR subdivided into three status registers
    - Application Program Status Register (APSR)
    - Interrupt Program Status Register (IPSR)
    - Execution Program Status Register (EPSR)
Here only APSR part is writable others are readable only.

| xPSR bit index | bit | Bit field status |
|----------------|-----|------------------|
| 31 | N | Negative flag when bit 31 is 1 |
| 30 | Z | Zero flag instruction has zero value or equal result |
| 29 | C | Carry flag Unsigned data processing status |
| 28 | V | Overflow flag Signed data processing status |
| 27 | Q | Sticky saturation flag, Triggered by saturating instructions |
| 26 : 25, 15 : 10 | ICI/IT | Interrupt continuable Instruction bits, If-Then instruction status bit |
| 24 | T | Thumb state |
| 9 | - | Reserved (in stacked xPSR, it shows stack alignment padding) |
| 8 : 0 | Exception number | Indicates the exception the processor handling |

### Interrupt Mask Registers:

- PRIMASK: Disable all interrupts except NMI (Non Maskable Interrupt) and hard fault.
- FAULTMASK: Disable all interrupts except NMI
- BASEPRI: Disable all interrupts of specific priority level or lower priority level

### Control Register:

- Defines processor privilege level and stack pointer selection. This register has 2 bits.
- Control[1] : Indicate stack status
    - 0 - Main Stack Pointer is used
    - 1 - Process Stack Pointer is used
- Control[0] : Indicates privileged level of the thread
    - 0 - Privileged state in thread mode
    - 1 - User state in thread mode

## Nested Vectored Interrupt Controller(NVIC):

- The Cortex-M3 processor includes an interrupt controller called **NVIC**, which is closely coupled to the processor core. It provides  
  - Nested Interrupt support (Interrupts can be in different priority level)
  - Vectored Interrupt support (Interrupt Service Routine address stored in vector table)
  - Dynamic priority changes (Priority can be changed at runtime)
  - Reduced interrupt latency 
  - Interrupt masking (Masked using interrupt masking registers)

- The Cortex-M3 has **predefined memory map**, which allows built-in peripherals to be accessed by simple instructions.

## Exception types in Cortex-M3

- Interrupts are generated by peripherals and systick timer also connected to the interrupt input signals.
- The number of the interrupts based on microcontroller design.

| Exception number | Exception type | Priority | Function |
|------------------|----------------|----------|----------|
| 1 | Reset | -3 | Resetting the CPU and peripherals |
| 2 | NMI | -2 | Nonmaskable Interrupt (guaranteed execution) |
| 3 | Hard fault | -1 | All classes of fault, when corresponding fault is disabled or masked|
| 4 | MemManage | settable | Caused by MPU violation or invalid access |
| 5 | Bus fault | settable | Caused by instruction prefetch abort or data access error |
| 6 | Usage fault | settable | Caused by invalid instructions like trying ARM instruction here |
| 11 | SVC | settable | Supervisor call via SVC instruction |
| 12 | Debug monitor | settable | Debug monitor |
| 14 | PendSV | settable | Pendable request for system service | 
| 15 | SYSTICK | settable | System tick timer interrupt |
| 16 - 255 | IRQ | settable | IRQ inputs |

## Interrupt handling features

- Vector table based interrupt, so no need of software to set up nested interrupt support.
- The Cortex-M3 processor automatically pushes registers R0-R3, R12, LR, PSR and PC in the stack at interrupt entry and pops them back at interrupt exit.
- PUSH, POP, LDM, STM are interruptable.

## Bus Interface

- Code memory buses
- System bus
- Private peripheral bus
    
- The code memory buses has I-Code and D-Code buses. This allows carry instruction fetches and data accesses at the same time.
- The system bus is used to access memory and peripherals. It provides access to SRAM, peripherals, external RAM and part of system level region.
- The private peripheral bus provides access to a part of system level memory dedicated to private memory peripherals such as debugging components. 

## Stack

- The Cortex-M3 uses full descending stack, means SP decremented when PUSH operation done.
- Stack operations are memory read and write, with the address specified by an Stack Pointer.
- PUSH operation is data in registers saved into stack memory
- POP operation is data can be restored to registers from stack
- SP automatically adjusted so multiple loading won't cause old data erased.
- For each PUSH there must be a POP operation, the **address of the POP operation should match the PUSH operation**.


## Reset Sequence

- After processor exits from RESET, it will read two words,
    1. Address 0x00000000 : It is starting value of R13 (SP)
    2. Address 0x00000004 : it is **Reset vector to boot code or Reset_handler function.** (bit 0 of this address must be 1 for Thumb state)

## Instruction Sets

- In assembler code, instruction would be
    - label
        opcode operand1, operand2,... ; comments

    - here opcode is the instruction. mostly the first operand will be destination. following operands are depends on instruction.

## Memory systems 

- The Cortex-M3 has total 4 GB of address space.
- Memory attributes in the Cortex-M3 are bufferable, cacheable, executable and sharable. 
  - Code memory region: This region is executable and write transfers are bufferable.
  - SRAM region: This region is on-chip RAM, it is also executable and bufferable.
  - Peripheral region: it is not executable and non cacheable.
  - System region: This region is private peripherals and vendor specific. The vendor specific is bufferable but PPB is not.

## Default memory permissions

- Vendor specific has full access
- ROM user access is bus fault
- PPB user access is bus fault
- NVIC region user access is bus fault except **Software trigger interrupt register**, which can be configured to allow user access (USERSETMPEND bit in CCR).
- Code has full user access
- SRAM has full user access
- Peripheral has full user access

## Bit Band regions

- Bit band regions allow single load or store operation to access to a single data bit. 
- The 1 MB bit band region is in the SRAM and other 1 MB is located in the peripheral region.
- In this region, for writing one bit, instead of using three instructions (read, modify, write), here we can do the task by one instruction.
- Bit band alias will cause access to the bit band region.

- 0x20000000–0x200FFFFF (SRAM, 1 MB)
- 0x40000000–0x400FFFFF (peripherals, 1 MB)

- Bit band alias regions (32 MB each):
  - 0x22000000–0x23FFFFFF (SRAM alias)
  - 0x42000000–0x43FFFFFF (peripheral alias)
- alias address = alias base + (byte offset × 32) + (bit number × 4)

- If an unaligned access is carried out to bit-band alias address range, the result is unpredictable. 
- It is **atomic** (read, modify, write done in one operation, cannot be interrupted)

## Pipeline

- The Cortex-M3 has three stage pipeline, which are instruction fetch, instruction decode and execution.
- Processor fetches 32 bits at a time, which can hold two 16 bit instructions, so processor not fetch every cycle.
- Because of the above pipeline, the read value of PC is address of current instruction plus 4. Now this +4 is a fixed rule kept for Thumb compatibility.

## Interrupt Pending

- The total number of interrupts defined by MCU design, 1 - 15 are system exceptions. 
- The value of the current running exception is indicated by special interrupt program status register or VECTACTIVE field in Interrupt control register. 
- The preempt priority level defines whether interrupt can take place when processor is already running another ISR. The subpriority level is comparison against two same preempt priority level.
- In application interrupt and reset control register, the priority group controls preempt priority and subpriority levels.
- The exception handler functions are stored in vector table, it is located at memory address 0, and it can be relocated at runtime. 
- When interrupt is asserted, it will be pended to processor to process the request, in case interrupt deasserted before processor services it, the pending status still stays, so processor will execute the ISR.

## Bus Fault

- To enable this fault we need to enable BUSFAULTENA bit in System Handler Control and State Register (SHCSR).
- When error response received from a transfer on AHB interfaces. 
    - Instruction fetch error
    - Data read/write error
    - Stack push error at interrupt starting (STKERR)
    - Stack pop error at interrupt exiting (UNSTKERR)
- Vector fetch error at interrupt starting goes to hard fault.
- We can see the status of this error in Bus Fault Status Register.

## Memory Management Fault

- To enable this fault we need to enable MEMFAULTENA bit in SHCSR.
- When memory access rules violated this fault will occur.
    - accessing memory region not defined in MPU setup
    - writing to read-only regions
    - access in user state to privileged region
    - executing code from XN region (ex. peripheral region), even without MPU
- We can see the status of this fault in Memory management fault status register.

## Usage fault
    
- Usage fault can be caused by
    - Undefined instructions
    - Trying ARM instructions
    - Unaligned memory access in multiple load or store
    - Invalid EXC_RETURN value
    - Divide by zero and unaligned access (when enabled in CCR)

- If the above faults (bus, memory management, usage) not handled or not enabled, then hard fault handler will be executed.

## Supervisor Call (SVC)

- Supervisor call and Pendable supervisor call are targeted for software and operating system use. 
- It allows user program to access the hardware through SVC (not directly, the OS accesses the hardware on behalf of user program).
- When user wants to use hardware, it generates SVC exception using SVC instructions, then the Operating system will execute the software exception handler and provides the service user application requested.
- The immediate value required for this SVC instruction is the service number (8 bit), other parameters passed in R0 - R3.
- We **cannot use SVC inside an SVC handler, hard fault handler and NMI handler** because SVC priority is not higher than current priority (same in SVC handler, lower in hard fault and NMI). It will result hard fault (lockup in hard fault and NMI handler).

## Pendable Service Call (PendSV)

- It is pendable service call works with SVC in the OS. ex. Context Switching.
- SVC must run now, but PendSV can wait until no higher priority handler is running.
- It is triggered by setting PENDSVSET bit (bit 28) in Interrupt Control and State Register (ICSR, 0xE000ED04), not by an instruction.
- Mostly PendSV is set to lowest priority, so it runs after all other interrupt handlers finished.
- In OS, SysTick handler decides task switch is needed and only sets PendSV pending. If SysTick switches task directly while another ISR is half done, it will make problem.
- PendSV handler does the context switching:
    - hardware already pushed R0-R3, R12, LR, PC and xPSR to current task stack (PSP)
    - PendSV handler pushes R4-R11, saves PSP of current task
    - loads PSP of next task and pops its R4-R11
    - on return, hardware pops the rest, next task runs

| | SVC | PendSV |
|---|---|---|
| Triggered by | SVC instruction | PENDSVSET bit in ICSR |
| Timing | Must run now | Can wait (pending) |
| If it cannot run now | Fault | Stays pending, runs later |
| Typical use | OS services, starting first task | Context switching |
