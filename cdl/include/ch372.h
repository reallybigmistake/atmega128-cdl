#ifndef __CH372_H__
#define __CH372_H__
#include <inttypes.h>
/*
define commands use default configration
*/
#define RESET_ALL       0x05    //执行硬件复位
#define CHECK_EXIST     0x06    //测试工作状态
#define SET_USB_ID      0x12    //设置 USB 的厂商识别码 VID和产品识别码 PID(必须在 SET_USB_MODE 命令之前执行)
#define SET_USB_MODE    0x15    //设置 USB 工作模式
#define GET_STATUS      0x22    //获取中断状态并取消请求
#define UNLOCK_USB      0x23    //释放当前 USB 缓冲区
#define RD_USB_DATA     0x28    //从当前 USB 中断的端点缓冲区读取数据块并释放当前缓冲区
#define WR_USB_DATA5    0x2A    //向 USB 端点 1 的上传缓冲区写入数据块
#define WR_USB_DATA7    0x2B    //向 USB 端点 2 的上传缓冲区写入数据块
#define GET_IC_VER      0x01    //获取芯片及固件版本
#define ENTER_SLEEP     0x03    //进入低功耗睡眠挂起状态
#define CHK_SUSPEND     0x0B    //设置检查 USB 总线挂起状态的方式
#define RD_USB_DATA0    0x27    //从当前 USB 中断的端点缓冲区读取数据块
/*
define commands use custom configration
*/
#define SET_USB_ADDR    0x13    //设置 USB 地址
#define SET_ENDP2       0x18    //设置 USB 端点 0 的接收器
#define SET_ENDP3       0x19    //设置 USB 端点 0 的发送器
#define SET_ENDP4       0x19    //设置 USB 端点 1 的接收器
#define SET_ENDP5       0x1B    //设置 USB 端点 1 的发送器
#define SET_ENDP6       0x1C    //设置 USB 端点 2 的接收器
#define SET_ENDP7       0x1D    //设置 USB 端点 2 的发送器
#define GET_TOGGLE      0x0A    //获取当前 OUT 事务同步状态
#define WR_USB_DATA3    0x29    //向 USB 端点 0 的上传缓冲区写入数据块
/*
*usb endpoint work mode
*/
#define ACK_MODE_OFFSET     0
#define RESV_OFFSET         4
#define SYNC_TRIG_OFFSET    6
#define ACK_MODE_MASK   0
#define RESV_MASK       4
#define SYNC_TRIG_MASK  6
/*
define status
*/
#define CMD_RET_SUCCESS     0x51    //操作成功
#define CMD_RET_ABORT       0x5F    //操作失败
/*
define interrupt status*/
#define USB_INT_BUS_RESET1  0x03    //检测到 USB 总线复位
#define USB_INT_BUS_RESET2  0x07    //检测到 USB 总线复位
#define USB_INT_BUS_RESET3  0x0B    //检测到 USB 总线复位
#define USB_INT_BUS_RESET4  0x0F    //检测到 USB 总线复位
#define USB_INT_EP0_SETUP   0x0C    //端点 0 的接收器接收到数据，SETUP 成功
#define USB_INT_EP0_OUT     0x00    //端点 0 的接收器接收到数据，OUT 成功
#define CMD_RET_SUCCESS     0x51    //端点 0 的发送器发送完数据，IN 成功
#define USB_INT_EP1_OUT     0x01    //辅助端点/端点 1 接收到数据，OUT 成功
#define USB_INT_EP1_IN      0x09    //中断端点/端点 1 发送完数据，IN 成功
#define USB_INT_EP2_OUT     0x02    //批量端点/端点 2 接收到数据，OUT 成功
#define USB_INT_EP2_IN      0x0A    //批量端点/端点 2 发送完数据，IN 成功
#define USB_INT_USB_SUSPEND 0x05    //USB 总线挂起事件（如果已 CHK_SUSPEND）
#define USB_INT_WAKE_UP     0x06    //从睡眠中被唤醒事件（如果已 ENTER_SLEEP）

typedef enum{
    MODE_OFFLINE,   //default mode
    MODE_CUSTOM_FIRMWARE,
    MODE_INSIDE_FIRMWARE,
}E_USB_MODE;
/*
*interrupt mode
*/
#define TOKEN_OFFSET    2
#define ENDP_OFFSET     0
#define TOKEN_MASK  3<<TOKEN_OFFSET
#define ENDP_MASK   3<<ENDP_OFFSET

#define IC_VER_MASK 0x3f<<0

/*
*io to ch372
*/
#define PORTDATA    0
#define PORTCTRL    2
#define PIN_INT     0
#define PIN_A0      1
#define PIN_RD      2
#define PIN_WR      3
#define PIN_CS

#define INT_SIG     0
#define RD_EN       0
#define WR_EN       0
#define SEL_DATA    0
#define SEL_CMD     1  


#define EP0_BUF_SIZE 8
#define EP1_BUF_SIZE 8
#define EP2_BUF_SIZE 64
#define EP_MAX_SIZE  EP2_BUF_SIZE 

/*apis*/
int check_exist(char val1);
void set_usb_mode(E_USB_MODE mode);
void usb_ep0_setup();
void usb_ep0_out();
void usb_ep0_in();
void usb_ep1_out();
void usb_ep1_in();
void usb_ep2_out();
void usb_ep2_in();
void usb_suspend();
void usb_wakeup();
void usb_reset();
#endif
/*
单片机通过 CH372 芯片接收数据的处理步骤如下：
①、 当 CH372 接收到 USB 主机发来的数据后，首先锁定当前 USB 缓冲区，防止被后续数据覆盖，
然后将 INT#引脚设置为低电平，向单片机请求中断；
②、 单片机进入中断服务程序，首先执行 GET_STATUS 命令获取中断状态；
③、 CH372 在 GET_STATUS 命令完成后将 INT#引脚恢复为高电平，取消中断请求；
④、 由于通过上述 GET_STATUS 命令获取的中断状态是“下传成功”，所以单片机执行
RD_USB_DATA 命令从 CH372 读取接收到的数据；
⑤、 CH372 在 RD_USB_DATA 命令完成后释放当前缓冲区，从而可以继续 USB 通讯；
⑥、 单片机退出中断服务程序。
单片机通过 CH372 芯片发送数据的处理步骤如下：
①、 单片机执行 WR_USB_DATA 命令向 CH372 写入要发送的数据；
②、 CH372 被动地等待 USB 主机在需要时取走数据；
③、 当 USB 主机取走数据后，CH372 首先锁定当前 USB 缓冲区，防止重复发送数据，然后将 INT#
引脚设置为低电平，向单片机请求中断；
④、 单片机进入中断服务程序，首先执行 GET_STATUS 命令获取中断状态；
⑤、 CH372 在 GET_STATUS 命令完成后将 INT#引脚恢复为高电平，取消中断请求；
⑥、 由于通过上述 GET_STATUS 命令获取的中断状态是“上传成功”，所以单片机执行
WR_USB_DATA 命令向 CH372 写入另一组要发送的数据，如果没有后续数据需要发送，那么
单片机不必执行 WR_USB_DATA 命令；
⑦、 单片机执行 UNLOCK_USB 命令；
⑧、 CH372 在 UNLOCK_USB 命令完成后释放当前缓冲区，从而可以继续 USB 通讯；
⑨、 单片机退出中断服务程序；
⑩、 如果单片机已经写入了另一组要发送的数据，那么转到②，否则结束。*/