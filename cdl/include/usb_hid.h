#ifndef __USB_HID__
#define __USB_HID__

struct usb_config_descriptor_total {
    struct usb_config_descriptor cfg_descriptor;
    struct usb_interface_descriptor interf_descriptor;
    struct usb_class_hid_descriptor hid_descriptor;
    struct usb_endpoint_descriptor ep_descriptor;
}__attribute__ ((packed));

extern struct usb_device_descriptor dev_descriptor;
extern struct usb_device_descriptor* p_dev_descriptor;

extern struct usb_config_descriptor cfg_descriptor;
extern struct usb_config_descriptor* p_cfg_descriptor;

extern struct usb_interface_descriptor interf_descriptor;
extern struct usb_interface_descriptor* p_interf_descriptor;

extern struct usb_class_hid_descriptor hid_descriptor;
extern struct usb_class_hid_descriptor* p_hid_descriptor;

extern struct usb_endpoint_descriptor ep_descriptor;
extern struct usb_endpoint_descriptor* p_ep_descriptor;

extern struct usb_string_descriptor string_descriptor;
extern struct usb_string_descriptor* p_string_descriptor;

extern struct usb_config_descriptor_total config_total;
extern struct usb_config_descriptor_total* p_config_total;
#endif