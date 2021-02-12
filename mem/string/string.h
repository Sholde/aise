#ifndef _STRING_H_
#define _STRING_H_

#define STRING_MAX_SIZE 100

typedef struct string_s
{
  int size;
  char str[STRING_MAX_SIZE];
} string;

string *str_create(const char *);
void str_print(string *);
void str_copy(string *, string *);
void str_destroy(string *);

#endif
