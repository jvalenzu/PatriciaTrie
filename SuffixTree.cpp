#include "SuffixTree.h"
// ------------------------------------------------------------------------------------------------
void suffix_tree_expand_word(PTrie* ptrie, const char* word)
{
    if (!word)
        return;

    for (const char* c = word; *c; ++c)
        patricia_trie_insert(ptrie, c);
}
