#ifndef EMBEDDED_CDC_WRAPPER_H
#define EMBEDDED_CDC_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

void* CDC_create();
void CDC_destroy(void* thisCDC);
void CDC_receive(void* thisCDC, uint8_t* buf, const uint16_t len);
void CDC_transmit(void* thisCDC, uint8_t* buf, const uint16_t len);

#ifdef __cplusplus
}
#endif

#endif //EMBEDDED_CDC_WRAPPER_H
