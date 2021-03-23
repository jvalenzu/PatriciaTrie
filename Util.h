#include <stdlib.h>

// ------------------------------------------------------------------------------------------------
char* bitstring(char* dest, size_t len, const char* str, size_t min = 0);

template<int len>
static char* bitstring(char (&dest)[len], const char* str, size_t min = 0)
{
    return bitstring(dest, len, str, min);
}

// ------------------------------------------------------------------------------------------------

constexpr bool get_bit(const char* str, unsigned bit)
{
    return (str[bit >> 3] >> (7-(bit & 7)) & 1) == 1;
}

// ------------------------------------------------------------------------------------------------
// a | 10000000
// b | 00000000
// c | 01000000
// d | 00000000
//
// a and b differ in bit 0
// c and d differ in bit 1
unsigned first_differing_bit(const char* a, const char* key);
