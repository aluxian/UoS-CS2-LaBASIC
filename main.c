#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "util/delay.h"
#include "os.h"

#define BASIC_ON 1
#define BASIC_OFF 0

struct BasicVariable {
  int is_set;
  char name[30];
  int value;
};

struct BasicVariable VARS[100];

char *PROG[] = {
  "PRINT 'hello world'",
  "PRINT ENDL",
  "GOTO 5",
  "PRINT 'you should never see this'",
  "PRINT 'hello again'",
  "LED OFF",
  "PAUSE 500ms",
  "LED ON",
  "PAUSE 500ms",
  "LED OFF",
  "PRINT ENDL",
  "PRINT 'cool'",
  "PRINT ENDL"
};

int PRGCNT = 0;
int PRGSIZ = sizeof(PROG) / sizeof(PROG[0]);

void str_trim_whitespace(char str[]) {
  while (str[0] == ' ' || str[0] == '\n') {
    memmove(str, str + 1, strlen(str));
  }

  while (str[strlen(str) - 1] == ' ' || str[strlen(str) - 1] == '\n') {
    str[strlen(str) - 1] = '\0';
  }
}

int str_starts_with(char str[], char name[]) {
  return strncmp(name, str, strlen(name)) == 0;
}

void str_trim_beginning(char str[], int n) {
  memmove(str, str + n, strlen(str));
}

void str_trim_end(char str[], int n) {
  str[strlen(str) - n] = '\0';
}

void str_get_until_whitespace(char str[], char buf[]) {
  int i = 0;
  while (i < strlen(str)) {
    if (str[i] == ' ') break;
    buf[i] = str[i];
    i++;
  }
  buf[i] = '\0';
}

int parse_on_off(char str[]) {
  if (str_starts_with(str, "ON")) return BASIC_ON;
  if (str_starts_with(str, "OFF")) return BASIC_OFF;
  return -1;
}

int b_vars_add(char name[], int value) {
  for (int i = 0; i < 100; i++) {
    if (!VARS[i].is_set) {
      VARS[i].is_set = 1;
      strcpy(VARS[i].name, name);
      VARS[i].value = value;
      break;
    }
  }
}

void do_print(char _str[]) {
  char str[100];
  strcpy(str, _str);
  
  str_trim_beginning(str, strlen("PRINT"));
  str_trim_whitespace(str);
  
  if (str[0] == '\'' && str[strlen(str) - 1] == '\'') {
    str_trim_beginning(str, 1);
    str_trim_end(str, 1);
    display_string(str);
  } else if (str_starts_with(str, "ENDL")) {
    display_string("\n");
  } else {
    display_string("ERR");
  }
}

void do_pause(char _str[]) {
  _delay_ms(500);
}

void do_led(char _str[]) {
  char str[100];
  strcpy(str, _str);
  
  str_trim_beginning(str, strlen("LED"));
  str_trim_whitespace(str);

  int newState = parse_on_off(str);
  if (newState == BASIC_ON) os_led_brightness(255);
  if (newState == BASIC_OFF) os_led_brightness(0);
}

void do_let(char _str[]) {
  char str[100];
  strcpy(str, _str);
  
  str_trim_beginning(str, strlen("LET"));
  str_trim_whitespace(str);

  char name[30];
  str_get_until_whitespace(str, name);
  str_trim_whitespace(str);

  if (str[0] != '=') {
    display_string("ERR");
    return;
  }

  str_trim_beginning(str, strlen("="));
  str_trim_whitespace(str);

  int value = atoi(str);
  b_vars_add(name, value);
}

void do_goto(char _str[]) {
  char str[100];
  strcpy(str, _str);
  
  str_trim_beginning(str, strlen("GOTO"));
  str_trim_whitespace(str);

  int lineNum = atoi(str);
  PRGCNT = lineNum - 2;
}

void interpret() {
  while (0 <= PRGCNT && PRGCNT < PRGSIZ) {
    if (str_starts_with(PROG[PRGCNT], "PRINT")) do_print(PROG[PRGCNT]);
    if (str_starts_with(PROG[PRGCNT], "PAUSE")) do_pause(PROG[PRGCNT]);
    if (str_starts_with(PROG[PRGCNT], "LED")) do_led(PROG[PRGCNT]);
    if (str_starts_with(PROG[PRGCNT], "LET")) do_let(PROG[PRGCNT]);
    if (str_starts_with(PROG[PRGCNT], "GOTO")) do_goto(PROG[PRGCNT]);
    PRGCNT++;
  }
}

void main(void) {
  os_init();

  for (int i = 0; i < 100; i++) {
    VARS[i].is_set = 0;
  }

  f_mount(&FatFs, "", 0);
  //display_string(">>> running program\n");
  interpret();
  //display_string(">>> done\n");

  sei();
  for (;;) {}
}
