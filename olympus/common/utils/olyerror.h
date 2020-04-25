#pragma once

#ifdef _DEBUG
#include <logging/logging.h>

#define olyError(format, ...)\
oly::logging::error(format, __VA_ARGS__);\
_CrtDbgBreak();\

#else

#define olyError(format, ...) oly::logging::error(format, __VA_ARGS__);

#endif
