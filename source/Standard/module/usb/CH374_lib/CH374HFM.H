/* 06.07.20 V1.0 for CH374, 06.08.15 V1.1, 06.09.21 V1.2, 07.05.25 V1.3, 07.09.20 V1.4, 08.01.14 V1.5, 08.08.26 V1.6
****************************************
**  Copyright  (C)  W.ch  1999-2006   **
**  Web:  http://www.winchiphead.com  **
****************************************
**  USB Host File Interface for CH374 **
**  TC2.0@PC, KEIL_C_3.11@ARM         **
****************************************
*/
/* CH374 主机文件系统接口 */
/* 支持: FAT12/FAT16/FAT32 */
/* ARM单片机, Cortex-M3内核指令, 小端数据格式, 查询方式或者中断方式 */

#ifndef __CH374HFM_H__
#define __CH374HFM_H__

#define CH374_LIB_VER		0x16

#ifdef __cplusplus
extern "C" {
#endif

/* FILE: CH374INC.H */

/* ********************************************************************************************************************* */
/* USB协议层定义 */

/* USB的包标识PID, 主机方式可能用到 */
#ifndef	DEF_USB_PID_SETUP
#define	DEF_USB_PID_NULL	0x00			/* 保留PID, 未定义 */
#define	DEF_USB_PID_SOF		0x05
#define	DEF_USB_PID_SETUP	0x0D
#define	DEF_USB_PID_IN		0x09
#define	DEF_USB_PID_OUT		0x01
#define	DEF_USB_PID_ACK		0x02
#define	DEF_USB_PID_NAK		0x0A
#define	DEF_USB_PID_STALL	0x0E
#define	DEF_USB_PID_DATA0	0x03
#define	DEF_USB_PID_DATA1	0x0B
#define	DEF_USB_PID_PRE		0x0C
#endif

/* ********************************************************************************************************************* */
/* 芯片定义 */

/* 寄存器及缓冲区 */

#define	RAM_ENDP0_SIZE		0x08		/* 端点0的缓冲区长度 */
#define	RAM_ENDP0_TRAN		0x20		/* 端点0发送缓冲区的起始地址 */
#define	RAM_ENDP0_RECV		0x28		/* 端点0接收缓冲区的起始地址 */
#define	RAM_ENDP1_SIZE		0x08		/* 端点1的缓冲区长度 */
#define	RAM_ENDP1_TRAN		0x30		/* 端点1发送缓冲区的起始地址 */
#define	RAM_ENDP1_RECV		0x38		/* 端点1接收缓冲区的起始地址 */
#define	RAM_ENDP2_SIZE		0x40		/* 端点2的缓冲区长度 */
#define	RAM_ENDP2_TRAN		0x40		/* 端点2发送缓冲区的起始地址 */
#define	RAM_ENDP2_RECV		0xC0		/* 端点2接收缓冲区的起始地址 */
#define	RAM_ENDP2_EXCH		0x80		/* 端点2备用缓冲区的起始地址 */
#define	RAM_HOST_TRAN		0x40		/* 主机发送缓冲区的起始地址 */
#define	RAM_HOST_RECV		0xC0		/* 主机接收缓冲区的起始地址 */
#define	RAM_HOST_EXCH		0x80		/* 主机备用缓冲区的起始地址 */

#define	REG_HUB_SETUP		0x02		/* 仅USB主机方式: ROOT-HUB配置 */
#define	REG_HUB_CTRL		0x03		/* 仅USB主机方式: ROOT-HUB控制 */
#define	REG_SYS_INFO		0x04		/* 系统信息, 只读 */
#define	REG_SYS_CTRL		0x05		/* 系统控制, 不受软件复位影响 */
#define	REG_USB_SETUP		0x06		/* USB配置 */
#define	REG_INTER_EN		0x07		/* 中断使能 */
#define	REG_USB_ADDR		0x08		/* USB设备地址 */
#define	REG_INTER_FLAG		0x09		/* 中断标志, 只读, 位写1清0 */
#define	REG_USB_STATUS		0x0A		/* USB状态, 只读 */
#define	REG_USB_LENGTH		0x0B		/* USB长度, 读为当前USB接收长度, 设备方式下写为USB端点2, 主机方式下写为USB主机发送长度 */
#define	REG_USB_ENDP0		0x0C		/* 仅USB设备方式: USB端点0控制 */
#define	REG_USB_ENDP1		0x0D		/* 仅USB设备方式: USB端点1控制 */
#define	REG_USB_H_PID		0x0D		/* 仅USB主机方式: USB主机令牌 */
#define	REG_USB_ENDP2		0x0E		/* 仅USB设备方式: USB端点2控制 */
#define	REG_USB_H_CTRL		0x0E		/* 仅USB主机方式: USB主机控制 */

/* 寄存器的位及常用宏定义 */

#define	BIT_HUB0_EN			0x01		/* HUB0端口的USB传输使能: 0-禁止, 1-允许 */
#define	BIT_HUB0_RESET		0x02		/* HUB0端口的USB总线复位控制: 0-不复位, 1-复位 */
#define	BIT_HUB0_POLAR		0x04		/* HUB0端口的信号极性控制: 0-正极性/全速, 1-负极性/低速 */
#define	BIT_HUB0_ATTACH		0x08		/* HUB0端口的USB设备连接状态(只读): 0-尚未连接/断开/拔出, 1-已经连接/插入 */
#define	BIT_HUB1_DX_IN		0x10		/* HUB1全速正极性时UD+引脚/低速负极性时UD-引脚的采样状态: 0-低电平,速度失配, 1-高电平,速度匹配 */
#define	BIT_HUB2_DX_IN		0x20		/* HUB2全速正极性时UD+引脚/低速负极性时UD-引脚的采样状态: 0-低电平,速度失配, 1-高电平,速度匹配 */
#define	BIT_HUB_PRE_PID		0x40		/* 低速前置包PRE PID输出控制: 0-禁止, 1-允许(外部设备是USB-HUB) */
#define	BIT_HUB_DISABLE		0x80		/* 禁止ROOT-HUB根集线器功能: 0-允许(仅USB主机方式), 1-禁止(默认) */

#define	BIT_HUB1_EN			0x01		/* HUB1端口的USB传输使能: 0-禁止, 1-允许 */
#define	BIT_HUB1_RESET		0x02		/* HUB1端口的USB总线复位控制: 0-不复位, 1-复位 */
#define	BIT_HUB1_POLAR		0x04		/* HUB1端口的信号极性控制: 0-正极性/全速, 1-负极性/低速 */
#define	BIT_HUB1_ATTACH		0x08		/* HUB1端口的USB设备连接状态(只读): 0-尚未连接/断开/拔出, 1-已经连接/插入 */
#define	BIT_HUB2_EN			0x10		/* HUB2端口的USB传输使能: 0-禁止, 1-允许 */
#define	BIT_HUB2_RESET		0x20		/* HUB2端口的USB总线复位控制: 0-不复位, 1-复位 */
#define	BIT_HUB2_POLAR		0x40		/* HUB2端口的信号极性控制: 0-正极性/全速, 1-负极性/低速 */
#define	BIT_HUB2_ATTACH		0x80		/* HUB2端口的USB设备连接状态(只读): 0-尚未连接/断开/拔出, 1-已经连接/插入 */

#define	BIT_INFO_HW_ID		0x03		/* 硬件识别位: 总是常量01, 否则说明读操作或硬件连接有误 */
#define	BIT_INFO_USB_DM		0x04		/* USB总线HUB0的UD-引脚的逻辑电平状态: 0-低电平, 1-高电平 */
#define	BIT_INFO_USB_DP		0x08		/* USB总线HUB0的UD+引脚的逻辑电平状态: 0-低电平, 1-高电平 */
#define	BIT_INFO_CLK_8KHZ	0x10		/* 硬件8KHz时钟位 */
#define	BIT_INFO_SOF_PRES	0x20		/* 硬件1mS定时周期状态,对于USB主机, 1说明将要产生SOF */
#define	BIT_INFO_WAKE_UP	0x40		/* 芯片唤醒状态: 0-正在睡眠或唤醒过程中, 1-已唤醒 */
#define	BIT_INFO_POWER_RST	0x80		/* 硬件上电复位完成状态: 0-正在复位, 1-复位完成 */

#define	BIT_CTRL_OSCIL_OFF	0x01		/* 时钟振荡器控制: 0-允许振荡, 1-停止振荡 */
#define	BIT_CTRL_CLK_12MHZ	0x02		/* 输入时钟频率选择: 0-24MHz, 1-12MHz */
#define	BIT_CTRL_USB_POWER	0x04		/* V3引脚的USB电源调节器控制: 0-开启, 1-禁用 */
#define	BIT_CTRL_RESET_NOW	0x08		/* 芯片软件复位控制: 0-不复位, 1-复位 */
#define	BIT_CTRL_WATCH_DOG	0x10		/* RST引脚和RST#引脚的看门狗复位使能: 0-禁用, 1-启用 */
#define	BIT_CTRL_INT_PULSE	0x20		/* INT#引脚的中断输出方式: 0-低电平中断, 1-低电平脉冲中断 */
#define	BIT_CTRL_OE_POLAR	0x40		/* UEN引脚的USB输出使能极性: 0-高电平使能, 1-低电平使能 */

#define	BIT_SETP_TRANS_EN	0x01		/* 仅USB设备方式: USB设备传输使能: 0-禁止, 1-允许 */
#define	BIT_SETP_PULLUP_EN	0x02		/* 仅USB设备方式: USB上拉电阻控制: 0-禁用上拉电阻, 1-启用上拉电阻 */
#define	BIT_SETP_BUS_CTRL	0x03		/* 仅USB主机方式: USB总线状态控制: 00-正常/空闲, 01-D+低D-低(总线复位), 10-禁用, 11-D+低D-高(总线恢复) */
#define	M_SET_USB_BUS_FREE( old )		( (old) & ~ BIT_SETP_BUS_CTRL | 0x00 )		/* 仅USB主机方式: USB总线空闲 */
#define	M_SET_USB_BUS_RESET( old )		( (old) & ~ BIT_SETP_BUS_CTRL | 0x01 )		/* 仅USB主机方式: USB总线状态控制/D+低D-低(总线复位) */
#define	M_SET_USB_BUS_RESUME( old )		( (old) & ~ BIT_SETP_BUS_CTRL | 0x03 )		/* 仅USB主机方式: USB总线状态控制/D+低D-高(总线恢复) */
#define	BIT_SETP_RAM_MODE	0x0C		/* 备用缓冲区应用方式: 00-禁用备用缓冲区, 01-连接接收缓冲区以连续接收128字节, 10-用于发送第二缓冲区, 11-用于接收第二缓冲区 */
#define	M_SET_RAM_MODE_OFF( old )		( (old) & ~ BIT_SETP_RAM_MODE | 0x00 )		/* 备用缓冲区方式/禁用备用缓冲区 */
#define	M_SET_RAM_MODE_128( old )		( (old) & ~ BIT_SETP_RAM_MODE | 0x04 )		/* 备用缓冲区方式/连接接收缓冲区以连续接收128字节 */
#define	M_SET_RAM_MODE_2TX( old )		( (old) & ~ BIT_SETP_RAM_MODE | 0x08 )		/* 备用缓冲区方式/用于发送的第二缓冲区,支持连续发送 */
#define	M_SET_RAM_MODE_2RX( old )		( (old) & ~ BIT_SETP_RAM_MODE | 0x0C )		/* 备用缓冲区方式/用于接收的第二缓冲区,支持连续接收 */
#define	BIT_SETP_LOW_SPEED	0x20		/* USB总线传输速度: 0-12Mbps, 1-1.5Mbps */
#define	BIT_SETP_USB_SPEED	0x30		/* USB总线速率: 00-全速模式/正极性12Mbps, 11-低速模式/负极性1.5Mbps */
#define	BIT_SETP_LED_ACT	0x40		/* 仅USB设备方式: ACT#引脚低电平的激活事件: 0-收发传输过程, 1-USB主机活动 */
#define	BIT_SETP_AUTO_SOF	0x40		/* 仅USB主机方式: 自动产生SOF使能: 0-禁止, 1-允许 */
#define	BIT_SETP_HOST_MODE	0x80		/* USB主从方式选择: 0-设备方式, 1-主机方式 */

#define	BIT_IE_TRANSFER		0x01		/* USB传输完成中断使能, 1有效 */
#define	BIT_IE_BUS_RESET	0x02		/* 仅USB设备方式: USB总线复位中断使能, 1有效 */
#define	BIT_IE_DEV_DETECT	0x02		/* 仅USB主机方式: USB设备检测中断使能, 1有效 */
#define	BIT_IE_USB_SUSPEND	0x04		/* USB总线挂起中断使能, 1有效 */
#define	BIT_IE_USB_RESUME	0x08		/* USB总线恢复/唤醒中断使能, 1有效, 0-使能芯片唤醒完成中断, 1-使能USB总线恢复中断 */
#define	BIT_IE_CLK_OUT_DIV	0xF0		/* 可编程时钟的分频除数: 输出频率为48MHz/(该值+1), 例如: 0000-48MHz, 0001-24MHz, 0010-16MHz, 1111-3MHz */
#define	M_SET_CLK_DIV( old, div )		( (old) & ~ BIT_IE_CLK_OUT_DIV | (div) << 4 )	/* 设置时钟输出分频除数 */

#define	BIT_ADDR_USB_DEV	0x7F		/* 在设备方式下为自身作为USB设备的地址, 在主机方式下为当前被操作的USB设备地址 */

#define	BIT_IF_INTER_FLAG	0x0F		/* 所有的USB中断标志 */
#define	BIT_IF_TRANSFER		0x01		/* USB传输完成中断标志, 1有效, 向该位写1清标志, 该位在每次USB传输完成后自动置1 */
#define	BIT_IF_BUS_RESET	0x02		/* 仅USB设备方式: USB总线复位中断标志, 1有效, 向该位写1清标志, 该位在检测到USB总线复位时自动置1 */
#define	BIT_IF_DEV_DETECT	0x02		/* 仅USB主机方式: USB设备插拔检测中断标志, 1有效, 向该位写1清标志, 该位在检测到USB设备插拔后自动置1 */
#define	BIT_IF_USB_SUSPEND	0x04		/* USB总线挂起中断标志, 1有效, 向该位写1清标志, 该位在检测到USB总线挂起时自动置1 */
#define	BIT_IF_WAKE_UP		0x08		/* 芯片唤醒完成中断标志, 1有效, 向该位写1清标志, 该位在芯片唤醒完成后自动置1 */
#define	BIT_IF_USB_RESUME	0x08		/* USB总线恢复/唤醒中断标志, 1有效, 向该位写1清标志, 该位在检测到USB总线恢复时自动置1 */
#define	BIT_IF_USB_PAUSE	0x10		/* USB传输暂停标志, 1有效, 向该位写1清标志, 该位在每次USB传输完成后自动置1 */
#define	BIT_IF_DEV_ATTACH	0x20		/* USB设备连接状态: 0-尚未连接/断开/拔出, 1-至少有一个USB设备已经连接/插入 */
#define	BIT_IF_USB_OE		0x40		/* UEN引脚的USB输出使能状态: 0-UEN引脚为低电平, 1-UEN引脚为高电平 */
#define	BIT_IF_USB_DX_IN	0x80		/* HUB0全速正极性时UD+引脚/低速负极性时UD-引脚的采样状态: 0-低电平,速度失配, 1-高电平,速度匹配 */
#define	BIT_HUB0_DX_IN		0x80		/* HUB0全速正极性时UD+引脚/低速负极性时UD-引脚的采样状态: 0-低电平,速度失配, 1-高电平,速度匹配 */

#define	BIT_STAT_THIS_ENDP	0x03		/* 仅USB设备方式: USB传输的目的端点号: 00-端点0, 01-端点1, 10-端点2, 11-保留 */
#define	BIT_STAT_THIS_PID	0x0C		/* 仅USB设备方式: USB传输的事务/令牌PID: 00-OUT事务, 01-保留, 10-IN事务, 11-SETUP事务 */
#define	BIT_STAT_PID_ENDP	0x0F		/* 仅USB设备方式: USB传输的事务和端点号,参考后面的USB_INT_EP*定义 */
#define	BIT_STAT_DEV_RESP	0x0F		/* 仅USB主机方式: USB设备的应答PID: XX00=错误或超时,其它值-同PID定义,参考后面的USB_INT_RET_*定义 */
#define	M_IS_HOST_TIMEOUT( status )		( ( (status) & 0x03 ) == 0 )		/* 检查USB主机状态是否为应答超时/出错 */
#define	M_IS_HOST_IN_DATA( status )		( ( (status) & BIT_STAT_DEV_RESP & ~ ( DEF_USB_PID_DATA0 ^ DEF_USB_PID_DATA1 ) ) == ( DEF_USB_PID_DATA0 & DEF_USB_PID_DATA1 ) )	/* 检查是否返回DATA0或者DATA1 */
#define	BIT_STAT_TOG_MATCH	0x10		/* 指示当前的传输是否成功/当前接收的数据包是否同步: 0-不同步, 1-同步 */
#define	BIT_STAT_BUS_RESET	0x20		/* 当前USB总线复位状态: 0-没有复位, 1-正在复位 */
#define	BIT_STAT_SUSPEND	0x40		/* 当前USB总线挂起状态: 0-总线有活动, 1-总线挂起 */
#define	BIT_STAT_SIE_FREE	0x80		/* 当前USB接口引擎SIE的状态: 0=忙/正在传输, 1=空闲/等待 */

#define	BIT_EP0_TRAN_RESP	0x0F		/* 仅USB设备方式: 端点0发送响应: 0000~1000-应答数据长度0~8, 1110-应答NAK, 1111-应答STALL,其它值-禁用 */
#define	M_SET_EP0_TRAN_ACK( old, len )	( (old) & ~ BIT_EP0_TRAN_RESP | (len) & 0x0F )	/* 仅USB设备方式: 端点0发送响应/应答ACK */
#define	M_SET_EP0_TRAN_NAK( old )		( (old) & ~ BIT_EP0_TRAN_RESP | 0x0E )		/* 仅USB设备方式: 端点0发送响应/应答NAK */
#define	M_SET_EP0_TRAN_STA( old )		( (old) & ~ BIT_EP0_TRAN_RESP | 0x0F )		/* 仅USB设备方式: 端点0发送响应/应答STALL */
#define	BIT_EP0_RECV_RESP	0x30		/* 仅USB设备方式: 端点0接收响应: 00-应答ACK, 01-禁用, 10-应答NAK, 11-应答STALL */
#define	M_SET_EP0_RECV_ACK( old )		( (old) & ~ BIT_EP0_RECV_RESP | 0x00 )		/* 仅USB设备方式: 端点0接收响应/应答ACK */
#define	M_SET_EP0_RECV_NAK( old )		( (old) & ~ BIT_EP0_RECV_RESP | 0x20 )		/* 仅USB设备方式: 端点0接收响应/应答NAK */
#define	M_SET_EP0_RECV_STA( old )		( (old) & ~ BIT_EP0_RECV_RESP | 0x30 )		/* 仅USB设备方式: 端点0接收响应/应答STALL */
#define	BIT_EP0_TRAN_TOG	0x40		/* 仅USB设备方式: 端点0发送同步标志: 0-DATA0, 1-DATA1 */
#define	BIT_EP0_RECV_TOG	0x80		/* 仅USB设备方式: 端点0接收同步标志: 0-DATA0, 1-DATA1 */

#define	BIT_EP1_TRAN_RESP	0x0F		/* 仅USB设备方式: 端点1发送响应: 0000~1000-应答数据长度0~8, 1110-应答NAK, 1111-应答STALL,其它值-禁用 */
#define	M_SET_EP1_TRAN_ACK( old, len )	( (old) & ~ BIT_EP1_TRAN_RESP | (len) & 0x0F )	/* 仅USB设备方式: 端点1发送响应/应答ACK */
#define	M_SET_EP1_TRAN_NAK( old )		( (old) & ~ BIT_EP1_TRAN_RESP | 0x0E )		/* 仅USB设备方式: 端点1发送响应/应答NAK */
#define	M_SET_EP1_TRAN_STA( old )		( (old) & ~ BIT_EP1_TRAN_RESP | 0x0F )		/* 仅USB设备方式: 端点1发送响应/应答STALL */
#define	BIT_EP1_RECV_RESP	0x30		/* 仅USB设备方式: 端点1接收响应: 00-应答ACK, 01-禁用, 10-应答NAK, 11-应答STALL */
#define	M_SET_EP1_RECV_ACK( old )		( (old) & ~ BIT_EP1_RECV_RESP | 0x00 )		/* 仅USB设备方式: 端点1接收响应/应答ACK */
#define	M_SET_EP1_RECV_NAK( old )		( (old) & ~ BIT_EP1_RECV_RESP | 0x20 )		/* 仅USB设备方式: 端点1接收响应/应答NAK */
#define	M_SET_EP1_RECV_STA( old )		( (old) & ~ BIT_EP1_RECV_RESP | 0x30 )		/* 仅USB设备方式: 端点1接收响应/应答STALL */
#define	BIT_EP1_TRAN_TOG	0x40		/* 仅USB设备方式: 端点1发送同步标志: 0-DATA0, 1-DATA1 */
#define	BIT_EP1_RECV_TOG	0x80		/* 仅USB设备方式: 端点1接收同步标志: 0-DATA0, 1-DATA1 */

#define	BIT_HOST_PID_ENDP	0x0F		/* 仅USB主机方式: 目的端点号: 0000~1111-端点号0~15 */
#define	BIT_HOST_PID_TOKEN	0xF0		/* 仅USB主机方式: 指定事务/令牌PID: 1101-SETUP事务, 0001-OUT事务, 1001-IN事务, 0101-SOF包,其它值-禁用 */
#define	M_MK_HOST_PID_ENDP( pid, endp )	( (pid) << 4 | (endp) & BIT_HOST_PID_ENDP )		/* 用事务/令牌PID和目的端点号生成USB主机令牌数据 */

#define	BIT_EP2_TRAN_RESP	0x03		/* 仅USB设备方式: 端点2发送响应: 00-应答数据, 01-同步/等时传输, 10-应答NAK, 11-应答STALL */
#define	M_SET_EP2_TRAN_ACK( old )		( (old) & ~ BIT_EP2_TRAN_RESP | 0x00 )		/* 仅USB设备方式: 端点2发送响应/应答ACK */
#define	M_SET_EP2_TRAN_ISO( old )		( (old) & ~ BIT_EP2_TRAN_RESP | 0x01 )		/* 仅USB设备方式: 端点2发送响应/同步/等时传输/无需应答 */
#define	M_SET_EP2_TRAN_NAK( old )		( (old) & ~ BIT_EP2_TRAN_RESP | 0x02 )		/* 仅USB设备方式: 端点2发送响应/应答NAK */
#define	M_SET_EP2_TRAN_STA( old )		( (old) & ~ BIT_EP2_TRAN_RESP | 0x03 )		/* 仅USB设备方式: 端点2发送响应/应答STALL */
#define	BIT_EP2_RECV_RESP	0x30		/* 仅USB设备方式: 端点2接收响应: 00-应答ACK, 01-同步/等时传输, 10-应答NAK, 11-应答STALL */
#define	M_SET_EP2_RECV_ACK( old )		( (old) & ~ BIT_EP2_RECV_RESP | 0x00 )		/* 仅USB设备方式: 端点2接收响应/应答ACK */
#define	M_SET_EP2_RECV_ISO( old )		( (old) & ~ BIT_EP2_RECV_RESP | 0x10 )		/* 仅USB设备方式: 端点2接收响应/同步/等时传输/不作应答 */
#define	M_SET_EP2_RECV_NAK( old )		( (old) & ~ BIT_EP2_RECV_RESP | 0x20 )		/* 仅USB设备方式: 端点2接收响应/应答NAK */
#define	M_SET_EP2_RECV_STA( old )		( (old) & ~ BIT_EP2_RECV_RESP | 0x30 )		/* 仅USB设备方式: 端点2接收响应/应答STALL */
#define	BIT_EP2_TRAN_TOG	0x40		/* 仅USB设备方式: 端点2发送同步标志: 0-DATA0, 1-DATA1 */
#define	BIT_EP2_RECV_TOG	0x80		/* 仅USB设备方式: 端点2接收同步标志: 0-DATA0, 1-DATA1 */

#define	BIT_HOST_TRAN_ISO	0x01		/* 仅USB主机方式: 主机发送的传输类型: 0-控制/批量/中断传输, 1-同步/等时传输 */
#define	BIT_HOST_START		0x08		/* 仅USB主机方式: 主机传输启动控制: 0-暂停, 1-启动传输,完成后自动清0 */
#define	BIT_HOST_RECV_ISO	0x10		/* 仅USB主机方式: 主机接收的传输类型: 0-控制/批量/中断传输, 1-同步/等时传输 */
#define	BIT_HOST_TRAN_TOG	0x40		/* 仅USB主机方式: 主机发送同步标志: 0-DATA0, 1-DATA1 */
#define	BIT_HOST_RECV_TOG	0x80		/* 仅USB主机方式: 主机接收同步标志: 0-DATA0, 1-DATA1 */

/* 状态及命令等常量 */

#define	CMD_SPI_374READ		0xC0		/* SPI读操作命令 */
#define	CMD_SPI_374WRITE	0x80		/* SPI写操作命令 */

#define CH374_BLOCK_SIZE		64		/* CH374 maximum data block size, CH374_MAX_DATA_LEN */

/* ********************************************************************************************************************* */
/* USB状态 */

/* 以下状态代码用于USB设备方式 */
/*   位4指示当前接收的数据包是否同步, 0=不同步, 1-同步 */
/*   位3-位2指示当前事务, 00=OUT, 10=IN, 11=SETUP */
/*   位1-位0指示当前端点, 00=端点0, 01=端点1, 10=端点2 */
#ifndef	USB_INT_EP0_SETUP
#define	USB_INT_EP0_SETUP	0x0C			/* USB端点0的SETUP */
#define	USB_INT_EP0_OUT		0x00			/* USB端点0的OUT */
#define	USB_INT_EP0_IN		0x08			/* USB端点0的IN */
#define	USB_INT_EP1_OUT		0x01			/* USB端点1的OUT */
#define	USB_INT_EP1_IN		0x09			/* USB端点1的IN */
#define	USB_INT_EP2_OUT		0x02			/* USB端点2的OUT */
#define	USB_INT_EP2_IN		0x0A			/* USB端点2的IN */
#endif

/* 以下状态代码用于USB主机方式 */
/*   位4指示当前接收的数据包是否同步, 0=不同步, 1-同步 */
/*   位3-位0指示USB设备的应答: 0010=ACK, 1010=NAK, 1110=STALL, 0011=DATA0, 1011=DATA1, XX00=应答错误或者超时无应答 */
#ifndef	USB_INT_RET_ACK
#define	USB_INT_RET_ACK		DEF_USB_PID_ACK		/* 错误:对于OUT/SETUP事务返回ACK */
#define	USB_INT_RET_NAK		DEF_USB_PID_NAK		/* 错误:返回NAK */
#define	USB_INT_RET_STALL	DEF_USB_PID_STALL	/* 错误:返回STALL */
#define	USB_INT_RET_DATA0	DEF_USB_PID_DATA0	/* 错误:对于IN事务返回DATA0 */
#define	USB_INT_RET_DATA1	DEF_USB_PID_DATA1	/* 错误:对于IN事务返回DATA1 */
#define	USB_INT_RET_TOUT	0x00			/* 错误:应答错误或者超时无应答 */
#define	USB_INT_RET_TOUT1	0x04			/* 错误:应答错误或者超时无应答 */
#define	USB_INT_RET_TOUT2	0x08			/* 错误:应答错误或者超时无应答 */
#define	USB_INT_RET_TOUT3	0x0C			/* 错误:应答错误或者超时无应答 */
#endif

/* 以下状态代码1XH用于USB主机方式的操作状态代码, 可以自行定义任意值, 下面的定义仅为了兼容CH375 */
#ifndef	USB_INT_SUCCESS
#define	USB_INT_SUCCESS		0x14			/* USB事务或者传输操作成功 */
#define	USB_INT_CONNECT		0x15			/* 检测到USB设备连接事件 */
#define	USB_INT_DISCONNECT	0x16			/* 检测到USB设备断开事件 */
#define	USB_INT_BUF_OVER	0x17			/* USB控制传输的数据太多, 缓冲区溢出 */
#define	USB_INT_DISK_ERR	0x1F			/* USB存储器操作失败 */
#endif
#ifndef	USB_INT_CONNECT_LS
#define	USB_INT_CONNECT_LS	0x13			/* 检测到低速USB设备连接事件 */
#endif

/* ********************************************************************************************************************* */
/* 常用USB结构和相关常量 */

/* USB请求类型 */
#ifndef	DEF_USB_REQ_TYPE
#define	DEF_USB_REQ_READ		0x80			/* 控制读操作 */
#define	DEF_USB_REQ_WRITE		0x00			/* 控制写操作 */
#define	DEF_USB_REQ_TYPE		0x60			/* 控制请求类型 */
#define	DEF_USB_REQ_STAND		0x00			/* 标准请求 */
#define	DEF_USB_REQ_CLASS		0x20			/* 设备类请求 */
#define	DEF_USB_REQ_VENDOR		0x40			/* 厂商请求 */
#define	DEF_USB_REQ_RESERVE		0x60			/* 保留请求 */
#endif

/* USB标准设备请求, RequestType的位6位5=00(Standard) */
#ifndef	DEF_USB_GET_DESCR
#define	DEF_USB_CLR_FEATURE		0x01
#define	DEF_USB_SET_FEATURE		0x03
#define	DEF_USB_GET_STATUS		0x00
#define	DEF_USB_SET_ADDRESS		0x05
#define	DEF_USB_GET_DESCR		0x06
#define	DEF_USB_SET_DESCR		0x07
#define	DEF_USB_GET_CONFIG		0x08
#define	DEF_USB_SET_CONFIG		0x09
#define	DEF_USB_GET_INTERF		0x0A
#define	DEF_USB_SET_INTERF		0x0B
#define	DEF_USB_SYNC_FRAME		0x0C
#endif

/* 描述符类型 */
#ifndef	USB_DEVICE_DESCR_TYPE
#define	USB_DEVICE_DESCR_TYPE		0x01
#define	USB_CONFIG_DESCR_TYPE		0x02
#define	USB_STRING_DESCR_TYPE		0x03
#define	USB_INTERF_DESCR_TYPE		0x04
#define	USB_ENDP_DESCR_TYPE			0x05
#define	USB_POWER_DESCR_TYPE		0x06
#define	USB_HID_DESCR_TYPE			0x21
#define	USB_REPORT_DESCR_TYPE		0x22
#define	USB_PHYSIC_DESCR_TYPE		0x23
#endif

/* 端点类型和属性 */
#ifndef	USB_ENDP_TYPE_MASK
#define USB_ENDP_DIR_MASK			0x80
#define USB_ENDP_TYPE_MASK			0x03
#define USB_ENDP_TYPE_CTRL			0x00
#define USB_ENDP_TYPE_ISOCH			0x01
#define USB_ENDP_TYPE_BULK			0x02
#define	USB_ENDP_TYPE_INTER			0x03
#endif

/* 设备类别 */
#ifndef	USB_DEV_CLASS_HUB
#define	USB_DEV_CLASS_RESERVED		0x00
#define	USB_DEV_CLASS_AUDIO			0x01
#define	USB_DEV_CLASS_COMMUNIC		0x02
#define	USB_DEV_CLASS_HUMAN_IF		0x03
#define	USB_DEV_CLASS_MONITOR		0x04
#define	USB_DEV_CLASS_PHYSIC_IF		0x05
#define	USB_DEV_CLASS_POWER			0x06
#define	USB_DEV_CLASS_PRINTER		0x07
#define	USB_DEV_CLASS_STORAGE		0x08
#define	USB_DEV_CLASS_HUB			0x09
#define	USB_DEV_CLASS_VEN_SPEC		0xFF
#endif

#define	DEFAULT_ENDP0_SIZE	8				/* 默认的端点0最大包长度 */

/* ********************************************************************************************************************* */

/* FILE: CH374HF.H */

typedef unsigned char                BOOL1;

#ifndef UINT8
typedef unsigned char                UINT8;
#endif
#ifndef UINT16
typedef unsigned short               UINT16;
#endif
#ifndef UINT32
typedef unsigned long                UINT32;
#endif
#ifndef PUINT8
typedef unsigned char               *PUINT8;
#endif
#ifndef PUINT16
typedef unsigned short              *PUINT16;
#endif
#ifndef PUINT32
typedef unsigned long               *PUINT32;
#endif
#ifndef PUINT8C
typedef const unsigned char         *PUINT8C;
#endif
#ifndef UINT8V
typedef unsigned char volatile       UINT8V;
#endif

/* 错误码 */
#define ERR_SUCCESS				0x00	/* 操作成功 */
#define ERR_CH374_ERROR			0x81	/* CH374硬件错误,可能需要复位CH374 */
#define ERR_DISK_DISCON			0x82	/* 磁盘尚未连接,可能磁盘已经断开 */
#define ERR_STATUS_ERR			0x83	/* 磁盘状态错误,可能正在连接或者断开磁盘 */
#define ERR_HUB_PORT_FREE		0x84	/* USB-HUB已经连接但是HUB端口尚未连接磁盘,可能磁盘已经断开 */
#define ERR_MBR_ERROR			0x91	/* 磁盘的主引导记录无效,可能磁盘尚未分区或者尚未格式化 */
#define ERR_TYPE_ERROR			0x92	/* 磁盘分区类型不支持,只支持FAT12/FAT16/BigDOS/FAT32,需要由磁盘管理工具重新分区 */
#define ERR_BPB_ERROR			0xA1	/* 磁盘尚未格式化,或者参数错误,需要由WINDOWS采用默认参数重新格式化 */
#define ERR_TOO_LARGE			0xA2	/* 磁盘非正常格式化并且容量大于4GB,或者容量大于250GB,需要由WINDOWS采用默认参数重新格式化 */
#define ERR_FAT_ERROR			0xA3	/* 磁盘的文件系统不支持,只支持FAT12/FAT16/FAT32,需要由WINDOWS采用默认参数重新格式化 */
#define ERR_DISK_FULL			0xB1	/* 磁盘文件太满,剩余空间太少或者已经没有,需要磁盘整理 */
#define ERR_FDT_OVER			0xB2	/* 目录内文件太多,没有空闲的目录项,FAT12/FAT16根目录下的文件数应该少于500个,需要磁盘整理 */
#define ERR_MISS_DIR			0xB3	/* 指定路径的某个子目录没有找到,可能是目录名称错误 */
#define ERR_FILE_CLOSE			0xB4	/* 文件已经关闭,如果需要使用,应该重新打开文件 */
#define ERR_OPEN_DIR			0x41	/* 指定路径的目录被打开 */
#define ERR_MISS_FILE			0x42	/* 指定路径的文件没有找到,可能是文件名称错误 */
#define ERR_FOUND_NAME			0x43	/* 搜索到与通配符相匹配的文件名,文件名及其完整路径在命令缓冲区中,如果需要使用,应该打开该文件 */
/* 代码2XH-3XH用于USB主机方式的通讯失败代码,由CH374子程序模仿CH375的返回 */
/* 代码1XH用于USB主机方式的操作状态代码,由CH374子程序模仿CH375的返回 */
#define	ERR_USB_CONNECT_LS		0x13	/* 检测到低速USB设备连接事件 */
#define	ERR_USB_CONNECT			0x15	/* 检测到USB设备连接事件,磁盘已经连接 */
#define	ERR_USB_DISCON			0x16	/* 检测到USB设备断开事件,磁盘已经断开 */
#define	ERR_USB_DISK_ERR		0x1F	/* USB存储器操作失败,在初始化时可能是USB存储器不支持,在读写操作中可能是磁盘损坏或者已经断开 */
/* 以下状态代码为特殊事件中断 */
#define	USB_INT_USB_SUSPEND		0x05	/* USB总线挂起事件 */
#define	USB_INT_WAKE_UP			0x06	/* 从睡眠中被唤醒事件 */

/* 磁盘及文件状态 */
#define DISK_UNKNOWN			0x00	/* 尚未初始化,未知状态 */
#define DISK_DISCONNECT			0x01	/* 磁盘没有连接或者已经断开 */
#define DISK_CONNECT			0x02	/* 磁盘已经连接,但是尚未初始化或者无法识别该磁盘 */
#define DISK_MOUNTED			0x03	/* 磁盘已经初始化成功,但是尚未分析文件系统或者文件系统不支持 */
#define DISK_READY				0x10	/* 已经分析磁盘的文件系统并且能够支持 */
#define DISK_OPEN_ROOT			0x12	/* 已经打开根目录,扇区模式,只能以扇区为单位读写目录的内容,使用后必须关闭,注意FAT12/FAT16根目录是固定长度 */
#define DISK_OPEN_DIR			0x13	/* 已经打开子目录,扇区模式,只能以扇区为单位读写目录的内容 */
#define DISK_OPEN_FILE			0x14	/* 已经打开文件,扇区模式,可以以扇区为单位进行数据读写 */
#define DISK_OPEN_FILE_B		0x15	/* 已经打开文件,字节模式,可以以字节为单位进行数据读写 */

/* FAT类型标志 */
#define DISK_FS_UNKNOWN			0		/* 未知的文件系统 */
#define DISK_FAT12				1		/* FAT12文件系统 */
#define DISK_FAT16				2		/* FAT16文件系统 */
#define DISK_FAT32				3		/* FAT32文件系统 */

/* FAT数据区中文件目录信息 */
typedef struct _FAT_DIR_INFO {
	UINT8	DIR_Name[11];				/* 00H,文件名,共11字节,不足处填空格 */
	UINT8	DIR_Attr;					/* 0BH,文件属性,参考前面的说明 */
	UINT8	DIR_NTRes;					/* 0CH */
	UINT8	DIR_CrtTimeTenth;			/* 0DH,文件创建的时间,以0.1秒单位计数 */
	UINT16	DIR_CrtTime;				/* 0EH,文件创建的时间 */
	UINT16	DIR_CrtDate;				/* 10H,文件创建的日期 */
	UINT16	DIR_LstAccDate;				/* 12H,最近一次存取操作的日期 */
	UINT16	DIR_FstClusHI;				/* 14H */
	UINT16	DIR_WrtTime;				/* 16H,文件修改时间,参考前面的宏MAKE_FILE_TIME */
	UINT16	DIR_WrtDate;				/* 18H,文件修改日期,参考前面的宏MAKE_FILE_DATA */
	UINT16	DIR_FstClusLO;				/* 1AH */
	UINT32	DIR_FileSize;				/* 1CH,文件长度 */
} FAT_DIR_INFO;							/* 20H */

typedef FAT_DIR_INFO *P_FAT_DIR_INFO;

/* 文件属性 */
#define ATTR_READ_ONLY			0x01	/* 文件为只读属性 */
#define ATTR_HIDDEN				0x02	/* 文件为隐含属性 */
#define ATTR_SYSTEM				0x04	/* 文件为系统属性 */
#define ATTR_VOLUME_ID			0x08	/* 卷标 */
#define ATTR_DIRECTORY			0x10	/* 子目录 */
#define ATTR_ARCHIVE			0x20	/* 文件为存档属性 */
#define ATTR_LONG_NAME			( ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID )
/* 文件属性 UINT8 */
/* bit0 bit1 bit2 bit3 bit4 bit5 bit6 bit7 */
/*  只   隐   系   卷   目   存   未定义   */
/*  读   藏   统   标   录   档            */
/* 文件时间 UINT16 */
/* Time = (Hour<<11) + (Minute<<5) + (Second>>1) */
#define MAKE_FILE_TIME( h, m, s )	( (h<<11) + (m<<5) + (s>>1) )	/* 生成指定时分秒的文件时间数据 */
/* 文件日期 UINT16 */
/* Date = ((Year-1980)<<9) + (Month<<5) + Day */
#define MAKE_FILE_DATE( y, m, d )	( ((y-1980)<<9) + (m<<5) + d )	/* 生成指定年月日的文件日期数据 */

/* 文件名 */
#define PATH_WILDCARD_CHAR		0x2A	/* 路径名的通配符 '*' */
#define PATH_SEPAR_CHAR1		0x5C	/* 路径名的分隔符 '\' */
#define PATH_SEPAR_CHAR2		0x2F	/* 路径名的分隔符 '/' */
#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN			30		/* 最大路径长度,含所有斜杠分隔符和小数点间隔符以及路径结束符00H */
#endif
#ifndef MAX_BYTE_IO
#define MAX_BYTE_IO		( MAX_PATH_LEN - 1 )	/* 以字节为单位单次读写文件时的最大长度,超过该长度可以分多次读写 */
#endif

/* 外部命令参数 */
typedef union _CMD_PARAM {
	struct {
		UINT8	mBuffer[ MAX_PATH_LEN ];
	} Other;
	struct {
		UINT32	mReserved;
		UINT32	mTotalSector;			/* 返回: 当前逻辑盘的总扇区数 */
		UINT32	mFreeSector;			/* 返回: 当前逻辑盘的剩余扇区数 */
		UINT8	mDiskFat;				/* 返回: 当前逻辑盘的FAT类型 */
	} Query;							/* CMD_DiskQuery, 查询磁盘信息 */
	struct {
		UINT8	mPathName[ MAX_PATH_LEN ];	/* 输入参数: 路径: [盘符,冒号,斜杠,目录名或者文件名及扩展名...,结束符00H], 其中盘符和冒号可以省略, 例如"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} Open;								/* CMD_FileOpen, 打开文件 */
	struct {
		UINT8	mPathName[ MAX_PATH_LEN ];	/* 输入参数: 路径: [盘符,冒号,斜杠,目录名或者文件名及扩展名(含通配符*)...,枚举序号], 其中盘符和冒号可以省略, 例如"C:\DIR1.EXT\DIR2\FILE*",00H */
	} Enumer;							/* CMD_FileEnumer, 枚举文件,返回文件名 */
	struct {
		UINT8	mUpdateLen;				/* 输入参数: 是否允许更新长度: 0禁止,1允许 */
	} Close;							/* CMD_FileClose, 关闭当前文件 */
	struct {
		UINT8	mPathName[ MAX_PATH_LEN ];	/* 输入参数: 路径: [盘符,冒号,斜杠,目录名或者文件名及扩展名...,结束符00H], 其中盘符和冒号可以省略, 例如"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} Create;							/* CMD_FileCreate, 新建文件并打开,如果文件已经存在则先删除后再新建 */
	struct {
		UINT8	mPathName[ MAX_PATH_LEN ];	/* 输入参数: 路径: [盘符,冒号,斜杠,目录名或者文件名及扩展名...,结束符00H], 其中盘符和冒号可以省略, 例如"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} Erase;							/* CMD_FileErase, 删除文件并关闭 */
	struct {
		UINT32	mFileSize;				/* 输入参数: 新的文件长度,为0FFFFFFFFH则不修改, 返回: 原长度 */
		UINT16	mFileDate;				/* 输入参数: 新的文件日期,为0FFFFH则不修改, 返回: 原日期 */
		UINT16	mFileTime;				/* 输入参数: 新的文件时间,为0FFFFH则不修改, 返回: 原时间 */
		UINT8	mFileAttr;				/* 输入参数: 新的文件属性,为0FFH则不修改, 返回: 原属性 */
	} Modify;							/* CMD_FileQuery, 查询当前文件的信息; CMD_FileModify, 查询或者修改当前文件的信息 */
	struct {
		UINT32	mSectorOffset;			/* 输入参数: 扇区偏移,0则移动到文件头,0FFFFFFFFH则移动到文件尾, 返回: 当前文件指针对应的绝对线性扇区号, 0FFFFFFFFH则已到文件尾 */
	} Locate;							/* CMD_FileLocate, 移动当前文件指针 */
	struct {
		UINT8	mSectorCount;			/* 输入参数: 读取扇区数, 返回: 实际读取扇区数 */
	} Read;								/* CMD_FileRead, 从当前文件读取数据 */
	struct {
		UINT8	mSectorCount;			/* 输入参数: 写入扇区数, 返回: 实际写入扇区数 */
	} Write;							/* CMD_FileWrite, 向当前文件写入数据 */
	struct {
		UINT8	mSectorCount;			/* 输入参数: 读取扇区数, 返回: 实际读取扇区数 */
		UINT8	mReserved;
		UINT8	mLbaCount;
		UINT8	mReserved5[5];
		PUINT8	mDataBuffer;			/* 输入参数: 缓冲区起始地址, 返回: 缓冲区当前地址 */
	} ReadX;							/* CMD_FileReadX, 从当前文件读取数据到指定缓冲区 */
	struct {
		UINT8	mSectorCount;			/* 输入参数: 写入扇区数, 返回: 实际写入扇区数 */
		UINT8	mReserved;
		UINT8	mLbaCount;
		UINT8	mReserved5[5];
		PUINT8	mDataBuffer;			/* 输入参数: 缓冲区起始地址, 返回: 缓冲区当前地址 */
	} WriteX;							/* CMD_FileWriteX, 向当前文件写入指定缓冲区的数据 */
	struct {
		UINT32	mDiskSizeSec;			/* 返回: 整个物理磁盘的总扇区数 */
	} DiskSize;							/* CMD_DiskSize, 查询磁盘容量 */
	struct {
		UINT32	mByteOffset;			/* 输入参数: 以字节为单位的偏移量, 以字节为单位的文件指针, 返回: 当前文件指针对应的绝对线性扇区号, 0FFFFFFFFH则已到文件尾 */
	} ByteLocate;						/* CMD_ByteLocate, 以字节为单位移动当前文件指针 */
	struct {
		UINT8	mByteCount;				/* 输入参数: 准备读取的字节数,不得大于MAX_BYTE_IO, 返回: 实际读出的字节数 */
		UINT8	mByteBuffer[ MAX_BYTE_IO ];	/* 返回: 读出的数据块 */
	} ByteRead;							/* CMD_ByteRead, 以字节为单位从当前文件读取数据块 */
	struct {
		UINT8	mByteCount;				/* 输入参数: 准备写入的字节数,不得大于MAX_BYTE_IO, 返回: 实际写入的字节数 */
		UINT8	mByteBuffer[ MAX_BYTE_IO ];	/* 输入参数: 准备写入的数据块 */
	} ByteWrite;						/* CMD_ByteWrite, 以字节为单位向当前文件写入数据块 */
	struct {
		UINT8	mSaveVariable;			/* 输入参数: 为0则恢复单个U盘的变量,为0x80则恢复多个U盘的变量,其它值则备份/保存变量 */
		UINT8	mReserved[3];
		PUINT8	mBuffer;				/* 输入参数: 指向子程序库的变量的备份缓冲区,长度不小于80个字节 */
	} SaveVariable;						/* CMD_SaveVariable, 备份/保存/恢复子程序库的变量 */
} CMD_PARAM;

typedef CMD_PARAM CMD_PARAM_I;
typedef CMD_PARAM *P_CMD_PARAM;

/* SCSI命令码 */
#define SPC_CMD_INQUIRY			0x12
#define SPC_CMD_READ_CAPACITY	0x25
#define SPC_CMD_READ10			0x28
#define SPC_CMD_WRITE10			0x2A
#define SPC_CMD_TEST_READY		0x00
#define SPC_CMD_REQUEST_SENSE	0x03
#define SPC_CMD_MODESENSE6		0x1A
#define SPC_CMD_MODESENSE10		0x5A
#define SPC_CMD_START_STOP		0x1B

/* BOC */
typedef union _BULK_ONLY_CMD {
	struct {
/*		UINT32	mCBW_Sig;*/
/*		UINT32	mCBW_Tag;*/
		UINT8	mCBW_DataLen0;			/* 输入: 数据传输长度,有效值是0到65535 */
		UINT8	mCBW_DataLen1;
		UINT16	mCBW_DataLen2;
		UINT8	mCBW_Flag;				/* 输入: 传输方向等标志 */
		UINT8	mCBW_LUN;
		UINT8	mCBW_CB_Len;			/* 输入: 命令块的长度,有效值是1到16 */
		UINT8	mCBW_CB_Buf[10];		/* 输入: 命令块,该缓冲区最多为16个字节 */
	} mCBW;								/* BulkOnly协议的命令块, 输入CBW结构 */
	struct {
		UINT32	mCSW_Sig;
		UINT32	mCSW_Tag;
		UINT32	mCSW_Residue;			/* 返回: 剩余数据长度 */
		UINT8	mCSW_Status;			/* 返回: 命令执行结果状态 */
		UINT8	mReserved;
	} mCSW;								/* BulkOnly协议的命令状态块, 输出CSW结构 */
} BULK_ONLY_CMD;						/* CMD_BulkOnlyCmd, 执行基于BulkOnly协议的命令, 如果有数据传输那么数据在pDISK_BASE_BUF中 */

typedef BULK_ONLY_CMD BULK_ONLY_CMD_X;

/* FILE: CH374HF?.C */

#define EN_DISK_WRITE			1
#define EN_DISK_FAT12			1
#define EN_DISK_FAT32			1
#define EN_BYTE_ACCESS			1
#define EN_SAVE_VARIABLE		1
#define EN_HUB_DISK				1
#define EN_SEC_SIZE_AUTO		1

#ifndef LIB_CFG_NO_DLY
#define LIB_CFG_NO_DLY			0		/* 默认情况下,在写操作结束后的延时方式为"写后延时" */
#endif
#ifndef LIB_CFG_INT_EN
#define LIB_CFG_INT_EN			0		/* 默认情况下,CH374的INT#引脚连接方式为"查询方式" */
#endif
#ifndef DISK_BASE_BUF_LEN
#define DISK_BASE_BUF_LEN		4096		/* 默认的磁盘数据缓冲区大小为512字节,建议选择为2048甚至4096以支持某些大扇区的U盘,为0则禁止在.H文件中定义缓冲区并由应用程序在pDISK_BASE_BUF中指定 */
#endif

/* 子程序库中提供的变量 */
extern UINT8V	CH374IntStatus;			/* CH374操作的中断状态 */
extern UINT8V	CH374DiskStatus;		/* 磁盘及文件状态 */
extern UINT8	CH374vDiskFat;			/* 逻辑盘的FAT标志:1=FAT12,2=FAT16,3=FAT32 */
extern UINT8	CH374vSecPerClus;		/* 逻辑盘的每簇扇区数 */
extern UINT32	CH374vStartCluster;		/* 当前文件或者目录的起始簇号 */
extern UINT32	CH374vFileSize;			/* 当前文件的长度 */
extern UINT32	CH374vCurrentOffset;	/* 当前文件指针,当前读写位置的字节偏移 */
extern UINT32	CH374vDiskRoot;			/* 对于FAT16盘为根目录占用扇区数,对于FAT32盘为根目录起始簇号 */
extern UINT32	CH374vDataStart;		/* 逻辑盘的数据区域的起始LBA */
extern UINT32	CH374vFdtLba;			/* 当前FDT所在的LBA地址 */
extern UINT16	CH374vFdtOffset;		/* 当前FDT在扇区内的偏移地址 */
extern UINT8	CH374vRetryCount;		/* 位7为1则NAK无限重试,为0则NAK不重试,位5为1则USB存储设备的子类为6,为0则子类为非6,位3至位0为出错重试次数 */
extern BOOL1	CH374vUsbPidIn;			/* 当前USB传输的PID是否为IN:1=是IN,0=是OUT或者SETUP */
extern UINT8	CH374vDevEndpTog;		/* USB存储设备的端点的数据同步标志:位7对应BIT_HOST_RECV_TOG,位6对应BIT_HOST_TRAN_TOG,位3必须为1,其它位必须为0 */
extern UINT8	CH374vCurrentLun;		/* USB存储设备的当前逻辑单元号 */
extern UINT8	CH374vDiskRetry;		/* USB存储设备读写失败后的重试计数,位7为1则启用磁盘存取的外部接口 */
extern UINT8	CH374vEmbHubIndex;		/* CH374内置HUB的当前操作端口号,为0则不启用内部HUB,否则为端口号 */
#ifdef EN_HUB_DISK
extern UINT8	CH374vHubPortCount;		/* HUB上的端口数,为0则没有HUB */
extern UINT8	CH374vHubPortIndex;		/* HUB上的当前操作端口号,位7为0则自动查询,为1则指定端口号 */
#endif
#ifdef EN_SEC_SIZE_AUTO
extern UINT16	CH374vSectorSize;		/* 磁盘的扇区大小 */
#else
#define	CH374vSectorSize	512			/* 磁盘的扇区大小 */
#endif
extern PUINT8	pDISK_BASE_BUF;			/* 指向外部RAM的磁盘数据缓冲区,缓冲区长度不小于CH374vSectorSize,由应用程序初始化 */

extern UINT8	CH374ReadBlock( void );		/* 从磁盘读取多个扇区的数据到外部接口交换区 */
#ifdef EN_DISK_WRITE
extern UINT8	CH374WriteBlock( void );	/* 将外部接口交换区的多个扇区的数据块写入磁盘 */
#endif

/* 子程序库中提供的子程序 */
/* 下述子程序中, 文件操作子程序CH374File*和磁盘查询子程序CH374DiskQuery都可能会用到磁盘数据缓冲区pDISK_BASE_BUF,
   并且有可能在pDISK_BASE_BUF中保存了磁盘信息, 所以必须保证pDISK_BASE_BUF不被用于其它用途,
   如果RAM较少, 要将pDISK_BASE_BUF临时用于其它用途, 那么在临时用完后必须调用CH374DirtyBuffer清除磁盘缓冲区 */
extern UINT8	CH374GetVer( void );		/* 获取当前子程序库的版本号 */
extern UINT8	CH374Init( void );			/* 初始化CH374 */
extern void		CH374DelaymS( UINT8 iDelay );	/* 延时指定毫秒,不大于255毫秒 */
extern UINT8	CH374DiskConnect( void );	/* 检查磁盘是否连接并更新磁盘状态 */
extern UINT8	CH374DiskReady( void );		/* 查询磁盘是否准备好 */
extern void		CH374DirtyBuffer( void );	/* 清除磁盘缓冲区 */
extern UINT8	CH374FileOpen( void );		/* 打开文件或者枚举文件 */
extern UINT8	CH374FileClose( void );		/* 关闭当前文件 */
#ifdef EN_DISK_WRITE
extern UINT8	CH374FileErase( void );		/* 删除文件并关闭 */
extern UINT8	CH374FileCreate( void );	/* 新建文件并打开,如果文件已经存在则先删除后再新建 */
#endif
extern UINT8	CH374FileModify( void );	/* 查询或者修改当前文件的信息 */
extern UINT8	CH374FileLocate( void );	/* 移动当前文件指针 */
extern UINT8	CH374FileReadX( void );		/* 从当前文件读取数据到指定缓冲区 */
#ifdef EN_DISK_WRITE
extern UINT8	CH374FileWriteX( void );	/* 向当前文件写入指定缓冲区的数据 */
#endif
#ifdef EN_BYTE_ACCESS
extern UINT8	CH374ByteLocate( void );	/* 以字节为单位移动当前文件指针 */
extern UINT8	CH374ByteRead( void );		/* 以字节为单位从当前位置读取数据块 */
#ifdef EN_DISK_WRITE
extern UINT8	CH374ByteWrite( void );		/* 以字节为单位向当前位置写入数据块 */
#endif
#endif
extern UINT8	CH374DiskSize( void );		/* 查询磁盘容量 */
extern UINT8	CH374DiskQuery( void );		/* 查询磁盘信息 */
#ifdef EN_SAVE_VARIABLE
extern void		CH374SaveVariable( void );	/* 备份/保存/恢复子程序库的变量,用于子程序库在多个CH374芯片之间进行切换 */
#endif
extern UINT8	CH374BulkOnlyCmd( void );	/* 执行基于BulkOnly协议的命令 */
extern UINT8	CH374HostTransact( void );	/* 传输事务,需预先输入CH374UsbPidIn,PID令牌+目的端点地址,同步标志,返回同CH375(除USB_INT_SUCCESS为ERR_SUCCESS),NAK及出错重试 */
extern UINT8	CH374CtrlTransfer( void );	/* 执行控制传输,需预先写入8字节请求码,收发的数据在pDISK_BASE_BUF缓冲区 */
extern UINT8	CH374EmbHubAttach( void );  /* 检查当前的内置HUB端口是否有USB设备,返回:0无设备,非0有设备,( BIT_HUB0_ATTACH | BIT_HUB0_EN )有设备且未变化过 */
extern void		CH374EmbHubEnable( void );	/* 启用内置HUB端口, 输入: CH374vEmbHubIndex 内置HUB的操作端口号 */

/* 该头文件可以为CH374子程序库分配必要的I/O及内存资源,并产生必要的与硬件有关的目标代码,
   如果该文件是被工程项目的多个源程序包含作为头文件,那么应该只允许一个头文件分配资源和产生代码,
   除此之外的头文件应该被事先定义CH374HF_NO_CODE,从而禁止该头文件产生重复的目标代码,例如:
#define		CH374HF_NO_CODE		1
#include	CH374HF?.H
*/
#ifdef CH374HF_NO_CODE

extern UINT8	CH374_READ_REGISTER( UINT8 mAddr );					/* 从指定寄存器读取数据 */
extern void		CH374_WRITE_REGISTER( UINT8 mAddr, UINT8 mData );	/* 向指定寄存器写入数据 */
extern void		CH374_READ_BLOCK( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf );	/* 从指定起始地址读出数据块 */
extern void		CH374_WRITE_BLOCK( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf );	/* 向指定起始地址写入数据块 */
extern PUINT8	CH374_READ_BLOCK64( UINT8 mAddr, PUINT8 mBuf );	/* 从双缓冲区读出64字节的数据块,返回当前地址 */
#ifdef EN_DISK_WRITE
extern PUINT8	CH374_WRITE_BLOCK64( UINT8 mAddr, PUINT8 mBuf );	/* 向双缓冲区写入64字节的数据块,返回当前地址 */
#endif
extern void		CH374_WRITE_BLOCK_C( UINT8 mLen, PUINT8C mBuf );	/* 向RAM_HOST_TRAN写入常量型数据块 */

extern CMD_PARAM_I mCmdParam;				/* 命令参数 */
extern BULK_ONLY_CMD_X mBOC;				/* BO协议的命令包 */
#if DISK_BASE_BUF_LEN
extern UINT8  DISK_BASE_BUF[ DISK_BASE_BUF_LEN ];	/* 外部RAM的磁盘数据缓冲区,缓冲区长度为一个扇区的长度,起始地址必须为8字节边界地址 */
#endif
#ifdef FILE_DATA_BUF_LEN
extern UINT8  FILE_DATA_BUF[ FILE_DATA_BUF_LEN ];	/* 外部RAM的文件数据缓冲区,缓冲区长度不小于一次读写的数据长度 */
extern UINT8	CH374FileRead( void );		/* 从当前文件读取数据 */
#ifdef EN_DISK_WRITE
extern UINT8	CH374FileWrite( void );		/* 向当前文件写入数据 */
#endif
#endif
#ifndef NO_DEFAULT_CH374_F_ENUM
extern UINT8	CH374FileEnumer( void );	/* 枚举文件 */
#endif
#ifndef NO_DEFAULT_CH374_F_QUERY
extern UINT8	CH374FileQuery( void );		/* 查询当前文件的信息 */
#endif
#ifndef NO_DEFAULT_CH374_RESET
extern void		CH374Reset( void );			/* 复位CH374 */
#endif
extern void xQueryInterrupt( void );		/* 外部定义的被CH374程序库调用的子程序,查询CH374中断 */
#ifdef EN_DISK_WRITE
extern void xDelayAfterWrite( void );		/* 外部定义的被CH374程序库调用的子程序,写操作后延时 */
#endif
extern void xFileNameEnumer( void );		/* 外部定义的被CH374程序库调用的子程序,文件名枚举回调子程序 */
extern UINT8 CH374LibInit( void );			/* 初始化CH374程序库和CH374芯片,操作成功返回0 */

#else

UINT8	CH374_READ_REGISTER( UINT8 mAddr );					/* 外部定义的被CH374程序库调用的子程序,从指定寄存器读取数据 */
void	CH374_WRITE_REGISTER( UINT8 mAddr, UINT8 mData );	/* 外部定义的被CH374程序库调用的子程序,向指定寄存器写入数据 */
void	CH374_READ_BLOCK( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf );	/* 外部定义的被CH374程序库调用的子程序,从指定起始地址读出数据块 */
void	CH374_WRITE_BLOCK( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf );	/* 外部定义的被CH374程序库调用的子程序,向指定起始地址写入数据块 */
PUINT8	CH374_READ_BLOCK64( UINT8 mAddr, PUINT8 mBuf );	/* 外部定义的被CH374程序库调用的子程序,从双缓冲区读出64字节的数据块,返回当前地址 */
#ifdef EN_DISK_WRITE
PUINT8	CH374_WRITE_BLOCK64( UINT8 mAddr, PUINT8 mBuf );	/* 外部定义的被CH374程序库调用的子程序,向双缓冲区写入64字节的数据块,返回当前地址 */
#endif
void	CH374_WRITE_BLOCK_C( UINT8 mLen, PUINT8C mBuf );	/* 外部定义的被CH374程序库调用的子程序,向RAM_HOST_TRAN写入常量型数据块 */

CMD_PARAM_I mCmdParam;					/* 命令参数 */
BULK_ONLY_CMD_X mBOC;					/* BO协议的命令包 */
#if DISK_BASE_BUF_LEN
//#pragma SAVE
//#pragma PACK(4)
#ifdef DISK_BASE_BUF_ADDR
//__align(8)	UINT8  DISK_BASE_BUF[ DISK_BASE_BUF_LEN ];	/* 外部RAM的磁盘数据缓冲区,缓冲区长度为一个扇区的长度,起始地址必须为8字节边界地址 */
UINT8  DISK_BASE_BUF[ DISK_BASE_BUF_LEN ];	/* 外部RAM的磁盘数据缓冲区,缓冲区长度为一个扇区的长度,起始地址必须为8字节边界地址 */
#else
__align(8)	UINT8  DISK_BASE_BUF[ DISK_BASE_BUF_LEN ];	/* 外部RAM的磁盘数据缓冲区,缓冲区长度为一个扇区的长度,起始地址必须为8字节边界地址 */
#endif
//#pragma RESTORE
#endif
#ifdef FILE_DATA_BUF_LEN
//#pragma SAVE
//#pragma PACK(4)
#ifdef FILE_DATA_BUF_ADDR
__align(8)	UINT8  FILE_DATA_BUF[ FILE_DATA_BUF_LEN ] _at_ FILE_DATA_BUF_ADDR;	/* 外部RAM的文件数据缓冲区,缓冲区长度不小于一次读写的数据长度,起始地址必须为8字节边界地址 */
#else
__align(8)	UINT8  FILE_DATA_BUF[ FILE_DATA_BUF_LEN ];	/* 外部RAM的文件数据缓冲区,缓冲区长度不小于一次读写的数据长度,起始地址必须为8字节边界地址 */
#endif
//#pragma RESTORE
UINT8	CH374FileRead( void )		/* 从当前文件读取数据 */
{
	mCmdParam.ReadX.mDataBuffer = &FILE_DATA_BUF[0];  /* 指向文件数据缓冲区 */
	return( CH374FileReadX( ) );
}
#ifdef EN_DISK_WRITE
UINT8	CH374FileWrite( void )		/* 向当前文件写入数据 */
{
	mCmdParam.WriteX.mDataBuffer = &FILE_DATA_BUF[0];  /* 指向文件数据缓冲区 */
	return( CH374FileWriteX( ) );
}
#endif
#endif

/* 以下程序可以根据需要修改 */

#ifndef NO_DEFAULT_CH374_IO			/* 在应用程序中定义NO_DEFAULT_CH374_IO可以禁止默认的CH374接口程序,然后用自行编写的程序代替它 */
#define CH374_WRITE_INDEX( c )		{ CH374_IDX_PORT = ( c ); }	/* 写索引地址 */
#define CH374_WRITE_DATA( c )		{ CH374_DAT_PORT = ( c ); }	/* 写数据且地址增量 */
#define CH374_READ_DATA( )			( CH374_DAT_PORT )			/* 读数据且地址增量 */
#define CH374_READ_DATA0( )			( CH374_IDX_PORT )			/* 读数据且地址不增量 */
#endif

#ifndef NO_DEFAULT_CH374_F_ENUM			/* 在应用程序中定义NO_DEFAULT_CH374_F_ENUM可以禁止默认的枚举文件程序,然后用自行编写的程序代替它 */
UINT8	CH374FileEnumer( void )			/* 枚举文件 */
{
	UINT8	status;
	status = CH374FileOpen( );
	if ( status == ERR_FOUND_NAME ) status = ERR_SUCCESS;  /* 操作成功 */
	return( status );
}
#endif

#ifndef NO_DEFAULT_CH374_F_QUERY		/* 在应用程序中定义NO_DEFAULT_CH374_F_QUERY可以禁止默认的查询当前文件的信息程序,然后用自行编写的程序代替它 */
UINT8	CH374FileQuery( void )			/* 查询当前文件的信息 */
{
	PUINT8	buf;
	UINT8	count;
	buf = (PUINT8)( & mCmdParam.Modify.mFileSize );
	for ( count = sizeof( mCmdParam.Modify ); count != 0; count -- ) {
		*buf = 0xFF;  /* 输入参数全部无效,仅查询不修改 */
		buf ++;
	}
	return( CH374FileModify( ) );
}
#endif

#ifndef NO_DEFAULT_CH374_RESET			/* 在应用程序中定义NO_DEFAULT_CH374_RESET可以禁止默认的CH374复位程序,然后用自行编写的程序代替它 */
void	CH374Reset( void )				/* 复位CH374,硬件复位后需调用CH374Init或者CH374LibInit重新初始化 */
{
	CH374_WRITE_REGISTER( REG_SYS_CTRL, 0 );  /* 清控制寄存器 */
	CH374_WRITE_REGISTER( REG_SYS_CTRL, BIT_CTRL_RESET_NOW );  /* 芯片软件复位控制 */
	CH374DelaymS( 5 );  /* 延时至少5毫秒 */
	CH374_WRITE_REGISTER( REG_SYS_CTRL, 0 );  /* 结束复位 */
	CH374DelaymS( 5 );  /* 延时至少5毫秒 */
}
#endif

#ifndef NO_DEFAULT_CH374_INT			/* 在应用程序中定义NO_DEFAULT_CH374_INT可以禁止默认的中断处理程序,然后用自行编写的程序代替它 */
#if LIB_CFG_INT_EN == 0					/* CH374的INT#引脚连接方式为"查询方式" */
void xQueryInterrupt( void )			/* 查询CH374中断 */
{
#ifdef CH374_INT_WIRE					/* 连接了CH374的中断引脚 */
	while ( CH374_INT_WIRE );  /* 如果CH374的中断引脚输出高电平则等待 */
#else									/* 未连接CH374的中断引脚 */
	while ( ( CH374_READ_REGISTER( REG_INTER_FLAG ) & ( BIT_IF_DEV_DETECT | BIT_IF_TRANSFER ) ) == 0 );  /* 查询中断标志寄存器 */
#endif
}
#else									/* LIB_CFG_INT_EN != 0, CH374的INT#引脚连接方式为"中断方式" */
void xQueryInterrupt( void )			/* 查询中断状态,等待硬件中断 */
{
	while ( CH374IntStatus == 0 );  /* 子程序库调用该子程序之前CH374IntStatus=0,硬件中断后,由中断服务程序置为非0的实际中断状态后返回 */
}
void	CH374Interrupt( void )	__irq	/* CH374中断服务程序,使用寄存器组1,由CH374的INT#的低电平或者下降沿触发单片机中断 */
{
	CH374IntStatus = 1;  /* 已接收到CH374中断则置标志 */
#ifdef CLEAR_INT_MARK
	CLEAR_INT_MARK( );  /* 某些单片机需要由软件清除中断标志 */
#endif
}
#endif
#endif

#ifdef EN_DISK_WRITE
#ifndef NO_DEFAULT_DELAY_WRITE			/* 在应用程序中定义NO_DEFAULT_DELAY_WRITE可以禁止默认的写操作后延时程序,然后用自行编写的程序代替它 */
void xDelayAfterWrite( void )			/* 写操作后延时 */
{
#if LIB_CFG_NO_DLY == 0					/* 写后延时 */
	UINT32	count;
	for ( count = 2600; count != 0; count -- );  /* 延时200uS左右,4x20nS@50MHz */
#endif
}
#endif
#endif

#ifndef NO_DEFAULT_FILE_ENUMER			/* 在应用程序中定义NO_DEFAULT_FILE_ENUMER可以禁止默认的文件名枚举回调程序,然后用自行编写的程序代替它 */
void xFileNameEnumer( void )			/* 文件名枚举回调子程序 */
{
/* 如果指定枚举序号CH374vFileSize为0xFFFFFFFF后调用FileOpen，那么每搜索到一个文件FileOpen都会调用本回调程序，
   回调程序xFileNameEnumer返回后，FileOpen递减CH374vFileSize并继续枚举直到搜索不到文件或者目录。建议做法是，
   在调用FileOpen之前定义一个全局变量为0，当FileOpen回调本程序后，本程序由CH374vFdtOffset得到结构FAT_DIR_INFO，
   分析结构中的DIR_Attr以及DIR_Name判断是否为所需文件名或者目录名，记录相关信息，并将全局变量计数增量，
   当FileOpen返回后，判断返回值如果是ERR_MISS_FILE或ERR_FOUND_NAME都视为操作成功，全局变量为搜索到的有效文件数。
   如果在本回调程序xFileNameEnumer中将CH374vFileSize置为1，那么可以通知FileOpen提前结束搜索。以下是回调程序例子 */
#if 0
#ifdef FILE_DATA_BUF_LEN
	UINT8			i;
	UINT16			FileCount;
	P_FAT_DIR_INFO	pFileDir;
	PUINT8			NameBuf;
	pFileDir = (P_FAT_DIR_INFO)( pDISK_BASE_BUF + CH374vFdtOffset );  /* 当前FDT的起始地址 */
	FileCount = (UINT16)( 0xFFFFFFFF - CH374vFileSize );  /* 当前文件名的枚举序号,CH374vFileSize初值是0xFFFFFFFF,找到文件名后递减 */
	if ( FileCount < FILE_DATA_BUF_LEN / 12 ) {  /* 检查缓冲区是否足够存放,假定每个文件名需占用12个字节存放 */
		NameBuf = & FILE_DATA_BUF[ FileCount * 12 ];  /* 计算保存当前文件名的缓冲区地址 */
		for ( i = 0; i < 11; i ++ ) NameBuf[ i ] = pFileDir -> DIR_Name[ i ];  /* 复制文件名,长度为11个字符,未处理空格 */
		if ( pFileDir -> DIR_Attr & ATTR_DIRECTORY ) NameBuf[ i ] = 1;  /* 判断是目录名 */
		NameBuf[ i ] = 0;  /* 文件名结束符 */
	}
#endif
#endif
}
#endif

#ifdef EXT_SEC_INTERFACE
UINT8 xDiskSectorAccess( PUINT32 mLba, UINT8 mMode )	/* 该子程序由CH374的子程序库调用,用于以扇区为单位存取磁盘 */
{
#if 0
	UINT8	SectCnt;
	UINT8	OpCmd;
	UINT32	StartLba;
	PUINT8	DataBuf;
	StartLba = *mLba;  /* 将读写的起始扇区号 */
	switch ( mMode ) {
		case 0:  /* 读一个扇区到pDISK_BASE_BUF */
			SectCnt = 1;
			DataBuf = pDISK_BASE_BUF;
			OpCmd = CMD_READ_SECTOR;
/* 如果操作失败必须调用CH374DirtyBuffer( ) */
			break;
		case 1:  /* 从pDISK_BASE_BUF写一个扇区 */
			SectCnt = 1;
			DataBuf = pDISK_BASE_BUF;
			OpCmd = CMD_WRITE_SECTOR;
/* 如果操作失败必须调用CH374DirtyBuffer( ) */
			break;
		case 2:  /* 读mCmdParam.ReadX.mLbaCount个扇区到mCmdParam.ReadX.mDataBuffer */
			SectCnt = mCmdParam.ReadX.mLbaCount;
			DataBuf = mCmdParam.ReadX.mDataBuffer;
			OpCmd = CMD_READ_SECTOR;
/* 必须更新mCmdParam.ReadX.mDataBuffer(累计缓冲区地址)和CH374vCurrentOffset(累计文件指针)和mCmdParam.ReadX.mSectorCount(累计扇区数) */
			break;
		case 3:  /* 从mCmdParam.WriteX.mDataBuffer写mCmdParam.WriteX.mLbaCount个扇区 */
			SectCnt = mCmdParam.WriteX.mLbaCount;
			DataBuf = mCmdParam.WriteX.mDataBuffer;
			OpCmd = CMD_WRITE_SECTOR;
/* 必须更新mCmdParam.WriteX.mDataBuffer(累计缓冲区地址)和CH374vCurrentOffset(累计文件指针)和mCmdParam.WriteX.mSectorCount(累计扇区数) */
			break;
		default:
			return( ERR_CH374_ERROR );  /* 未知操作 */
	}
/* 以扇区为单位进行读写操作,起始扇区号为StartLba,扇区数为SectCnt,缓冲区为DataBuf */
#else
	mLba++;
	mMode++;
#endif
	return( ERR_SUCCESS );
}
#endif

UINT8	CH374LibInit( void )  /* 初始化CH374程序库和CH374芯片,操作成功返回0 */
{	  RS232_PutString("\r\nZZ--2\r\n");
	if ( CH374GetVer( ) < CH374_LIB_VER ) return( 0xFF );  /* 获取当前子程序库的版本号,版本太低则返回错误 */
#if DISK_BASE_BUF_LEN
	pDISK_BASE_BUF = & DISK_BASE_BUF[0];  /* 指向外部RAM的磁盘数据缓冲区 */
#endif
    RS232_PutString("\r\nZZ--2\r\n");
	return( CH374Init( ) );  /* 初始化CH374 */
}

#endif

#ifdef __cplusplus
}
#endif

#endif
