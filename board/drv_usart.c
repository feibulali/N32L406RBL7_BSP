#include <rtthread.h>
#include <rtdevice.h>
#include "n32l40x.h"
#include "board.h"

/* ============================================================
 * UART1 配置：PA9 (TX), PA10 (RX)
 * ============================================================ */
static rt_err_t uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    USART_InitType USART_InitStructure;

    /* 打开 UART 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* 打开 GPIO 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* PA9 = TX 推挽输出 */
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* PA10 = RX 浮空输入 */
    GPIO_InitStructure.Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* UART 参数配置 */
    USART_InitStructure.BaudRate = cfg->baud_rate;
    USART_InitStructure.WordLength = USART_WordLength_8b;
    USART_InitStructure.StopBits = USART_StopBits_1;
    USART_InitStructure.Parity = USART_Parity_No;
    USART_InitStructure.HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

    return RT_EOK;
}

/* ============================================================
 * UART 写入（发送）
 * ============================================================ */
static rt_err_t uart_putc(struct rt_serial_device *serial, char c)
{
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return RT_EOK;
}

/* ============================================================
 * UART 读取（接收）
 * ============================================================ */
static int uart_getc(struct rt_serial_device *serial)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
    {
        return USART_ReceiveData(USART1);
    }
    return -1;
}

/* ============================================================
 * UART 操作表
 * ============================================================ */
static const struct rt_uart_ops uart_ops =
{
    uart_configure,
    uart_putc,
    uart_getc,
};

/* ============================================================
 * UART1 设备对象
 * ============================================================ */
static struct rt_serial_device serial1;

/* ============================================================
 * UART 驱动初始化
 * ============================================================ */
int rt_hw_usart_init(void)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    serial1.ops    = &uart_ops;
    serial1.config = config;

    /* 注册 UART1 设备 */
    rt_hw_serial_register(&serial1, "uart1",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                          RT_NULL);

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_usart_init);
