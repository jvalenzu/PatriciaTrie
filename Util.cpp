#include "Util.h"

#include <assert.h>
#include <string.h>

#include "xxhashct/xxh32.hpp"

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

// ------------------------------------------------------------------------------------------------
void split_line(std::vector<token>* dest, const char* str)
{
    const char* sep = " \r\n";
    const char* last = str + strlen(str);
    const size_t sep_len = strlen(sep);

    auto add_word = [dest](const char* str, size_t len)
    {
        token t;
        t.m_Value = str;
        t.m_Len = len;
        t.m_Hash = hash(str, len);
        dest->push_back(t);
    };

    auto is_sep = [sep, sep_len](char c)
    {
        for (int i = 0; i < sep_len; ++i)
        {
            if (sep[i] == c)
                return true;
        }
        return false;
    };

    while (str < last)
    {
        const char* start = str;
        while (true)
        {
            if (!*start)
                break;
            
            if (is_sep(*start))
                break;
            
            start++;
        }

        add_word(str, start - str);
        while (*start && is_sep(*start))
            start++;
        
        str = start;
    }
}
