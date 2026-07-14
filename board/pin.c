#include <rtthread.h>
#include <rtdevice.h>
#include "n32l40x.h"

struct pin_index
{
    uint8_t index;
    GPIO_TypeDef *gpio;
    uint16_t pin;
};

#define __N32_PIN(index, gpio, pin) {index, gpio, pin}

static const struct pin_index pin_map[] =
{
   
 __N32_PIN(0, GPIOA, GPIO_Pin_0),
    __N32_PIN(1, GPIOA, GPIO_Pin_1),
    __N32_PIN(2, GPIOA, GPIO_Pin_2),
    __N32_PIN(3, GPIOA, GPIO_Pin_3),
    __N32_PIN(4, GPIOA, GPIO_Pin_4),
    __N32_PIN(5, GPIOA, GPIO_Pin_5),
    __N32_PIN(6, GPIOA, GPIO_Pin_6),
    __N32_PIN(7, GPIOA, GPIO_Pin_7),
    __N32_PIN(8, GPIOA, GPIO_Pin_8),
    __N32_PIN(9, GPIOA, GPIO_Pin_9),
    __N32_PIN(10, GPIOA, GPIO_Pin_10),
    __N32_PIN(11, GPIOA, GPIO_Pin_11),
    __N32_PIN(12, GPIOA, GPIO_Pin_12),
    __N32_PIN(13, GPIOA, GPIO_Pin_13),
    __N32_PIN(14, GPIOA, GPIO_Pin_14),
    __N32_PIN(15, GPIOA, GPIO_Pin_15),


 __N32_PIN(16, GPIOB, GPIO_Pin_0),
    __N32_PIN(17, GPIOB, GPIO_Pin_1),
    __N32_PIN(18, GPIOB, GPIO_Pin_2),
    __N32_PIN(19, GPIOB, GPIO_Pin_3),
    __N32_PIN(20, GPIOB, GPIO_Pin_4),
    __N32_PIN(21, GPIOB, GPIO_Pin_5),
    __N32_PIN(22, GPIOB, GPIO_Pin_6),
    __N32_PIN(23, GPIOB, GPIO_Pin_7),
    __N32_PIN(24, GPIOB, GPIO_Pin_8),
    __N32_PIN(25, GPIOB, GPIO_Pin_9),
    __N32_PIN(26, GPIOB, GPIO_Pin_10),
    __N32_PIN(27, GPIOB, GPIO_Pin_11),
    __N32_PIN(28, GPIOB, GPIO_Pin_12),
    __N32_PIN(29, GPIOB, GPIO_Pin_13),
    __N32_PIN(30, GPIOB, GPIO_Pin_14),
    __N32_PIN(31, GPIOB, GPIO_Pin_15),

  __N32_PIN(32, GPIOC, GPIO_Pin_0),
    __N32_PIN(33, GPIOC, GPIO_Pin_1),
    __N32_PIN(34, GPIOC, GPIO_Pin_2),
    __N32_PIN(35, GPIOC, GPIO_Pin_3),
    __N32_PIN(36, GPIOC, GPIO_Pin_4),
    __N32_PIN(37, GPIOC, GPIO_Pin_5),
    __N32_PIN(38, GPIOC, GPIO_Pin_6),
    __N32_PIN(39, GPIOC, GPIO_Pin_7),
    __N32_PIN(40, GPIOC, GPIO_Pin_8),
    __N32_PIN(41, GPIOC, GPIO_Pin_9),
    __N32_PIN(42, GPIOC, GPIO_Pin_10),
    __N32_PIN(43, GPIOC, GPIO_Pin_11),
    __N32_PIN(44, GPIOC, GPIO_Pin_12),
    __N32_PIN(45, GPIOC, GPIO_Pin_13),
    __N32_PIN(46, GPIOC, GPIO_Pin_14),
    __N32_PIN(47, GPIOC, GPIO_Pin_15),

 __N32_PIN(48, GPIOD, GPIO_Pin_0),
    __N32_PIN(49, GPIOD, GPIO_Pin_1),
    __N32_PIN(50, GPIOD, GPIO_Pin_2),
    __N32_PIN(51, GPIOD, GPIO_Pin_3),
    __N32_PIN(52, GPIOD, GPIO_Pin_4),
    __N32_PIN(53, GPIOD, GPIO_Pin_5),
    __N32_PIN(54, GPIOD, GPIO_Pin_6),
    __N32_PIN(55, GPIOD, GPIO_Pin_7),
    __N32_PIN(56, GPIOD, GPIO_Pin_8),
    __N32_PIN(57, GPIOD, GPIO_Pin_9),
    __N32_PIN(58, GPIOD, GPIO_Pin_10),
    __N32_PIN(59, GPIOD, GPIO_Pin_11),
    __N32_PIN(60, GPIOD, GPIO_Pin_12),
    __N32_PIN(61, GPIOD, GPIO_Pin_13),
    __N32_PIN(62, GPIOD, GPIO_Pin_14),
    __N32_PIN(63, GPIOD, GPIO_Pin_15),

/* 我们编写了 64pin 的全部映射 */
};

static void n32_pin_write(rt_device_t dev, rt_base_t pin, rt_base_t value)
{
    const struct pin_index *index = &pin_map[pin];
    if (value)
        GPIO_SetBits(index->gpio, index->pin);
    else
        GPIO_ResetBits(index->gpio, index->pin);
}

static int n32_pin_read(rt_device_t dev, rt_base_t pin)
{
    const struct pin_index *index = &pin_map[pin];
    return GPIO_ReadInputDataBit(index->gpio, index->pin);
}

static void n32_pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    GPIO_InitType GPIO_InitStructure;
    const struct pin_index *index = &pin_map[pin];

    GPIO_InitStructure.Pin = index->pin;

    if (mode == PIN_MODE_OUTPUT)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    }
    else if (mode == PIN_MODE_INPUT)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    }

    GPIO_Init(index->gpio, &GPIO_InitStructure);
}

static const struct rt_pin_ops n32_pin_ops =
{
    n32_pin_mode,
    n32_pin_write,
    n32_pin_read,
};

int rt_hw_pin_init(void)
{
    return rt_device_pin_register("pin", &n32_pin_ops, RT_NULL);
}
INIT_BOARD_EXPORT(rt_hw_pin_init);
