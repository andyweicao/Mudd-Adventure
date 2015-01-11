#ifndef _VGA_LED_H
#define _VGA_LED_H

// Wei Cao(wc2467), Mingru Xu(mx2151)

#include <linux/ioctl.h>

#define VGA_LED_DIGITS 8


typedef struct {
  unsigned int column;
  unsigned int data1;
  unsigned int data2;  
  
} vga_led_screen;


typedef struct {
  unsigned int cox;
  unsigned int coy;
} vga_led_ball;


typedef struct {
  unsigned char digit;    /* 0, 1, .. , VGA_LED_DIGITS - 1 */
  unsigned char segments; /* LSB is segment a, MSB is decimal point */
} vga_led_arg_t;

#define VGA_LED_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_LED_WRITE_DIGIT _IOW(VGA_LED_MAGIC, 1, vga_led_arg_t *)
#define VGA_LED_READ_DIGIT  _IOWR(VGA_LED_MAGIC, 2, vga_led_arg_t *)
#define VGA_LED_WRITE_BALL  _IOW(VGA_LED_MAGIC, 3, vga_led_arg_t *)
#define VGA_LED_WRITE_SCREEN  _IOW(VGA_LED_MAGIC, 4, vga_led_arg_t *)

#endif
