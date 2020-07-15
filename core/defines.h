#ifndef DEFINES_H
#define DEFINES_H

#ifdef __EMSCRIPTEN__
# include <emscripten.h>
#else
# define EMSCRIPTEN_KEEPALIVE
#endif

#define PASTE(x, y) x ## y
#define CONCAT(x, y) PASTE(x, y)

#define GETMASK(index, size) (((1U << (size)) - 1) << (index))
#define READFROM(data, index, size) (((data) & GETMASK((index), (size))) >> (index))
#define WRITE(data, index, size, value) ((data) = ((data) & (~GETMASK((index), (size)))) | ((uint32_t)(value) << (index)))

#define write8(data, index, value) WRITE(data, index, 8, value)
#define read8(data, index) READFROM(data, index, 8)

#ifndef __has_builtin
# define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_expect)
# define unlikely(x) (__builtin_expect((x), 0))
# define likely(x)   (!unlikely(!(x)))
#else
# define unlikely(x) (x)
# define likely(x)   (x)
#endif

#endif
