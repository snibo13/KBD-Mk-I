#include "tusb.h"
#include "usb_descriptors.h"

tusb_desc_device_t const desc_device =
    {
        .bLength = sizeof(tusb_desc_device_t), // Size of description in bytes
        .bDescriptorType = TUSB_DESC_DEVICE,   // 0x01 Only one device descriptor for USB device
        .bcdUSB = 0x0200,                      // USB Type
        .bDeviceClass = TUSB_CLASS_HID,        // Try TUSB_CLASS_MISC
        .bDeviceSubClass = HID_SUBCLASS_NONE,  // MISC_SUBCLASS_COMMON
        .bDeviceProtocol = MISC_PROTOCOL_IAD,  //??
        .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

        .idVendor = 0xFEED, // Standard from QMK 0xCAFE for TinyUSB
        .idProduct = 0x2131,
        .bcdDevice = 0x0100, // Release number

        .iManufacturer = 0x01,
        .iProduct = 0x02,
        .iSerialNumber = 0x03,

        .bNumConfigurations = 0x01,
};

// Invoked when descriptor requested
uint8_t const *tud_descriptor_device_cb(void)
{
  return (uint8_t const *)&desc_device;
}

uint8_t const desc_hid_report[] =
    {
        TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
        TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(REPORT_ID_MOUSE)),
        TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL)),
        TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(REPORT_ID_GAMEPAD))};

// Invoked when looking for HID descriptor ??
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
  (void)instance;
  return desc_hid_report;
}

// Callbacks for reporting
// uint16_t const *tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
  (void)instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT && report_id == REPORT_ID_KEYBOARD)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    // bufsize should be (at least) 1
    if (bufsize < 1)
      return;

    uint8_t const kbd_leds = buffer[0];

    // if (caps_cb)
    // {
    //   caps_cb(kbd_leds & KEYBOARD_LED_CAPSLOCK);
    // }
  }
}

enum
{
  ITF_NUM_HID,
  ITF_NUM_TOTAL // Interface number
};

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

#define EPNUM_HID 0x01

uint8_t const desc_configuration[] =
    {
        // Config number, number of interfaces, string index, total length, attribute, power in ma
        TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

        // Interface number, string index, protocol, descriptor len, EP in address, size and polling interval
        TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_KEYBOARD, sizeof(desc_hid_report), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5)};

// Reporting device_qualifier and other_speed_configuration for high speed operation
#if TUD_OPT_HIGH_SPEED

uint8_t desc_other_speed_config[CONFIG_TOTAL_LEN];

// device qualifier
tusb_desc_device_qualifier_t const desc_device_qualifier =
    {
        .bLength = sizeof(tusb_desc_device_qualifier_t),
        .bDescriptorType = TUSB_DESC_DEVICE_QUALIFIER,
        .bcdUSB = 0x0200,

        .bDeviceClass = 0x00,
        .bDeviceSubClass = 0x00,
        .bDeviceProtocol = 0x00,

        .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
        .bNumConfigurations = 0x01,
        .bReserved = 0x00};

// Invoked when qualifier descriptor is requested
uint8_t const *tud_descriptor_device_qualifier_cb(void)
{
  return (uint8_t const *)&desc_device_qualifier;
}

// Invoked when get other speed descriptor requested
uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
  (void)index;

  return desc_configuration;
}

#endif // highspeed

char const *string_desc_arr[] =
    {
        (const char[]){0x09, 0x04},
        "Scatterbrain",
        "KBD Mk I"};

static uint16_t _desc_str[32];

// Invoked on get strind descriptor
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void)langid;

  uint8_t chr_count;

  if (index == 0)
  {
    memccpy(&_desc_str[1], string_desc_arr[0], 2, sizeof(_desc_str));
    chr_count = 1;
  }
  else
  {
    if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
      return NULL;

    const char *str = string_desc_arr[index];

    chr_count = strlen(str);

    // Cap at max char
    chr_count = strlen(str);
    if (chr_count > 31)
      chr_count = 31;

    for (uint8_t i = 0; i < chr_count; i++)
    {
      _desc_str[1 + i] = str[i];
    }
  }

  _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);
}