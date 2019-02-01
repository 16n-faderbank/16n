#include <usb_names.h>

#define PRODUCT_NAME \
  {                  \
    '1', '6', 'n'    \
  }
#define PRODUCT_NAME_LEN 3

struct usb_string_descriptor_struct usb_string_product_name = {
    2 + PRODUCT_NAME_LEN * 2,
    3,
    PRODUCT_NAME};
