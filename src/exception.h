#ifndef EXCEPTION_HEADER
#define EXCEPTION_HEADER

#include <stdlib.h>

#define THROW_EXCEPTION(message) throw_exception(message)

typedef int (* exception_handler_t)(const char *message);

void set_exception_handler(exception_handler_t _handler);
void throw_exception(const char *message);

#endif //EXCEPTION_HEADER