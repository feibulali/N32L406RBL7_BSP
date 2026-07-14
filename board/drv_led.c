#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "drv_led.h"

/* LED 引脚表 */
static const rt_base_t led_table[] =
{
    GET_PIN(A, 8),   /* LED1 */
    GET_PIN(B, 4),   /* LED2 */
    GET_PIN(B, 5),   /* LED3 */
};

/* LED 初始化 */
void led_init(void)
{
    for (int i = 0; i < LED_MAX; i++)
    {
        rt_pin_mode(led_table[i], PIN_MODE_OUTPUT);
        rt_pin_write(led_table[i], PIN_LOW);
    }
}

/* LED 亮 */
void led_on(int id)
{
    if (id < LED_MAX)
        rt_pin_write(led_table[id], PIN_HIGH);
}

/* LED 灭 */
void led_off(int id)
{
    if (id < LED_MAX)
        rt_pin_write(led_table[id], PIN_LOW);
}

/* LED 翻转 */
void led_toggle(int id)
{
    if (id < LED_MAX)
    {
        int val = rt_pin_read(led_table[id]);
        rt_pin_write(led_table[id], !val);
    }
}
