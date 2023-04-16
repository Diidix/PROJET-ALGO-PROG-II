#ifndef DICTIONARY_H
#define DICTIONARY_H

#define MAX_WORD_SIZE 50
#define MAX_SUGGESTIONS 10
#define DICTIONARY_SIZE 300000


typedef struct Word {
    char word[MAX_WORD_SIZE];
    int frequency;
    struct Word *next;
    time_t last_used;
} Word;

typedef struct Dictionary {
    Word *words[DICTIONARY_SIZE];
    int count;
} Dictionary;

//Déclaration de la variable statique 
static Dictionary *prediction_dict = NULL;


// Fonctions pour le dictionnaire
Dictionary *load_dictionary(char *filename);
void unload_dictionary(Dictionary *dict);
int hash_word(char *word);
Word *find_word(Dictionary *dict, char *word);
void add_word_to_dictionary(Dictionary *dict, char *word);
void remove_word_from_dictionary(Dictionary *dict, char *word);
void modify_word_in_dictionary(Dictionary *dict, char *old_word, char *new_word);
void add_word_to_prediction(Dictionary *dict, char *word);

#endif