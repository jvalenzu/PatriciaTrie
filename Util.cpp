#include "Util.h"

#include <assert.h>
#include <string.h>

// ------------------------------------------------------------------------------------------------
char* bitstring(char* dest, size_t len, const char* str, size_t min)
{
    char* base = dest;
    char c;
    while (c = *str++)
    {
        for (int i=0; i<8; ++i)
        {
            *dest++ = c&0x80 ? '1' : '0';
            c <<= 1;
        }
    }
    
    if (min)
    {
        for (int i = (dest - base)/8; i<min; ++i)
        {
            strcpy(dest, "00000000");
            dest += 8;
        }
    }
    
    *dest = '\0';
    return base;
}

// ------------------------------------------------------------------------------------------------
unsigned first_differing_bit(const char* a, const char* key)
{
    const char* base = key;
    while (*a == *key)
    {
        a++;
        key++;
        continue;
    }
    assert(*a != *key);
    const unsigned c = (*a ^ *key) & 0xff;
    return (key - base) * 8 + __builtin_clz(c) - 24;
}
