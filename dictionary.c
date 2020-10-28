// Implements a dictionary's functionality

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
}
node;

// Number of buckets in hash table
const unsigned int N = 93047;

// Hash table
node *table[N];

// Number of words in dictionary
int dict_size = 0;

// Checks if word is in dictionary
bool check(const char *word)
{
    // Compute word hash value
    int hash_value = hash(word);

    // Go to hash_value index in table
    // Iterate over linked list nodes to find target word
    node *iterator;
    iterator = table[hash_value];
    while (iterator != NULL)
    {
        if (strcasecmp(word, iterator->word) == 0)
            return true;
        else
            iterator = iterator->next;
    }

    // If word not found, return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int i = 0, hash_value = 0;

    // Convert word to all lowercase letters
    char word_to_hash[LENGTH + 1];
    while (word[i] != '\0')
    {
        if (word[i] >= 'A' && word[i] <= 'Z' && word[i] != '\'')
        {
            word_to_hash[i] = word[i] + 32;
            i++;
            continue;
        }
        word_to_hash[i] = word[i];
        i++;
    }
    word_to_hash[i] = '\0';

    // Compute hash using lowercase conversion
    i = 0;
    while (word_to_hash[i] != '\0')
    {
        hash_value += word_to_hash[i];
        i++;
    }
    hash_value *= 9857;

    return hash_value %= N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dictionary_file = fopen(dictionary, "r");
    if (dictionary_file == NULL)
    {
        printf("Error opening dictionary file.\n");
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // For each word in dictionary, place word in new node
    // Then, compute hash of current word
    // Then, place node in linked list at hash_value index of hash table
    node *new_node = NULL;
    char curr_word[LENGTH + 1];
    int hash_value;
    while (fscanf(dictionary_file, "%s", curr_word) != EOF)
    {
        new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Not enough memory.");
            return false;
        }

        // Place node into linked list
        strcpy(new_node->word, curr_word);
        hash_value = hash(new_node->word);
        new_node->next = table[hash_value];
        table[hash_value] = new_node;

        // Count dictionary size as words are loaded to reduce computation time for size function
        dict_size++;
    }

    // Close dictionary file
    fclose(dictionary_file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *curr_node, *prev_node;

    // Iterate over indices in hash table
    for (int i = 0; i < N; i++)
    {
        curr_node = table[i];
        prev_node = curr_node;

        // For each index, free all nodes in linked list
        while (curr_node != NULL)
        {
            curr_node = curr_node->next;
            free(prev_node);
            prev_node = curr_node;
            table[i] = curr_node;
        }
    }

    return true;
}
