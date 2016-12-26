/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 * 
 * Bogdan Ioan Nechifor
 *
 * Implements a dictionary's functionality.
 * 
 * Used some tips for better performance in speed from this article written
 * by Koushik Ghosh, an indian software developer for IBM.The website link 
 * http://tinyurl.com/zjznk23
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// global variable for size() function
unsigned int num_words;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // initialize a cursor for iterating the trie
    tNode* cursor = root;
    
    // check word's letter against trie letters
    for (unsigned int i = 0; word[i] != '\0'; i++)
    {
        // look for apostrophe and assign the last child positon
        unsigned int idx;
        if (word[i] == 39)
        {
            idx = 26;
        }
        // store the alphabet index resulted from current letter
        else
        {
            // tolower() function same performance as the bitwise | operators
            // extract index by substracting value 97 ASCII code for 'a' 
            idx = tolower(word[i]) - 97;
        }
      
        // check if letter exist
        if (cursor->alphabet[idx] == NULL)
        {
            // word doesn't exist
            return false;
        }
        else
        {
            // move to the next letter
            cursor = cursor->alphabet[idx];
        }
    }
    
    // return the value found at the end of the list
    return cursor->is_word;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // set counter for words to 0
    // set local variable copy for optimization
    int local_words = 0;
    
    // container for word, adding the null character
    char word[LENGTH + 1];
    
    // allocate memory and initialize all bits to 0
    // use calloc() function to avoid valgrind error:
    // "Conditional jump or move depends on uninitialised value(s)"
    root = calloc(1, sizeof(tNode));
    if (root == NULL)
    {
        return false;
    }
    
    // initialize cursor pointer to iterate through nodes
    tNode* cursor;
    
    // open dictionary
    FILE* fptr = fopen(dictionary, "r");
    if (fptr == NULL)
    {
        return false;
    }
    
    // iterate assuming that there is only one word per line
    // and no word is bigger then 45 characters
    // fscanf() will return 0 if failure or EOF value
    while (fscanf(fptr, "%45s", word) == 1)
    {
        // reset cursor for each word read
        cursor = root;
        
        // load word in memory
        for (unsigned int i = 0; word[i] != '\0'; i++)
        {
            // look for apostrophe and asign the last child positon
            unsigned int idx;
            if (word[i] == 39)
            {
                idx = 26;
            }
            // store the alphabet index resulted from current letter
            else
            {
                // extract index
                idx = word[i] - 97; 
            }
            
            // create new node if doesn't exist
            if (cursor->alphabet[idx] == NULL)
            {
                // allocate memory and initialize all bits to 0
                tNode* newNode = calloc(1, sizeof(tNode));
                if (newNode == NULL)
                {
                    return false;
                }
                
                // point cursor to that node
                cursor->alphabet[idx] = newNode;
                
                // move cursor, prepare for the next letter
                cursor = cursor->alphabet[idx];
            }
            // if exist go to the next node
            else
            {
                cursor = cursor->alphabet[idx];
            }
        }
        
        // add flag for finished word
        cursor->is_word = true;
        
        // increase word in dictionary count
        local_words++;
    }
    
    // check for errors during reading
    if (ferror(fptr))
    {
        return false;
    }
    
    // store the number of words into the global variable
    num_words = local_words;
    
    // close dictionary
    fclose(fptr);
    
    // all is loaded
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return num_words;
}

/**
 * Function for freeing memory allocated to a trie node
 */
void freeNode(tNode* node)
{
    // base case
    if (node == NULL)
        return;
        
    // travel to the lowest  posible node
    for (unsigned int i = 0; i < 27; i++)
    {
            // call itself for next node
            freeNode(node->alphabet[i]);
    }
    
    // free node
    free(node);
}


/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // Unload from the bottom up
    freeNode(root);
    return true;
}
