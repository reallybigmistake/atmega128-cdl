#include "usb_ch9.h"
#include "usb_hid.h"

struct usb_device_descriptor dev_descriptor={
        .bLength =              0x12,
        .bDescriptorType =      0x01,
        .bcdUSB =               0x0200,
        .bDeviceClass =         0x00,
        .bDeviceSubClass =      0x00,
        .bDeviceProtocol =      0x00,
        .bMaxPacketSize0 =      0x08,
        .idVendor =             0x413c,//Dell^^
        .idProduct =            0x0000,
        .bcdDevice =            0x0001,
        .iManufacturer =        0x01,
        .iProduct =             0x01,
        .iSerialNumber =        0x00,
        .bNumConfigurations =   0x01,
};
struct usb_device_descriptor* p_dev_descriptor = &dev_descriptor;


struct usb_config_descriptor cfg_descriptor;
struct usb_config_descriptor* p_cfg_descriptor = &cfg_descriptor;

struct usb_interface_descriptor interf_descriptor;
struct usb_interface_descriptor* p_interf_descriptor = &interf_descriptor;

struct usb_string_descriptor string_descriptor;
struct usb_string_descriptor* p_string_descriptor = &string_descriptor;
