#pragma once

#include <ctype.h>
#include <string.h>

struct PTrie
{
    struct Node
    {
        static unsigned s_DebugIndex;
        unsigned m_TestBit;
        unsigned m_DebugIndex;
        const char* m_Value;
        struct Node* m_Left;
        struct Node* m_Right;

        Node(unsigned testBit, const char* value)
            : m_TestBit(testBit)
            , m_DebugIndex(s_DebugIndex++)
            , m_Value(value)
            , m_Left(nullptr)
            , m_Right(nullptr)
        {
        }
    };
    
    Node* m_Root = nullptr;
};

void               patricia_trie_init(PTrie* ptrie);
void               patricia_trie_fini(PTrie* ptrie);

const PTrie::Node* patricia_trie_search(const PTrie* ptrie, const char* key);
bool               patricia_trie_exact_search(const PTrie* ptrie, const char* key);
void               patricia_trie_insert(PTrie* ptrie, const char* key);

void               patricia_trie_visit(const PTrie::Node* node, const void* key, size_t key_size, void (*func)(const void* key, size_t key_size, const PTrie::Node*));
