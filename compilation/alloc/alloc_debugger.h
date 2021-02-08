#ifndef _ALLOC_DEBUGGER_H_
#define _ALLOC_DEBUGGER_H_

#define MAX_ALLOC 20

typedef struct alloc_table_s
{
  size_t count;
  void *ptr[MAX_ALLOC];
  size_t size[MAX_ALLOC];
} alloc_table_t;

void *malloc(size_t size);
void free(void *ptr);
__attribute((constructor)) void init(void);
__attribute((destructor)) void finalize(void);

#endif
