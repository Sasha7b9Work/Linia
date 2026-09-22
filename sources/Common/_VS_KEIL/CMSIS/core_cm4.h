#pragma once

#define __CC_ARM

#ifndef __IO
#define __IO volatile
#endif

#include <stdint.h>

#undef __ASM
#define __ASM()


#define __FPU_USED 0

#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */

struct SysTickStruct
{
    __IO uint32_t CTRL;   /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
    __IO uint32_t LOAD;   /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
    __IO uint32_t VAL;    /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
    __IO uint32_t CALIB;  /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
};

#define SysTick    ((SysTickStruct *)0)

#define NVIC_SetPriority(x, y)

typedef struct
{
    __IO uint32_t ISER[8U];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
    uint32_t RESERVED0[24U];
    __IO uint32_t ICER[8U];               /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
    uint32_t RESERVED1[24U];
    __IO uint32_t ISPR[8U];               /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
    uint32_t RESERVED2[24U];
    __IO uint32_t ICPR[8U];               /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
    uint32_t RESERVED3[24U];
    __IO uint32_t IABR[8U];               /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
    uint32_t RESERVED4[56U];
    __IO uint8_t  IP[240U];               /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
    uint32_t RESERVED5[644U];
    __IO  uint32_t STIR;                   /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
}  NVIC_Type;

#define NVIC_BASE           (SCS_BASE +  0x0100UL)                    /*!< NVIC Base Address */

#define NVIC   ((NVIC_Type      *)     NVIC_BASE     )   /*!< NVIC configuration struct */


extern struct structSCB
{
    unsigned VTOR;
} *SCB;


#define __disable_irq()
#define __set_MSP(x)
#define __enable_irq()
