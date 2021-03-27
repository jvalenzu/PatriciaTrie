#include "SuffixTree.h"
#include "SuffixTreeDebug.h"

#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------------------------------------
void suffix_debug_insert_strings(PTrie* ptrie, const char* fname)
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
            
            suffix_tree_expand_word(ptrie, line);
            
            free(line);
            line = nullptr;
        }
        fclose(fh);
    }
}
