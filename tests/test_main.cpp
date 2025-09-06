//
//https://github.com/meekrosoft/fff
//
#include "fff/fff.h"
DEFINE_FFF_GLOBALS;

#include "fakes.h"

DEFINE_FAKE_VALUE_FUNC_VARARG(int, open, const char*, int, ...);
DEFINE_FAKE_VALUE_FUNC(int, close, int);
DEFINE_FAKE_VALUE_FUNC(ssize_t, read, int, void*, size_t);
DEFINE_FAKE_VALUE_FUNC(ssize_t, write, int, const void*, size_t);
