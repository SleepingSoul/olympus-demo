#pragma once

#include <logging/logging.h>

#ifdef _DEBUG

#define olyError(format, ...)\
oly::logging::error(format, __VA_ARGS__);\
_CrtDbgBreak()\

#else

#define olyError(format, ...) oly::logging::error(format, __VA_ARGS__)

#endif
