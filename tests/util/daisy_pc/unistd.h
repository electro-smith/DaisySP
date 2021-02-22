#pragma once
#ifndef __DSY_UNISTD_STUB_H__
#define __DSY_UNISTD_STUB_H__

#if defined(_WIN32)

#include <io.h>
//constexpr auto write = _write;

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2


#endif

#endif //__DSY_UNISTD_STUB_H__