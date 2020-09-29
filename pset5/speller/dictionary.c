// Implements a dictionary's functionality
#include <stdbool.h>

#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536;

// Hash table
node *table[N];

unsigned int num = 0; // for counting words.

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char lword[strlen(word)];
    strcpy(lword, word);
    for (int l = 0; lword[l] != '\0'; l++)
    {
        lword[l] = tolower(lword[l]);
    }

    int hsh = hash(lword);
    if (table[hsh] != NULL)
    {
        //printf("hsh = %i\n", hsh);
        //printf("wordgiven %s\n", lword);
        for (node *cursor = table[hsh]; cursor != NULL; cursor = cursor->next)
        {
            if (strcasecmp(cursor->word, lword) == 0)
            {
                return true;
            }
        }
    }
    return false;
}


// credit to: https://cs50.stackexchange.com/questions/37209/pset5-speller-hash-function
// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash_value = (hash_value << 2) ^ word[i];
    }
    return hash_value % N; //N is size of hashtable
}



// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *txt_r = fopen(dictionary, "r"); // making a file of txt given

    if (txt_r == NULL) // checking the file
    {
        return false;
    }

    //printf("opened file\n");
    for (int i = 0; i <= N; i++)
    {
        table[i] = NULL;
    }
    //printf("went through table and made all null\n");
    char text[LENGTH + 1];
    node *tmpn;

    while (fscanf(txt_r, "%s", text) != EOF)
    {
        num++; // counting the words to use in size (might have to divide by 2)
        //printf("counted words added of dict\n");
        tmpn = malloc(sizeof(node)); // creating new node
        if (tmpn == NULL) // checking if new node has space
        {
            free(tmpn);
            //printf("tmpn is null so freed it\n");
        }
        //} while (tmpn == NULL);

        strcpy(tmpn->word, text); // placing word into node
        //printf("copy dict word to node, %s = %s\n", text , tmpn->word);
        int h = hash(text); //getting hash num of word
        if (table[h] == NULL)
        {
            tmpn->next = NULL;
            table[h] = tmpn;
            //printf("table[h] is null so made it equal to tmpn and tmpn null\n");
        }
        else
        {
            tmpn->next = table[h]; // pointing to what table is pointing at
            table[h] = tmpn; // make table point to new node
            //printf("table[h] is not null so made tmpn point to table point then table to tmpn\n");
        }
        //free(tmpn);
    }
    fclose(txt_r); // closing file that was created at first
    //printf("finished loaded\n");
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (num <= 0)
    {
        return 0;
    }
    return num;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    if (num <= 0)
    {
        return false;
    }
    for (int i = 0; i <= N; i++)
    {
        if (table[i] != NULL)
        {
            node *point_free = table[i];
            while (point_free != NULL)
            {
                node *tmp = point_free;
                point_free = point_free->next;
                free(tmp);
            }
        }
    }
    return true;
}
