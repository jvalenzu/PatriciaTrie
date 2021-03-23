#pragma once

// testing
void               patricia_trie_test();
void               patricia_trie_debug_insert_strings(PTrie* ptrie, const char* fname);
void               patricia_trie_dump(const PTrie* ptrie);
void               patricia_trie_dump(const PTrie::Node* node);
