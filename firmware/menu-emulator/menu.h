#pragma once
#include <stdint.h>
#include <stdio.h>
//////////////////////////////////////
extern int serial_port;
int cdc_send(uint8_t *buf, uint16_t len);
int cdc_recv(uint8_t *buf, uint16_t len);
void cdc_putchar(uint8_t);
void cdc_getchar(uint8_t *);
void HAL_Delay(uint32_t delay);
//////////////////////////////////////
#define LEN(X) sizeof(X) / sizeof(X[0])
#define MV 0.75
#define MH 0.3
#define DESCMARG 3
#define TITLEMARG 5
#define ESC 033
#define CLEAR [2J
//////////////////////////////////////
typedef struct {
  uint8_t *title;
  uint8_t *desc;    // desciption
  uint8_t property; // some id for color for later use
  uint8_t title_len;
  uint16_t desc_len; // not used
  const uint8_t (*fptr)(void);
  uint8_t isSolved;
} Menuitem;
//////////////////////////////////////
uint8_t menu_handler();
