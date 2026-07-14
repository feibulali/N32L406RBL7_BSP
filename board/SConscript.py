# RT-Thread BSP SConscript for N32L406RBL7 Board

import os

# 当前目录路径
cwd = GetCurrentDir()

# 本目录需要编译的源文件
src = [
    'board.c',
    'pin.c',
    'drv_gpio.c',
    'drv_usart.c',
    'drv_led.c',
    'drv_key.c',
]

# 定义一个编译组（RT-Thread 标准写法）
group = DefineGroup(
    name        = 'Board',
    src         = src,
    depend      = ['RT_USING_DEVICE'],
    CPPPATH     = [cwd],
)

Return('group')
