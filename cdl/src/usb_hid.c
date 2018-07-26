#include "usb_ch9.h"
#include "usb_hid.h"

struct usb_device_descriptor dev_descriptor={
    .bLength =              USB_DT_DEVICE_SIZE,
    .bDescriptorType =      USB_DT_DEVICE,
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


struct usb_config_descriptor cfg_descriptor={
    .bLength =              USB_DT_CONFIG_SIZE,
	.bDescriptorType =      USB_DT_CONFIG,

	.wTotalLength =         0x00,
	.bNumInterfaces =       0x01,
	.bConfigurationValue =  0x01,
	.iConfiguration =       0x00,
	.bmAttributes =         USB_CONFIG_ATT_ONE,
	.bMaxPower =            0x32,   
};
struct usb_config_descriptor* p_cfg_descriptor = &cfg_descriptor;

struct usb_interface_descriptor interf_descriptor = {
    .bLength =              USB_DT_INTERFACE_SIZE,
	.bDescriptorType =      USB_DT_INTERFACE,

	.bInterfaceNumber =     0x00,
	.bAlternateSetting =    0x00,
	.bNumEndpoints =        0x01,
	.bInterfaceClass =      USB_CLASS_HID,//HID
	.bInterfaceSubClass =   0x01,
	.bInterfaceProtocol =   0x02,//mouse
	.iInterface =           0x00,
};
struct usb_interface_descriptor* p_interf_descriptor = &interf_descriptor;


struct usb_class_hid_descriptor hid_descriptor = {
    .bLength = 		0x09,
	.bDescriptorType = 	USB_DESCRIPTOR_TYPE_HID,
	.bcdCDC =               0x0110,
	.bCountryCode = 	0x00,
    .bNumDescriptors =      0x01,	/* 0x01 */
	.bDescriptorType0 = 	USB_DESCRIPTOR_TYPE_REPORT,
	.wDescriptorLength0 = 	0x002E,
};
struct usb_class_hid_descriptor* p_hid_descriptor = &hid_descriptor;


struct usb_endpoint_descriptor ep_descriptor = {
	.bLength =              USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =      USB_DT_ENDPOINT,

	.bEndpointAddress =     0x81,
	.bmAttributes   =       USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =       0x0008,
	.bInterval =            0x0A,     
};
struct usb_endpoint_descriptor* p_ep_descriptor = &ep_descriptor;



struct usb_class_report_descriptor report_descriptor;
struct usb_class_report_descriptor* p_report_descriptor =  &report_descriptor;

struct usb_string_descriptor string_descriptor;
struct usb_string_descriptor* p_string_descriptor = &string_descriptor;


struct usb_config_descriptor_total config_total = {
    .cfg_descriptor = {
        .bLength =              USB_DT_CONFIG_SIZE,
        .bDescriptorType =      USB_DT_CONFIG,

        .wTotalLength =         USB_DT_CONFIG_SIZE\
                            +   USB_DT_INTERFACE_SIZE\
                            +   sizeof(struct usb_class_hid_descriptor)\
                            +   USB_DT_ENDPOINT_SIZE,
        .bNumInterfaces =       0x01,
        .bConfigurationValue =  0x01,
        .iConfiguration =       0x00,
        .bmAttributes =         USB_CONFIG_ATT_ONE,
        .bMaxPower =            0x32,
    },
    .interf_descriptor = {
        .bLength =              USB_DT_INTERFACE_SIZE,
        .bDescriptorType =      USB_DT_INTERFACE,

        .bInterfaceNumber =     0x00,
        .bAlternateSetting =    0x00,
        .bNumEndpoints =        0x01,
        .bInterfaceClass =      USB_CLASS_HID,//HID
        .bInterfaceSubClass =   0x01,
        .bInterfaceProtocol =   0x02,//mouse
        .iInterface =           0x00,
    },
    .hid_descriptor = {
        .bLength =              sizeof(struct usb_class_hid_descriptor),
        .bDescriptorType =      USB_DESCRIPTOR_TYPE_HID,
        .bcdCDC =               0x0110,
        .bCountryCode =         0x00,
        .bNumDescriptors =      0x01,   /* 0x01 */
        .bDescriptorType0 =     USB_DESCRIPTOR_TYPE_REPORT,
        .wDescriptorLength0=    0x002E,
    },
    .ep_descriptor = {
        .bLength =          USB_DT_ENDPOINT_SIZE,
        .bDescriptorType =  USB_DT_ENDPOINT,

        .bEndpointAddress = 0x81,
        .bmAttributes   =   USB_ENDPOINT_XFER_INT,
        .wMaxPacketSize =   0x0008,
        .bInterval =        0x0A,   
    },
};
struct usb_config_descriptor_total* p_config_total = &config_total;