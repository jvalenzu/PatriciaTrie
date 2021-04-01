#include <stdlib.h>
#include <stdint.h>
#include <vector>

#include "xxhashct/xxh32.hpp"

int constexpr length(const char* str)
{
    return *str ? 1 + length(str + 1) : 0;
}

uint32_t constexpr hash(const char* str, uint32_t len = 0)
{
    return xxh32::hash(str, len ? len : length(str), 0);
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

struct token
{
    const char* m_Value;
    uint32_t m_Hash;
    size_t m_Len;
};

void split_line(std::vector<token>* dest, const char* str);
