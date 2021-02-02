#ifndef _ALLOC_DEBUGGER_H_
#define _ALLOC_DEBUGGER_H_

void *malloc(size_t size);
//void free(void *ptr);
__attribute((destructor)) void finalize(void);

#endif
