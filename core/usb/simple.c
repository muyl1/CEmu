#include "device.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dump(void *data, size_t size) {
    uint8_t *ptr;
    size_t remaining;
    for (ptr = data, remaining = size; remaining--; ptr++)
        printf("%02X ", *ptr);
    for (remaining = size; remaining < 64; remaining++)
        printf("   ");
    putchar('|');
    for (ptr = data, remaining = size; remaining--; ptr++)
        putchar(*ptr >= ' ' && *ptr <= '~' ? *ptr : '.');
    putchar('|');
    putchar('\n');
}

int usb_simple_device(usb_event_t *event) {
    static const uint8_t setup[][8] = {
        {0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x08, 0x00},
        {0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x12, 0x00},
        {0x80, 0x06, 0x00, 0x02, 0x00, 0x00, 0x40, 0x00},
        {0x80, 0x06, 0x01, 0x02, 0x00, 0x00, 0x40, 0x00},
        {0x80, 0x06, 0x02, 0x02, 0x00, 0x00, 0x40, 0x00},
        {0x80, 0x06, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x00},
        {0x80, 0x06, 0x01, 0x03, 0x09, 0x04, 0xFF, 0x00},
        {0x80, 0x06, 0x02, 0x03, 0x09, 0x04, 0xFF, 0x00},
        {0x00, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
    };
    static const uint8_t bufSizeReq[] = {0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x04, 0x00};

    usb_event_type_t type = event->type;
    event->type = USB_INIT_EVENT;
    uintptr_t state = (uintptr_t)event->context;
    switch (type) {
        case USB_INIT_EVENT:
            state = 0;
            break;
        case USB_TRANSFER_EVENT:
            if (event->info.transfer.endpoint == 2 &&
                !event->info.transfer.setup && !event->info.transfer.direction)
                switch (state) {
                    case 21:
                        memcpy(event->info.transfer.buffer, bufSizeReq, event->info.transfer.length);
                        break;
                }
            break;
        case USB_DESTROY_EVENT:
            return 0;
    }
    switch (state) {
        case 0:
        case 2:
        case 4:
        case 6:
        case 8:
        case 10:
        case 12:
        case 14:
        case 16:
        case 18:
            event->type = USB_TRANSFER_EVENT;
            event->info.transfer.buffer = (uint8_t *)setup[state >> 1];
            event->info.transfer.length = sizeof(*setup);
            event->info.transfer.endpoint = 0;
            event->info.transfer.setup = true;
            event->info.transfer.direction = setup[state >> 1][0] >> 7;
            break;
    }
    dump(event->info.transfer.buffer, event->info.transfer.length);
    switch (type) {
        case USB_INIT_EVENT:
        case USB_TRANSFER_EVENT:
            switch (state) {
                case 1:
                case 3:
                case 5:
                case 7:
                case 9:
                case 11:
                case 13:
                case 15:
                case 17:
                case 19:
                    event->type = USB_TRANSFER_EVENT;
                    event->info.transfer.length = 0;
                    event->info.transfer.endpoint = 0;
                    event->info.transfer.setup = false;
                    event->info.transfer.direction = !(setup[state >> 1][0] >> 7);
                    break;
                case 20:
                    event->type = USB_TRANSFER_EVENT;
                    event->info.transfer.length = sizeof(bufSizeReq);
                    event->info.transfer.endpoint = 2;
                    event->info.transfer.setup = false;
                    event->info.transfer.direction = false;
                    break;
                case 21:
                    event->type = USB_TRANSFER_EVENT;
                    event->info.transfer.length = 0x40;
                    event->info.transfer.endpoint = 1;
                    event->info.transfer.setup = false;
                    event->info.transfer.direction = true;
                    break;
                case 22:
                    event->type = USB_DESTROY_EVENT;
                    return 0;
            }
            break;
    }
    event->context = (void *)++state;
    return 0;
}
