
#ifndef _LED_H_
#define _LED_H_

#define LED_RED   0
#define LED_GREEN 1

extern void led_write(int num, int state);
extern void led_init(void);

#endif
