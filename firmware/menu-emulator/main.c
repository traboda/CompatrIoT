#include "menu.h"
#include <errno.h> // Error integer and strerror() function
#include <fcntl.h> // Contains file controls like O_RDWR
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()

int serial_port;

int cdc_send(uint8_t *buf, uint16_t len) {
  write(serial_port, buf, len);
  return 0;
}
int cdc_recv(uint8_t *buf, uint16_t len) {
  read(serial_port, buf, len);
  return 0;
}
void cdc_putchar(uint8_t x) {
#ifdef SLOW
	HAL_Delay(5);
#endif
	cdc_send(&x, 1); }
void cdc_getchar(uint8_t *x) { cdc_recv(x, 1); }
void HAL_Delay(uint32_t delay) {
  usleep(delay * 1000);
  return;
}
int main(int argc, char **argv) {
  if (argc != 2)
    return 1;
  serial_port = open(argv[1], O_RDWR);
  struct termios tty;
  if (tcgetattr(serial_port, &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in
                          // communication (most common)
  tty.c_cflag &= ~CSIZE;  // Clear all bits that set the data size
  tty.c_cflag |= CS8;     // 8 bits per byte (most common)
  tty.c_cflag &=
      ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |=
      CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;   // Disable echo
  tty.c_lflag &= ~ECHOE;  // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                   ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g.
                         // newline chars)
  tty.c_oflag &=
      ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT
  // PRESENT ON LINUX) tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars
  // (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon
                        // as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);
  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    return 1;
  }
  int mode = menu_handler();

  close(serial_port);
  return 0; // success
};
