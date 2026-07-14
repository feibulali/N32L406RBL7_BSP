#include <rthw.h>
#include <rtthread.h>
#include "board.h"
#include "n32l40x.h"

/* ==================== 时钟配置（基于 8MHz HSE → 64MHz） ==================== */
void SystemClock_Config(void)
{
    /* 复位 RCC 配置 */
    RCC_DeInit();

    /* 1. 启用 HSE (8MHz) */
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_WaitForHSEStartUp() != SUCCESS);

    /* 2. 配置 PLL: HSE * 8 = 64MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_8);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* 3. 系统时钟切换到 PLL */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08);

    /* 4. 总线分频 */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);   /* AHB = 64MHz */
    RCC_PCLK1Config(RCC_HCLK_Div2);    /* APB1 = 32MHz */
    RCC_PCLK2Config(RCC_HCLK_Div1);    /* APB2 = 64MHz */

    /* 5. 更新 SystemCoreClock 变量 */
    SystemCoreClockUpdate();

    /* 6. GPIO 时钟使能（★ 必须） */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA |
        RCC_APB2Periph_GPIOB |
        RCC_APB2Periph_GPIOC |
        RCC_APB2Periph_GPIOD,
        ENABLE);
}

/* ==================== NVIC 配置 ==================== */
void NVIC_Configuration(void)
{
#ifdef VECT_TAB_RAM
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
}

/* ==================== SysTick → RT-Thread Tick ==================== */
void SysTick_Handler(void)
{
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
}

/* ==================== Board 初始化 ==================== */
void rt_hw_board_init(void)
{
    /* 1. 时钟配置 */
    SystemClock_Config();

    /* 2. NVIC */
    NVIC_Configuration();

    /* 3. SysTick 初始化 */
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    /* 4. Heap 初始化 */
#ifdef BSP_USING_SRAM
    rt_system_heap_init((void *)EXT_SRAM_BEGIN, (void *)EXT_SRAM_END);
#else
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif

    /* 5. 初始化 pin 设备（★ 必须） */
    rt_hw_pin_init();

    /* 6. 组件初始化（自动初始化驱动框架） */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

    /* 7. Console 设备设置（默认 USART1: PA9/PA10） */
#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
}
