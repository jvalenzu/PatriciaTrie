#include "PatriciaTrie.h"
#include "Util.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <set>

// todo
// only works with string literals, string poolify
// get rid of root node convention
// "baked" API
// can get rid of the memcmp in prefix searching if we bump up the testbit to character boundaries.
// test ascii 7 bit instead of 8

unsigned PTrie::Node::s_DebugIndex;

// ------------------------------------------------------------------------------------------------
void patricia_trie_init(PTrie* ptrie)
{
}

// ------------------------------------------------------------------------------------------------
void patricia_trie_fini(PTrie* ptrie)
{
}

// ------------------------------------------------------------------------------------------------
static const PTrie::Node* s_patricia_trie_search_internal(const PTrie* ptrie, const char* key)
{
    const unsigned last_key_bit = 8 * strlen(key);
    const PTrie::Node* found = nullptr;
    const PTrie::Node* parent = ptrie->m_Root;
    
    {
        if (!parent)
            return nullptr;
        
        found = parent->m_Left;
        
        while (parent->m_TestBit < found->m_TestBit && found->m_TestBit <= last_key_bit)
        {
            parent = found;
            
            const bool goRight = get_bit(key, found->m_TestBit) ? true : false;
            const PTrie::Node* next = goRight ? found->m_Right : found->m_Left;
            found = next;
        }
    }

    return found;
}

// ------------------------------------------------------------------------------------------------
const PTrie::Node* patricia_trie_search(const PTrie* ptrie, const char* key)
{
    const PTrie::Node* found = s_patricia_trie_search_internal(ptrie, key);
    if (found && strstr(found->m_Value, key))
        return found;
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
bool patricia_trie_exact_search(const PTrie* ptrie, const char* key)
{
    const PTrie::Node* found = s_patricia_trie_search_internal(ptrie, key);
    if (found)
    {
        const size_t len0 = strlen(key);
        const size_t len1 = strlen(found->m_Value);
        const size_t len2 = (len0 < len1) ? len0 : len1;
        return !memcmp(found->m_Value, key, len2);
    }
    return false;
}

// ------------------------------------------------------------------------------------------------
void patricia_trie_insert(PTrie* ptrie, const char* key)
{
    const PTrie::Node* found = s_patricia_trie_search_internal(ptrie, key);

    // found an exact match?
    if (found && !strcmp(found->m_Value, key))
        return;
    
    {
        char* v = strdup(key);
        for (int i = strlen(key); i>0; --i)
        {
            char c = tolower(v[i]);
            if (isalpha(c))
                v[i] = c;
            else
                v[i] = '\0';
        }
        
        key = v;
    }
    
    if (!ptrie->m_Root)
    {
        ptrie->m_Root = new PTrie::Node(first_differing_bit("", key), key);
        ptrie->m_Root->m_Left = ptrie->m_Root;
        return;
    }
    
    const unsigned differingBit = first_differing_bit(found->m_Value, key);
    
    PTrie::Node* parent = ptrie->m_Root;
    PTrie::Node* child = ptrie->m_Root->m_Left;
    
    while (parent->m_TestBit < child->m_TestBit && child->m_TestBit < differingBit)
    {
        parent = child;
        
        const bool goRight = get_bit(key, child->m_TestBit) ? true : false;
        child = goRight ? child->m_Right : child->m_Left;
    }
    
    PTrie::Node* newNode = new PTrie::Node(differingBit, key);

    {
        const bool goRight = get_bit(key, differingBit) ? true : false;
        if (goRight)
        {
            newNode->m_Left = child;
            newNode->m_Right = newNode;
        }
        else
        {
            newNode->m_Left = newNode;
            newNode->m_Right = child;            
        }
    }

    if (!get_bit(key, parent->m_TestBit) || parent == ptrie->m_Root)
    {
        parent->m_Left = newNode;
    }
    else
    {
        parent->m_Right = newNode;
    }
}

// ------------------------------------------------------------------------------------------------
static void patricia_trie_visit_internal(std::set<const PTrie::Node*> *seen,
                                         const PTrie::Node* node,
                                         const unsigned tb,
                                         const void* key, size_t key_size,
                                         void (*func)(const void* key, size_t key_size, const PTrie::Node*))
{
    if (node->m_TestBit < tb)
    {
        func(key, key_size, node);
        return;
    }
    
    auto contains = [&seen](const PTrie::Node* node) -> bool
    {
        return seen->find(node) != seen->end();
    };
    
    seen->insert(node);

    if (node->m_Left && !contains(node->m_Left))
    {
        patricia_trie_visit_internal(seen, node->m_Left, tb, key, key_size, func);
    }
    
    func(key, key_size, node);
    
    if (node->m_Right && !contains(node->m_Right))
    {
        patricia_trie_visit_internal(seen, node->m_Right, tb, key, key_size, func);
    }
}

// ------------------------------------------------------------------------------------------------
void patricia_trie_visit(const PTrie::Node* node, const void* key, size_t key_size, void (*func)(const void* key, size_t key_size, const PTrie::Node*))
{
    if (node)
    {
        std::set<const PTrie::Node*> seen;
        const unsigned min = node->m_TestBit;
        patricia_trie_visit_internal(&seen, node, min, key, key_size, func);
    }
}
