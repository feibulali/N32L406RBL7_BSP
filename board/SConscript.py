# RT-Thread BSP SConscript for N32L406RBL7 Board

import os

# 当前目录
cwd = GetCurrentDir()

# 源文件列表（根据你的 board/ 目录）
src = [
    'board.c',
    'pin.c',
    'drv_gpio.c',
    'drv_usart.c',
    'drv_led.c',
    'drv_key.c',
]

# 添加到编译系统
group = DefineGroup(
    name        = 'Board',
    src         = src,
    depend      = ['RT_USING_DEVICE'],
    CPPPATH     = [cwd],
)

Return('group')
