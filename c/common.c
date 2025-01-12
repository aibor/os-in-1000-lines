#include "common.h"

void putchar(char ch);

void
memset(void *buf, const char c, size_t n) {
  // Optimization: use largest available size first.
  char v[sizeof(long)] = {[0 ... sizeof(long) - 1] = c};
  while (n >= sizeof(long)) {
    *(long *)buf = *(long *)v;
    buf += sizeof(long);
    n -= sizeof(long);
  }

  while (n--) {
    *(uint8_t *)buf++ = c;
  }
}

void
memcpy(void *dst, const void *src, size_t n) {
  // Optimization: use largest available size first.
  while (n >= sizeof(long)) {
    *(long *)dst = *(long *)src;
    dst += sizeof(long);
    src += sizeof(long);
    n -= sizeof(long);
  }

  while (n--) {
    *(uint8_t *)dst++ = *(const uint8_t *)src++;
  }
}

void
strcpy(char *dst, const char *src) {
  while (*src) {
    *dst++ = *src++;
  }

  *dst = '\0';
}

int
strcmp(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    if (*s1 != *s2) {
      break;
    }
    s1++;
    s2++;
  }

  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

void
printf(const char *fmt, ...) {
  va_list vargs;
  va_start(vargs, fmt);

  while (*fmt) {
    if (*fmt == '%') {
      fmt++;          // Skip '%'
      switch (*fmt) { // Read the next character
      case '\0':      // '%' at the end of the format string
        putchar('%');
        goto end;
      case '%': // Print '%'
        putchar('%');
        break;
      case 's': { // Print a NULL-terminated string.
        const char *s = va_arg(vargs, const char *);
        while (*s) {
          putchar(*s);
          s++;
        }
        break;
      }
      case 'd': { // Print an integer in decimal.
        int value = va_arg(vargs, int);
        if (value < 0) {
          putchar('-');
          value = -value;
        }

        int divisor = 1;
        while (value / divisor > 9) {
          divisor *= 10;
        }

        while (divisor > 0) {
          putchar('0' + value / divisor);
          value %= divisor;
          divisor /= 10;
        }

        break;
      }
      case 'x': { // Print an integer in hexadecimal.
        int value = va_arg(vargs, int);
        for (int i = 7; i >= 0; i--) {
          int nibble = (value >> (i * 4)) & 0xf;
          putchar("0123456789abcdef"[nibble]);
        }
      }
      }
    } else {
      putchar(*fmt);
    }

    fmt++;
  }

end:
  va_end(vargs);
}