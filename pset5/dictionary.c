/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// global viarible for size function
unsigned int num_words;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // initialize a cursor for iterating the trie
    tNode* cursor = root;
    
    // check word's letter against trie letters
    for (int i = 0; word[i] != '\0'; i++)
    {
        // store the alphabet index resulted from current letter
        int idx = word[i] -'a';
        
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
    num_words = 0;
    
    // container for word
    char word[LENGTH+ 1];
    
    // allocate space in memory for root
    root = malloc(sizeof(tNode));
    if (root == NULL)
    {
        return false;
    }
    
    // initialize cursor pointer to iterate through nodes
    tNode* cursor;
    
    // open dictionary
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        return false;
    }
    
    // iterate assuming that there is only one word per line
    while (!feof(fp))
    {
        // read word 
        fgets(word, LENGTH, fp);
        
        // reset cursor for each word
        cursor = root;
        
        // load word in memory
        for (int i = 0; word[i] != '\0'; i++)
        {
            // store the alphabet index resulted from current letter
            int idx = word[i] - 'a';
            
            // create new node if doesn't exist
            if (cursor->alphabet[idx] == NULL)
            {
                tNode* newNode = malloc(sizeof(tNode));
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
        num_words++;
    }
    
    // check for errors during reading
    if (ferror(fp))
    {
        return false;
    }
    
    // close dictionary
    fclose(fp);
    
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
bool freeNode(tNode* node)
{
    // iterate through each letter of the word until end
    for (int i = 0; i < 27; i++)
    {
        if (node->alphabet[i] != NULL)
        {
            // call itself for next node
            freeNode(node->alphabet[i]);
        }

    }
    
    free(node);
    return true;
}


/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // Unload from the bottom up
    
    return freeNode(root);
}

