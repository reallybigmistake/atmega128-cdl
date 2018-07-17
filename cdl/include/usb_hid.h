#ifndef __USB_HID__
#define __USB_HID__
extern struct usb_device_descriptor dev_descriptor;
extern struct usb_device_descriptor* p_dev_descriptor;

extern struct usb_config_descriptor cfg_descriptor;
extern struct usb_config_descriptor* p_cfg_descriptor;

extern struct usb_interface_descriptor interf_descriptor;
extern struct usb_interface_descriptor* p_interf_descriptor;

extern struct usb_string_descriptor string_descriptor;
extern struct usb_string_descriptor* p_string_descriptor;
#endif
