#include "exception.h"

static exception_handler_t handler = NULL;

void set_exception_handler(exception_handler_t _handler)
{
    handler = _handler;
}

void throw_exception(const char *message)
{
    if(!handler)
        exit(-1);
    exit(handler(message));
}