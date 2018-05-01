#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "util/delay.h"
#include "os.h"

void trim_whitespace(char str[]) {
  while (str[0] == ' ' || str[0] == '\n') {
    memmove(str, str + 1, strlen(str));
  }

  while (str[strlen(str) - 1] == ' ' || str[strlen(str) - 1] == '\n') {
    str[strlen(str) - 1] = '\0';
  }
}

void do_print(char _str[]) {
  char str[100];
  strcpy(str, _str);
  
  memmove(str, str + strlen("PRINT"), strlen(str));
  trim_whitespace(str);
  
  if (str[0] == '\'' && str[strlen(str) - 1] == '\'') {
    memmove(str, str + 1, strlen(str));
    str[strlen(str) - 1] = '\0';
    display_string(str);
  } else if (strncmp("ENDL", str, strlen("ENDL")) == 0) {
    display_string("\n");
  } else {
    display_string("ERR");
  }
}

void do_pin(char _str[]) {
  char str[100];
  strcpy(str, _str);
  
  memmove(str, str + strlen("PIN"), strlen(str));
  trim_whitespace(str);

  char num1[10] = "";
  char num2[10] = "";
  
  while (isdigit(str[0])) {
    char tmpdigit[] = "0";
    tmpdigit[0] = str[0];
    strcat(num1, tmpdigit);
    memmove(str, str + 1, strlen(str));
  }

  trim_whitespace(str);

  while (isdigit(str[0])) {
    char tmpdigit[2] = "0";
    tmpdigit[0] = str[0];
    strcat(num2, tmpdigit);
    memmove(str, str + 1, strlen(str));
  }

  int n1 = atoi(num1);
  int n2 = atoi(num2);
}

void do_pinmode(char _str[]) {

}

void do_pause(char _str[]) {
  _delay_ms(1000);
}

void do_led_on(char _str[]) {
  LED_ON;
}

void do_led_off(char _str[]) {
  LED_OFF;
}

void interpret_line(char str[]) {
  if (strncmp("PRINT", str, strlen("PRINT")) == 0) do_print(str);
  if (strncmp("PIN", str, strlen("PIN")) == 0) do_pin(str);
  if (strncmp("PINMODE", str, strlen("PINMODE")) == 0) do_pinmode(str);
  if (strncmp("PAUSE", str, strlen("PAUSE")) == 0) do_pause(str);
  if (strncmp("LED_ON", str, strlen("LED_ON")) == 0) do_led_on(str);
  if (strncmp("LED_OFF", str, strlen("LED_OFF")) == 0) do_led_off(str);
}

void interpret_prog(char *prog[], int n_lines) {
  for (int i = 0; i < n_lines; i++) {
    interpret_line(prog[i]);
  }
}

void main(void) {
  os_init();

  char *prog[] = {
    "PRINT 'hello world'",
    "LED_OFF",
    "PAUSE 500",
    "PRINT 'cool'"
  };

  int lines = 2;

  f_mount(&FatFs, "", 0);
  interpret_prog(prog, lines);

  sei();
  for (;;) {}
}
