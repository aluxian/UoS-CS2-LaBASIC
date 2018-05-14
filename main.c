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
  "LED ON",
  "PAUSE 1500",
  "LED OFF",
  "PAUSE 800",
  "LED ON",
  "PRINT ENDL",
  "PRINT 'cool. creating a var now'",
  "PRINT ENDL",
  "LET test_var = 5",
  "PRINT 'it is = '",
  "PRINT $test_var",
  "PRINT ENDL",
  "PRINT 'reading input from encoder'",
  "PRINT ENDL",
  "READ input_var",
  "PRINT 'read value is = '",
  "PRINT $input_var",
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

void str_pad_right(char str[], char c, int n) {
  int i = strlen(str);
  while (n > 0) {
    str[i] = c;
    i++;
    n--;
  }
  str[i] = '\0';
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
      return i;
    }
  }

  return -1;
}

int b_vars_get(char name[]) {
  for (int i = 0; i < 100; i++) {
    if (strcmp(name, VARS[i].name) == 0) {
      return VARS[i].value;
    }
  }

  return -1;
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
  } else if (str_starts_with(str, "$")) {
    str_trim_beginning(str, 1);
    int value = b_vars_get(str);
    char buf[10];
    itoa(value, buf, 10);
    display_string(buf);
  } else {
    display_string("PRINT_ERR");
  }
}

void do_pause(char _str[]) {
  char str[100];
  strcpy(str, _str);
  
  str_trim_beginning(str, strlen("PAUSE"));
  str_trim_whitespace(str);

  int target_ms = atoi(str);
  for (int ms = 0; ms < target_ms; ms += 50) {
    _delay_ms(50);
  }
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
  str_trim_beginning(str, strlen(name));
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

void do_read(char _str[]) {
  char str[100];
  strcpy(str, _str);
  
  str_trim_beginning(str, strlen("READ"));
  str_trim_whitespace(str);

  char name[30];
  str_get_until_whitespace(str, name);

  int cvalue = 0;
  display_string("0");

  while (!get_switch_short(_BV(SWC))) {
    cvalue += os_enc_delta();
    if (cvalue < 0) cvalue = 0;
    char buf[30];
    itoa(cvalue, buf, 10);
    str_pad_right(buf, ' ', 10);
    display_string_xy(buf, 0, display.y);
    _delay_ms(50);
  }

  display_string("\n");
  b_vars_add(name, cvalue);
}

void interpret() {
  while (0 <= PRGCNT && PRGCNT < PRGSIZ) {
    if (str_starts_with(PROG[PRGCNT], "PRINT")) do_print(PROG[PRGCNT]);
    if (str_starts_with(PROG[PRGCNT], "PAUSE")) do_pause(PROG[PRGCNT]);
    if (str_starts_with(PROG[PRGCNT], "LED")) do_led(PROG[PRGCNT]);
    if (str_starts_with(PROG[PRGCNT], "LET")) do_let(PROG[PRGCNT]);
    if (str_starts_with(PROG[PRGCNT], "GOTO")) do_goto(PROG[PRGCNT]);
    if (str_starts_with(PROG[PRGCNT], "READ")) do_read(PROG[PRGCNT]);
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
