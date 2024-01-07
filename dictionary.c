// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 60000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int h = hash(word);

    node *ptr = table[h];

    while (ptr != NULL)
    {
        if (strcasecmp(ptr->word, word) == 0)
            return true;
        else
            ptr = ptr->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    int h = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        h = (h * 31 + toupper(word[i]));
    }

    return h % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    // Open dictionary file on read mode & make sure it opens
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fclose(inptr);
        return 0;
    }

    // Initialize buffer, character, index, and hashing variable
    char buffer[LENGTH - 1];
    char c;
    int index = 0, h = 0;

    // Loop iterating over all dictionary characters
    while (fread(&c, sizeof(char), 1, inptr))
    {
        if (c != '\n')
        {
            buffer[index] = c;
            index++;
        }
        else
        {
            buffer[index] = '\0';

            h = hash(buffer);

            // If hash place is empty
            if (table[h] == NULL)
            {
                table[h] = malloc(sizeof(node));
                if (table[h] == NULL)
                {
                    free(table[h]);
                    return 0;
                }
                table[h]->next = NULL;

                for (int i = 0; i <= index; i++)
                    table[h]->word[i] = buffer[i];

                table[h]->word[index] = '\0';

                index = 0;
            }

            // If collision, attach in linked list
            else
            {
                // Create new node, assign its next pointer NULL, & copy buffer to its word
                node *new = malloc(sizeof(node));
                if (new == NULL)
                {
                    free(new);
                    return 0;
                }

                new->next = table[h];
                for (int i = 0; i <= index; i++)
                    new->word[i] = buffer[i];

                new->word[index] = '\0';

                table[h] = new;

                index = 0;
            }
        }
    }
    fclose(inptr);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    unsigned int size = 0;

    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
            continue;

        else
        {
            size++;

            node *ptr = table[i];
            while (ptr->next != NULL)
            {
                size++;
                ptr = ptr->next;
            }
        }
    }

    return size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        node *next = ptr;

        if (ptr == NULL)
            continue;

        else
        {
            while (ptr != NULL)
            {
                next = ptr->next;
                free(ptr);
                ptr = next;
            }
        }
    }

    return true;
}