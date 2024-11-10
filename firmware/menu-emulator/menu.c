#include "menu.h"
#include "menu-config.h"
#include "ansi.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void redraw();
void clear();
void put_int(uint_fast16_t);

uint8_t chosen = 0;
uint_fast16_t xmax, ymax; // screen sizee
uint_fast16_t xmid;
uint_fast16_t ymid;
uint_fast16_t state=0;
uint_fast16_t page=0;


static uint8_t seq_clear[4] = {"\033[2J"};
static uint8_t seq_curpos[4] = {"\033[6n"};
/* colors */
uint8_t seq_bgcolor[7] = {"\033[42;1m"};
uint8_t seq_reset[4] = {"\033[0m"};
uint8_t seq_fgcolor[7] = FGGREEN;


/********STATE VARIABLES********/
int8_t selection = 0;
//////////////////////////////////////
void move(uint_fast16_t y, uint_fast16_t x) {
  cdc_putchar(0x1b);
  cdc_putchar('[');
  put_int(y);
  cdc_putchar(';');
  put_int(x);
  cdc_putchar('H');
}
//////////////////////////////////////
void clear() { cdc_send(seq_clear, LEN(seq_clear)); }
//////////////////////////////////////
/*print int as string*/
void put_int(uint_fast16_t i) {
  uint_fast16_t ii;

  if (i >= 10) {
    if (i >= 100) {
      ii = i / 100;
      cdc_putchar(ii + '0');
      i -= 100 * ii;
    }

    ii = i / 10;
    cdc_putchar(ii + '0');
    i -= 10 * ii;
  }
  cdc_putchar(i + '0');
}
//////////////////////////////////////
void getxy(uint_fast16_t *y, uint_fast16_t *x) {
  *y = 0;
  *x = 0;
  cdc_send(seq_curpos, LEN(seq_curpos));
  // requested
  uint8_t i;
  while (1) {
    cdc_getchar(&i);
    if (i != 0x1b)
      continue;
    cdc_getchar(&i); //[
    cdc_getchar(&i);
    if (i > 57 || i < 48)
      continue;
    while (i != ';') {
      *y = ((*y) * 10) + (i - '0');
      cdc_getchar(&i);
    }
    cdc_getchar(&i);
    while (i != 'R') {
      *x = ((*x) * 10) + (i - '0');
      cdc_getchar(&i);
    }
    if (i == 'R') {
      return;
    } else {
      *x = 0;
      *y = 0;
    }
  }
}
//////////////////////////////////////
void drawbox(float mv, float mh) {
  move(1000, 1000);
  getxy(&ymax, &xmax);
  xmid = xmax * mh; //vertical line
  ymid = ymax * mv; //horizontal line

  for (uint_fast16_t i = 1; i <= ymax; i++) {
    move(i, xmid);
    cdc_putchar('|');
  }

  for (uint_fast16_t i = xmid + 1; i <= xmax; i++) {
    move(ymid, i);
    cdc_putchar('_');
  }
}
//////////////////////////////////////
/* return 1 if changed else 0 */
/* TODO: change function name */
uint8_t getxymax() {
  move(399, 399);
  uint_fast16_t x = xmax, y = ymax;
  getxy(&ymax, &xmax);
  return x != xmax || y != ymax;
}
//////////////////////////////////////
void drawlist() {
  move(1, 4);
  cdc_send(seq_bgcolor, LEN(seq_bgcolor));
  cdc_send(seq_reset, LEN(seq_reset));
}
//////////////////////////////////////
void reset() { cdc_send(seq_reset, LEN(seq_reset)); }
void setbg() { cdc_send(seq_bgcolor, LEN(seq_bgcolor)); }
void drawlab() {
  Menuitem *m;
  move(2, lmargin);
  cdc_send(FGGREEN,LEN(FGGREEN));
  cdc_send(" TRAINING LABS ",15);
  reset();
  /////////BUG CHANGE//////////
  uint_fast16_t labmax = ((ymax - 5)/2)-1;
  if(selection <= labmax){
    for(uint_fast16_t i=0;i<= labmax && i< LEN(menu); i++){
      m = &menu[i];
      if((LEN(menu))*2 +1+3 >= ymax){
      move(ymax, lmargin+6);
      cdc_send(seq_fgcolor,LEN(seq_fgcolor));
      cdc_send("\\/",2);
      reset();
      }
      if (i == selection){
        move(((i + 1) * 2)-1 +3, lmargin);
        cdc_send(seq_fgcolor,LEN(seq_fgcolor));
        cdc_send("▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄",42);
        move(((i + 1) * 2)+1 +3, lmargin);
        cdc_send("▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀",42);
        reset();
        setbg();
      }
      move((i + 1) * 2 +3, lmargin);
      cdc_send(" Lab - ",7);
      put_int(i);
      if((m->property & 0xF0)==0x00)
        cdc_send(" UART",6);
      else if(m->property >> 4 == 1)
        cdc_send(" JTAG",6);
      else if(m->property >> 4 == 2)
        cdc_send(" I2C ",6);
      else if(m->property >> 4 == 3)
        cdc_send(" SPI ",6);
      else if(m->property >> 4 == 4)
        cdc_send(" BLE ",6);
      if(i<=9)
        cdc_send(" ",1);

      reset();
    }
  }
  else {
    for(uint_fast16_t i=labmax+1;i< LEN(menu); i++){
      m = &menu[i];
      if((LEN(menu))*2 +1+3 >= ymax){
      move(4, lmargin+6);
      cdc_send(seq_fgcolor,LEN(seq_fgcolor));
      cdc_send("/\\",2);
      reset();
      }
      if (i == selection){
        move(((i-labmax + 1) * 2)-1 +3, lmargin);
        cdc_send(seq_fgcolor,LEN(seq_fgcolor));
        cdc_send("▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄",42);
        move(((i-labmax + 1) * 2)+1 +3, lmargin);
        cdc_send("▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀",42);
        reset();
        setbg();
      }
      move((i-labmax + 1) * 2 +3, lmargin);
      cdc_send(" Lab - ",7);
      put_int(i);
      if((m->property & 0xF0)==0x00)
        cdc_send(" UART",6);
      else if(m->property >> 4 == 1)
        cdc_send(" JTAG",6);
      else if(m->property >> 4 == 2)
        cdc_send(" I2C ",6);
      else if(m->property >> 4 == 3)
        cdc_send(" SPI ",6);
      else if(m->property >> 4 == 4)
        cdc_send(" BLE ",6);
      if(i<=9)
        cdc_send(" ",1);

      reset();
    }
  }
  ////////BUG CHANGE///////////
}
//////////////////////////////////////
void drawlabdetials() {
  Menuitem *m;
  m = &menu[selection];
  uint_fast16_t start = 0;
  int_fast16_t len = xmax-xmid-(DESCMARG*2);
  uint_fast16_t y = ((1 - MH) * xmax) / 2 - m->title_len / 2;

  cdc_send(seq_fgcolor, LEN(seq_fgcolor));
  move(2,xmid+y);
  cdc_send(m->title, m->title_len);
  cdc_send(seq_reset, LEN(seq_reset));

  for (uint_fast16_t i = 1; ; i++) {
    move(i+5,xmid+DESCMARG+1);
    cdc_send(&m->desc[start], (i*len >= m->desc_len) ? (m->desc_len - start) : len );
    start = i*len ;
    if(start >= m->desc_len)  break;
  }
}
//////////////////////////////////////
void drawfooter() {
//  _____                             _      _____    _____
// /  __ \                           | |    |_   _|  |_   _|
// | /  \/ ___  _ __ ___  _ __   __ _| |_ _ __| |  ___ | |
// | |    / _ \| '_ ` _ \| '_ \ / _` | __| '__| | / _ \| |
// | \__/\ (_) | | | | | | |_) | (_| | |_| | _| || (_) | |
//  \____/\___/|_| |_| |_| .__/ \__,_|\__|_| \___/\___/\_/
//                       | |
//                       |_|

  static uint8_t compatriot[8][57] = {
  {" _____                             _      _____    _____ "},
  {"/  __ \\                           | |    |_   _|  |_   _|"},
  {"| /  \\/ ___  _ __ ___  _ __   __ _| |_ _ __| |  ___ | |  "},
  {"| |    / _ \\| '_ ` _ \\| '_ \\ / _` | __| '__| | / _ \\| |  "},
  {"| \\__/\\ (_) | | | | | | |_) | (_| | |_| | _| || (_) | |  "},
  {" \\____/\\___/|_| |_| |_| .__/ \\__,_|\\__|_| \\___/\\___/\\_/  "},
  {"                      | |                                "},
  {"                      |_|                                "},
  };
  uint_fast16_t y = ((1 - MH) * xmax) / 2 - 57 / 2;
  uint_fast16_t x = ((1 - MV) * ymax) / 4;

  // CompatrIoT
  cdc_send(seq_fgcolor, LEN(seq_fgcolor));
  for (uint_fast16_t i = 0; i < 8; i++) {
    move(ymid+i+1+x,xmid+y);  cdc_send(compatriot[i], 57);
  }

  cdc_send(FGRED, LEN(FGRED));
  y = ((1 - MH) * xmax) / 2 - 17 / 2;
  move(ymid + x + 8,xmid+y+10);
  cdc_send(tag_line, LEN(tag_line));
  cdc_send(seq_reset, LEN(seq_reset));
  move(399,399);

}
//////////////////////////////////////
// Changes the selection based on arrow key input
uint_fast8_t input_handler() {
  uint8_t c;
  while (1) {
    cdc_getchar(&c);
    if (c == '\033') {
      cdc_getchar(&c);
      if (c == '[') {
        cdc_getchar(&c);
        if (c == 'A') {
          selection = (selection - 1);
          if(selection== -1)  selection = LEN(menu)-1;
          return 1;
        } else if (c == 'B') {
          selection = (selection + 1) % (LEN(menu) - 0);
          return 1;
        } else {
          continue;
        }
      }
    } else {
      continue;
    }
  }

  return 0;
}
//////////////////////////////////////
uint8_t menu_handler() {
  redraw();
  while (1) {
    if (input_handler()) { }
    redraw();
  }
  /* HAL_Delay(50); */
  return chosen;
}
//////////////////////////////////////
void redraw() {
  clear();
  getxymax();
  static uint8_t error[] = "Increase screen size";
  if(ymax <= 30 || xmax <= 95){
    move(ymax/2, xmax/2-10);
    cdc_send(error, LEN(error));
    move(ymax/2+1, xmax/2-10);
    cdc_send("press any arrow key", 20);
  }
  else{
    drawbox(MV,MH);     // Layout
    drawlab();          // Lab list
    drawlabdetials();   // Lab Description
    drawfooter();       // Footer
  }
}
//////////////////////////////////////
