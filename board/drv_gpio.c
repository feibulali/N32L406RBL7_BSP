#include <rtthread.h>
#include "n32l40x.h"

/* ============================================================
 * GPIO 时钟使能（自动根据端口选择）
 * ============================================================ */
static void gpio_clock_enable(GPIO_TypeDef *gpio)
{
    if (gpio == GPIOA)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (gpio == GPIOB)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (gpio == GPIOC)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if (gpio == GPIOD)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
}

/* ============================================================
 * GPIO 模式配置（由 pin.c 调用）
 * ============================================================ */
void drv_gpio_mode(GPIO_TypeDef *gpio, uint16_t pin, uint32_t mode)
{
    GPIO_InitType GPIO_InitStructure;

    gpio_clock_enable(gpio);

    GPIO_InitStructure.Pin = pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    switch (mode)
    {
    case PIN_MODE_OUTPUT:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        break;

    case PIN_MODE_INPUT:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        break;

    case PIN_MODE_INPUT_PULLUP:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        break;

    case PIN_MODE_INPUT_PULLDOWN:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
        break;

    default:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        break;
    }

    GPIO_Init(gpio, &GPIO_InitStructure);
}

/* ============================================================
 * GPIO 写入（由 pin.c 调用）
 * ============================================================ */
void drv_gpio_write(GPIO_TypeDef *gpio, uint16_t pin, rt_base_t value)
{
    if (value)
        GPIO_SetBits(gpio, pin);
    else
        GPIO_ResetBits(gpio, pin);
}

/* ============================================================
 * GPIO 读取（由 pin.c 调用）
 * ============================================================ */
rt_base_t drv_gpio_read(GPIO_TypeDef *gpio, uint16_t pin)
{
    return GPIO_ReadInputDataBit(gpio, pin);
}
