#include <rtthread.h>
#include <rtdevice.h>
#include "n32l40x.h"

/* ============================================================
 * pin 映射结构体（★ N32L40x 必须使用 GPIO_Module）
 * ============================================================ */
struct pin_index
{
    uint8_t index;
    GPIO_Module *gpio;     /* ★ 修复：必须使用 GPIO_Module */
    uint16_t pin;
};

/* ============================================================
 * pin 映射表（适配 N32L406RBL7 LQFP64）
 * ============================================================ */
#define __N32_PIN(index, gpio, pin) {index, gpio, pin}

static const struct pin_index pin_map[] =
{
    /* GPIOA: PA0 ~ PA15 */
    __N32_PIN(0,  GPIOA, GPIO_Pin_0),
    __N32_PIN(1,  GPIOA, GPIO_Pin_1),
    __N32_PIN(2,  GPIOA, GPIO_Pin_2),
    __N32_PIN(3,  GPIOA, GPIO_Pin_3),
    __N32_PIN(4,  GPIOA, GPIO_Pin_4),
    __N32_PIN(5,  GPIOA, GPIO_Pin_5),
    __N32_PIN(6,  GPIOA, GPIO_Pin_6),
    __N32_PIN(7,  GPIOA, GPIO_Pin_7),
    __N32_PIN(8,  GPIOA, GPIO_Pin_8),
    __N32_PIN(9,  GPIOA, GPIO_Pin_9),
    __N32_PIN(10, GPIOA, GPIO_Pin_10),
    __N32_PIN(11, GPIOA, GPIO_Pin_11),
    __N32_PIN(12, GPIOA, GPIO_Pin_12),
    __N32_PIN(13, GPIOA, GPIO_Pin_13),
    __N32_PIN(14, GPIOA, GPIO_Pin_14),
    __N32_PIN(15, GPIOA, GPIO_Pin_15),

    /* GPIOB: PB0 ~ PB15 */
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

    /* GPIOC: PC0 ~ PC15 */
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

    /* GPIOD: PD0 ~ PD15 */
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
};

/* ============================================================
 * pin 越界检查
 * ============================================================ */
static const struct pin_index* get_pin(rt_base_t pin)
{
    if (pin < 0 || pin >= sizeof(pin_map) / sizeof(pin_map[0]))
        return RT_NULL;
    return &pin_map[pin];
}

/* ============================================================
 * pin 写入
 * ============================================================ */
static void n32_pin_write(rt_device_t dev, rt_base_t pin, rt_base_t value)
{
    const struct pin_index *index = get_pin(pin);
    if (!index) return;

    if (value)
        GPIO_SetBits(index->gpio, index->pin);
    else
        GPIO_ResetBits(index->gpio, index->pin);
}

/* ============================================================
 * pin 读取
 * ============================================================ */
static int n32_pin_read(rt_device_t dev, rt_base_t pin)
{
    const struct pin_index *index = get_pin(pin);
    if (!index) return -1;

    return GPIO_ReadInputDataBit(index->gpio, index->pin);
}

/* ============================================================
 * pin 模式配置（★ 使用 N32 的 GPIO_InitType）
 * ============================================================ */
static void n32_pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    const struct pin_index *index = get_pin(pin);
    if (!index) return;

    GPIO_InitType GPIO_InitStructure;   /* ★ 修复：必须使用 GPIO_InitType */

    GPIO_InitStructure.Pin = index->pin;
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

    GPIO_Init(index->gpio, &GPIO_InitStructure);
}

/* ============================================================
 * RT-Thread pin 操作表
 * ============================================================ */
static const struct rt_pin_ops n32_pin_ops =
{
    n32_pin_mode,
    n32_pin_write,
    n32_pin_read,
};

/* ============================================================
 * pin 驱动注册
 * ============================================================ */
int rt_hw_pin_init(void)
{
    return rt_device_pin_register("pin", &n32_pin_ops, RT_NULL);
}
INIT_BOARD_EXPORT(rt_hw_pin_init);
