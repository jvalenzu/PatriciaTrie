#include "PatriciaTrie.h"
#include "PatriciaTrieDebug.h"
#include "SuffixTree.h"
#include "SuffixTreeDebug.h"
#include "Util.h"

#include "linenoise/linenoise.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

int main(int argc, char* argv[])
{
    PTrie ptrie;

    if (argc > 1 && !strcmp(argv[1], "--dot"))
    {
        const char* fname = "test_strings.txt";
        if (argc > 2)
            fname = argv[2];
        
        patricia_trie_init(&ptrie);
        patricia_trie_debug_insert_strings(&ptrie, fname);
        
        patricia_trie_dump(&ptrie);
        return 0;
    }

    if (argc > 1 && !strcmp(argv[1], "--profile"))
    {
        const char* fname = "words";

        free(linenoise("$ "));
        
        patricia_trie_init(&ptrie);
        patricia_trie_debug_insert_strings(&ptrie, fname);
        
        for (int j=0; j<20000000; ++j)
        {
            for (int i=0; i<26; ++i)
            {
                char text[2] = { (char)('A' + i), 0 };
                const PTrie::Node* node = patricia_trie_search(&ptrie, text);
            }
        }

        return 0;
    }

    if (argc > 1 && !strcmp(argv[1], "--interactive"))
    {
        bool done = false;
        char* line = nullptr;
        while (!done && ((line = linenoise("$ ")) != nullptr)) {
            if (*line)
            {
                patricia_trie_init(&ptrie);
                
                if (!strcmp(line, "help") || !strcmp(line, "?"))
                {
                    puts("help");
                    puts("prefix <str>");
                    puts("exact <str>");
                    puts("quit");
                }
                else
                {
                    linenoiseHistoryAdd(line);
                    
                    std::vector<token> tokens;
                    split_line(&tokens, line);
                    
                    const uint32_t insert_hash = hash("insert");
                    const uint32_t dump_hash = hash("dump");
                    const uint32_t load_hash = hash("load");
                    const uint32_t sload_hash = hash("sload");
                    if (tokens[0].m_Hash == dump_hash)
                    {
                        patricia_trie_dump(&ptrie);
                    }
                    else if (tokens[0].m_Hash == insert_hash)
                    {
                        patricia_trie_insert(&ptrie, tokens[1].m_Value);
                    }
                    else if (tokens[0].m_Hash == insert_hash)
                    {
                        patricia_trie_insert(&ptrie, tokens[1].m_Value);
                    }
                    else if (tokens[0].m_Hash == load_hash)
                    {
                        patricia_trie_debug_insert_strings(&ptrie, tokens[1].m_Value);
                    }
                    else if (tokens[0].m_Hash == sload_hash)
                    {
                        suffix_debug_insert_strings(&ptrie, tokens[1].m_Value);
                    }
                    else if (!strncmp(line, "prefix ", 7))
                    {
                        char* text = line + 7;
                        if (*text)
                        {
                            const PTrie::Node* node = patricia_trie_search(&ptrie, text);
                            if (node)
                            {
                                patricia_trie_visit(node, text, [](const PTrie::Node* node)
                                {
                                    printf("%12s\n", node->m_Value);
                                });
                            }
                            else
                            {
                                printf("No matches: %12s\n", text);
                            }
                        }
                    }
                    else if (!strncmp(line, "exact ", 6))
                    {
                        char* text = line + 6;
                        if (*text)
                        {
                            if (patricia_trie_exact_search(&ptrie, text))
                                printf("%12s\n", text);
                            else
                                printf("%12s not found\n", text);
                        }
                    }
                    else if (!strncmp(line, "quit", 4))
                    {
                        done = true;
                    }
                    else if (!strncmp(line, "q", 4))
                    {
                        done = true;
                    }
                    else
                    {
                        printf("Unknown '%s'\n", line);
                    }
                }
            }
            
            free(line);
        }
        
        return 0;
    }

    patricia_trie_test();

    return 0;
}
