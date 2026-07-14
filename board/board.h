#ifndef __BOARD_H__
#define __BOARD_H__

#include "n32l40x.h"
#include <rtthread.h>

#define N32L406RBL7  


/* ============================================================
 * 1. MCU 内存配置（N32L406RBL7 = 32KB SRAM）
 * ============================================================ */
#define N32_SRAM_SIZE        (32 * 1024)
#define N32_SRAM_END         (0x20000000 + N32_SRAM_SIZE)

/* RT-Thread Heap 区域（使用内部 SRAM） */
#define HEAP_BEGIN           (void*)0x20000000
#define HEAP_END             (void*)N32_SRAM_END

/* ============================================================
 * 2. GPIO 引脚宏（RT-Thread 标准 GET_PIN）
 * ============================================================ */
#ifndef GET_PIN
#define GET_PIN(PORTx, PIN)  ( ( (PORTx) - 'A' ) * 16 + (PIN) )
#endif

/* ============================================================
 * 3. Console 默认串口（可随时修改）
 *    默认使用 USART1：PA9 / PA10
 * ============================================================ */
#define RT_CONSOLE_DEVICE_NAME  "uart1"

/* ============================================================
 * 4. 板级初始化函数声明
 * ============================================================ */
void SystemClock_Config(void);
void NVIC_Configuration(void);
void rt_hw_board_init(void);

#endif /* __BOARD_H__ */
