#ifndef FAKES_H_
#define FAKES_H_

#include "fff/fff.h"

#include <stdio.h>
#include <stddef.h>

#include <fcntl.h>
#include <unistd.h>

DECLARE_FAKE_VALUE_FUNC_VARARG(int, open, const char*, int, ...);
DECLARE_FAKE_VALUE_FUNC(int, close, int);
DECLARE_FAKE_VALUE_FUNC(ssize_t, read, int, void*, size_t);
DECLARE_FAKE_VALUE_FUNC(ssize_t, write, int, const void*, size_t);

#endif /* FAKES_H_ */
