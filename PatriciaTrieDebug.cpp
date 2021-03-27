#include "PatriciaTrie.h"
#include "PatriciaTrieDebug.h"
#include "Util.h"

#include <assert.h>
#include <stdio.h>
#include <stack>
#include <set>

// ------------------------------------------------------------------------------------------------
static void debug_label(char* display_label, size_t max, unsigned debug_index)
{
    snprintf(display_label, max, "n%x", debug_index);
}


// ------------------------------------------------------------------------------------------------
void patricia_trie_dump(const PTrie* ptrie)
{
    auto string_format = [ptrie](char label[32], const PTrie::Node* node)
    {
        if (node == nullptr)
        {
            snprintf(label, 32, "");
        }
        else if (node == ptrie->m_Root)
        {
            snprintf(label, 32, "root");
        }
        else
        {
            snprintf(label, 32, "node%p", node);
        }
    };
    
    auto add_attribute = [string_format,ptrie](const PTrie::Node* node) {
        char label[32];
        char left_label[32];
        char right_label[32];
        char buf[128];
        
        auto bitstring1 = [&buf](const char* str)
        {
            return bitstring(buf, str);
        };
        
        string_format(label, node);
        string_format(left_label, node->m_Left);
        string_format(right_label, node->m_Right);
        
        char display_label[32];
        if (node == ptrie->m_Root)
        {
            strncpy(display_label, "root", sizeof display_label);
        }
        else
        {
            debug_label(display_label, sizeof display_label, node->m_DebugIndex);
        }
        
        printf("    %s [ label = \"<f0> %s | <f1> testBit: %d | <f3> count %d | <f2> value:'%s' \n bits: %s\" ]\n",
               label, display_label, node->m_TestBit, node->m_Count, node->m_Value, bitstring1(node->m_Value));
        
        printf("    %s:f0 -> %s [ tailport=sw %s ] \n", label, left_label,
               (node->m_Left->m_DebugIndex < node->m_DebugIndex) ? "headport=w" : "");
        
        if (node->m_Right)
        {
            printf("    %s:f2 -> %s [ tailport=se %s ] \n", label, right_label,
            (node->m_Right->m_DebugIndex < node->m_DebugIndex) ? "headport=e" : "");
        }
    };
    
    puts("digraph ptrie {");
    puts("    node [ shape=\"record\" ]");
    puts("    graph [ overlap = false ]");

    std::set<const PTrie::Node*> seen;
    std::stack<const PTrie::Node*> nodeStack;
    nodeStack.push(ptrie->m_Root);

    seen.insert(nullptr);

    while (!nodeStack.empty())
    {
        const PTrie::Node* node = nodeStack.top();
        nodeStack.pop();
        add_attribute(node);

        seen.insert(node);

        if (seen.find(node->m_Left) == seen.end())
            nodeStack.push(node->m_Left);
        if (seen.find(node->m_Right) == seen.end())
            nodeStack.push(node->m_Right);
    }

    puts("}");
}

// ------------------------------------------------------------------------------------------------
void patricia_trie_test()
{
    char buf0[32];
    char buf1[32];

    {
        const char* a0 = "a";
        const char* b0 = "b";
        const unsigned result = first_differing_bit(a0, b0);
        printf("a[0]: %-16s b[1]: %-16s res: %d\n",  bitstring(buf0, a0, 2), bitstring(buf1, b0, 2), result);

        for (int i=0; i<8; ++i)
        {
            printf("\"a\"[%d]: %d \"b\"[%d]: %d\n",
                   i, get_bit(a0, i),
                   i, get_bit(b0, i));
        }
    }
    
    puts("");

    for (unsigned bit = 0x800000, i=1; i<=16; ++i, bit>>=1)
    {
        char testa[4];
        testa[0] = (bit>>16) & 0xff;
        testa[1] = (bit>>8) & 0xff;
        testa[2] = bit & 0xff;
        testa[3] = 0;
        char testb[4] = { 0, 0, 0, 0 };
        const unsigned result = first_differing_bit(testa, testb);
        printf("testa[0]: %-24s testb[1]: %-24s res: %d\n",  bitstring(buf0, &testa[0], 3), bitstring(buf1, &testb[0], 3), result);
    }

    puts("");
    
    for (unsigned bit = 0x800000, i=1; i<=16; ++i, bit>>=1)
    {
        char testa[4];
        testa[0] = (bit>>16) & 0xff;
        testa[1] = (bit>>8) & 0xff;
        testa[2] = bit & 0xff;
        testa[3] = 0;
        char testb[4];
        testb[0] = testa[0] | (bit>>17);
        testb[1] = testa[1] | (bit>>9);
        testb[2] = testa[2] | ((bit>>1) & 0xff);
        testb[3] = 0;
        const unsigned result = first_differing_bit(testa, testb);
        printf("testa[0]: %-24s testb[1]: %-24s res: %d\n",  bitstring(buf0, &testa[0], 3), bitstring(buf1, &testb[0], 3), result);
    }
    
    assert(get_bit("a", first_differing_bit("a", "b")) != get_bit("b", first_differing_bit("a", "b")));

    {
        PTrie ptrie;
        patricia_trie_init(&ptrie);
        patricia_trie_debug_insert_strings(&ptrie, "test_strings_r.txt");
        
        auto get = [](PTrie* ptrie, const char* label, const char* search_term)
        {
            const PTrie::Node* node = patricia_trie_search(ptrie, search_term);
            printf("%s: %14p %2u %s\n", label, node, node ? node->m_TestBit : 0, node ? node->m_Value : "");
            return node;
        };
        
        const PTrie::Node* r0 = get(&ptrie, "r0", "rome");
        const PTrie::Node* r1 = get(&ptrie, "r1", "roman");
        const PTrie::Node* r2 = get(&ptrie, "r2", "romulus");
        const PTrie::Node* r3 = get(&ptrie, "r3", "romaine");
        const PTrie::Node* r4 = get(&ptrie, "r4", "rommel");
        const PTrie::Node* r5 = get(&ptrie, "r5", "rom");
        const PTrie::Node* r6 = get(&ptrie, "r6", "frank");
        const PTrie::Node* r7 = get(&ptrie, "r7", "rank");
        const PTrie::Node* r8 = get(&ptrie, "r8", "roma");

        puts("");
        
        auto visitor = [](const PTrie::Node* node)
        {
            printf("NODE: %p %12s [l %p r %p]\n", node, node->m_Value, node->m_Left, node->m_Right);
        };
        
        puts("strings with prefix roma");
        patricia_trie_visit(r8, "roma", visitor);

        const bool roman_found = patricia_trie_exact_search(r8, "roman");
        const bool rome_found = patricia_trie_exact_search(r8, "rome");
        printf("exact search for 'roman' rooted at 'roma': %s\n", roman_found ? "FOUND" : "NOT FOUND");
        printf("exact search for 'rome' rooted at 'roma': %s\n", rome_found ? "FOUND" : "NOT FOUND");
        
        patricia_trie_fini(&ptrie);
    }
}

// ------------------------------------------------------------------------------------------------
void patricia_trie_debug_insert_strings(PTrie* ptrie, const char* fname)
{
    FILE* fh = fopen(fname, "rt");
    if (fh)
    {
        char* line = nullptr;
        ssize_t line_length;
        size_t line_cap;
        while ((line_length = getline(&line, &line_cap, fh)) > 0)
        {
            line[line_length-1] = '\0';
            if (line[line_length-2] == '\r')
                line[line_length-2] = '\0';
            patricia_trie_insert(ptrie, line);
            free(line);
            line = nullptr;
        }
        fclose(fh);
    }
}
