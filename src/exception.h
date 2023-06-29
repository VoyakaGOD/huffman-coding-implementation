#ifndef EXCEPTION_HEADER
#define EXCEPTION_HEADER

#include <stdlib.h>

#define THROW_EXCEPTION(message) throw_exception(message)
#define S_ALLOC(count, type) (type *)s_alloc(count, sizeof(type))

typedef int (* exception_handler_t)(const char *message);

void set_exception_handler(exception_handler_t _handler);
void throw_exception(const char *message);
void *s_alloc(size_t count, size_t size);

#endif //EXCEPTION_HEADER