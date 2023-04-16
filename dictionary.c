#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dictionary.h"

#define MAX_WORD_SIZE 50
#define MAX_SUGGESTIONS 10
#define DICTIONARY_SIZE 300000


// Cette fonction calcule l'index de hachage d'un mot
int hash_word(char *word) {
    int hash = 0;
    for (int i = 0; i < strlen(word); i++) {
        hash = 31 * hash + word[i];
    }
    return hash % DICTIONARY_SIZE;
}

// Cette fonction trouve un mot dans le dictionnaire et renvoie un pointeur vers sa structure Word
Word *find_word(Dictionary *dict, char *word) {
    if (dict == NULL) {
        return NULL;
    }
    
    int index = hash_word(word);
    if (index < 0 || index >= DICTIONARY_SIZE) {
        return NULL;
    }
    
    Word *current = dict->words[index];
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


// Cette fonction ajoute un mot au dictionnaire
void add_word_to_dictionary(Dictionary *dict, char *word) {
    int index = hash_word(word);
    Word *w = find_word(dict, word);
    if (w != NULL) {
        // Le mot est déjà dans le dictionnaire, on incrémente sa fréquence et sa dernière utilisation
        w->frequency++;
        time(&(w->last_used));
    } else {
        // Le mot n'est pas dans le dictionnaire, on crée une nouvelle structure Word pour lui
        w = (Word *) malloc(sizeof(Word));
        strcpy(w->word, word);
        w->frequency = 1;
        time(&(w->last_used));
        w->next = dict->words[index];
        dict->words[index] = w;
        dict->count++;
    }
}

// Cette fonction supprime un mot du dictionnaire
void remove_word_from_dictionary(Dictionary *dict, char *word) {
    int index = hash_word(word);
    Word *prev = NULL;
    Word *curr = dict->words[index];
    while (curr != NULL) {
        if (strcmp(curr->word, word) == 0) {
            if (prev == NULL) {
                dict->words[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            dict->count--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void modify_word_in_dictionary(Dictionary *dict, char *old_word, char *new_word) {
    int index = hash_word(old_word);
    Word *w = find_word(dict, old_word);
    if (w != NULL) {
        remove_word_from_dictionary(dict, old_word);
        add_word_to_dictionary(dict, new_word);
    }
}


Dictionary *load_dictionary(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }
    Dictionary *dict = (Dictionary *) malloc(sizeof(Dictionary));
    dict->count = 0;
    prediction_dict = (Dictionary *) malloc(sizeof(Dictionary));
    prediction_dict->count = 0;

    char line[MAX_WORD_SIZE];
    while (fgets(line, MAX_WORD_SIZE, fp)) {
        line[strcspn(line, "\r\n")] = 0;
        Word *word_in_dict = find_word(dict, line);
        if (word_in_dict != NULL) {
            word_in_dict->frequency++;
        } else {
            Word *new_word = (Word *) malloc(sizeof(Word));
            strcpy(new_word->word, line);
            new_word->frequency = 1;
            time(&(new_word->last_used));
            add_word_to_dictionary(dict, new_word->word);
            add_word_to_prediction(prediction_dict, new_word->word);
        }
    }

    fclose(fp);
    return dict;
}

// Libère la mémoire allouée pour le dictionnaire
void unload_dictionary(Dictionary *dict) {
    for (int i = 0; i < dict->count; i++) {
        free(dict->words[i]);
    }
    free(dict);
}


/* Adds a word to the prediction dictionary */
void add_word_to_prediction(Dictionary *dict, char *word) {
    Word *existing_word = find_word(dict, word);
    if (existing_word != NULL) {
        /* Word already exists, increment frequency and update last used time */
        existing_word->frequency++;
        existing_word->last_used = time(NULL);
    } else {
        /* Create new word and add it to the dictionary */
        Word *new_word = malloc(sizeof(Word));
        strncpy(new_word->word, word, MAX_WORD_SIZE);
        new_word->frequency = 1;
        new_word->last_used = time(NULL);
        dict->words[dict->count] = new_word;
        dict->count++;
    }
}
