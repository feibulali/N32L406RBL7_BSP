#ifndef __DRV_LED_H__
#define __DRV_LED_H__

enum
{
    LED1 = 0,
    LED2,
    LED3,
    LED_MAX
};

void led_init(void);
void led_on(int id);
void led_off(int id);
void led_toggle(int id);

#endif
