#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_SIZE 50
#define MAX_SUGGESTIONS 10
#define DICTIONARY_SIZE 300000

#define file "mots_courants.txt"

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

// Fonctions pour le dictionnaire
Dictionary *load_dictionary(char *filename);
void unload_dictionary(Dictionary *dict);
int hash_word(char *word);
Word *find_word(Dictionary *dict, char *word);
void add_word_to_dictionary(Dictionary *dict, char *word);
void remove_word_from_dictionary(Dictionary *dict, char *word);
void modify_word_in_dictionary(Dictionary *dict, char *old_word, char *new_word);

// Fonctions pour l'auto-complétion
Word **get_suggestions(Dictionary *dict, char *prefix);
void activate_autocomplete();
void deactivate_autocomplete();
void update_dictionary(char *word);
void add_word_to_prediction(Dictionary *dict, char *word);
void autocomplete_word(char *input);

// Fonction pour l'affichage
void display_menu();

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

static Dictionary *prediction_dict = NULL;

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

Word **get_suggestions(Dictionary *dict, char *prefix) {
    Word **suggestions = (Word **) malloc(sizeof(Word *) * MAX_SUGGESTIONS);
    int count = 0;
    // Recherche des mots commençant par le préfixe donné
    for (int i = 0; i < dict->count; i++) {
        if (strncmp(dict->words[i]->word, prefix, strlen(prefix)) == 0) {
            suggestions[count] = dict->words[i];
            count++;

            // On limite le nombre de suggestions à MAX_SUGGESTIONS
            if (count == MAX_SUGGESTIONS) {
                break;
            }
        }
    }

    // Si le nombre de suggestions est inférieur à MAX_SUGGESTIONS, on remplit le reste avec des mots aléatoires du dictionnaire
    if (count < MAX_SUGGESTIONS) {
        srand(time(NULL));
        int index;

        while (count < MAX_SUGGESTIONS) {
            index = rand() % dict->count;
            suggestions[count] = dict->words[index];
            count++;

            // Si on a déjà ajouté ce mot, on le retire de la liste pour éviter les doublons
            if (suggestions[count - 1] != NULL) {
                for (int j = count; j < MAX_SUGGESTIONS; j++) {
                    if (suggestions[j] == suggestions[count - 1]) {
                        suggestions[j] = NULL;
                    }
                }
            }
        }
    }

    return suggestions;
}

static int autocomplete_activated = 0;

void activate_autocomplete() {
    autocomplete_activated = 1;
}

void deactivate_autocomplete() {
    autocomplete_activated = 0;
}

void update_dictionary(char *word) {
    // Vérifier si le mot est déjà dans le dictionnaire de prédiction
    Word *word_in_dict = find_word(prediction_dict, word);
    
    if (word_in_dict != NULL) {
        // Le mot est déjà dans le dictionnaire, on incrémente sa fréquence et sa dernière utilisation
        word_in_dict->frequency++;
        time(&(word_in_dict->last_used));
    } else {
        // Le mot n'est pas dans le dictionnaire, on l'ajoute avec une fréquence de 1 et la date courante comme dernière utilisation
        Word *new_word = (Word *) malloc(sizeof(Word));
        strcpy(new_word->word, word);
        new_word->frequency = 1;
        time(&(new_word->last_used));
        add_word_to_dictionary(prediction_dict, word);
    }
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

// Libère la mémoire allouée pour le dictionnaire
void unload_dictionary(Dictionary *dict) {
    for (int i = 0; i < dict->count; i++) {
        free(dict->words[i]);
    }
    free(dict);
}

void autocomplete_word(char *input) {
    if (!autocomplete_activated) {
        printf("L'autocomplétion n'est pas activée.\n");
        return;
    }
    
    Word **suggestions = get_suggestions(prediction_dict, input);
    if (suggestions[0] == NULL) {
        printf("Aucune suggestion trouvée pour '%s'.\n", input);
        return;
    }
    
    printf("Suggestions pour '%s':\n", input);
    for (int i = 0; i < MAX_SUGGESTIONS && suggestions[i] != NULL; i++) {
        printf("%d. %s\n", i+1, suggestions[i]->word);
    }
    
    int choice = -1;
    while (choice < 0 || choice > MAX_SUGGESTIONS) {
        printf("Choisissez un mot (ou tapez 0 pour continuer à écrire) : ");
        scanf("%d", &choice);
        if (choice == 0) {
            // Continuer à écrire
            return;
        }
        else if (choice < 0 || choice > MAX_SUGGESTIONS) {
            printf("Choix invalide.\n");
        }
    }
    
    // Mettre un espace et écrire le mot choisi
    printf(" ");
    printf("%s", suggestions[choice-1]->word);
    
    // Libération de la mémoire allouée pour les suggestions
    free(suggestions);
}


void display_menu() {
    printf("=== MENU ===\n");
    printf("1. Ajouter un mot au dictionnaire\n");
    printf("2. Supprimer un mot du dictionnaire\n");
    printf("3. Modifier un mot dans le dictionnaire\n");
    printf("4. Rechercher un mot dans le dictionnaire\n");
    printf("5. Obtenir des suggestions de mots à partir d'un préfixe\n");
    printf("6. Activer l'autocomplétion\n");
    printf("7. Désactiver l'autocomplétion\n");
    printf("8. Mettre à jour les prédictions pour un mot\n");
    printf("9. Ajouter un mot aux prédictions\n");
    printf("10. Autocompléter un mot\n");
    printf("11. Quitter\n");
    printf("Votre choix : ");
}

int main() {
// Création du dictionnaire de mots
Dictionary *dictionary = load_dictionary(file);

// Variables pour la saisie utilisateur
char input[MAX_WORD_SIZE];
char word[MAX_WORD_SIZE];
char new_word[MAX_WORD_SIZE];
int choice;

// Boucle du menu
do {
    display_menu();
    scanf("%d", &choice);
    fflush(stdin);
    
    switch(choice) {
        case 1:
            printf("Entrez le mot à ajouter : ");
            scanf("%s", word);
            fflush(stdin);
            add_word_to_dictionary(dictionary, word);
            printf("Le mot '%s' a été ajouté au dictionnaire.\n", word);
            printf("\n");
            break;
        case 2:
            printf("Entrez le mot à supprimer : ");
            scanf("%s", word);
            fflush(stdin);
            remove_word_from_dictionary(dictionary, word);
            printf("Le mot '%s' a été supprimé du dictionnaire.\n", word);
            printf("\n");
            break;
        case 3:
            printf("Entrez le mot à modifier : ");
            scanf("%s", word);
            fflush(stdin);
            printf("Entrez la nouvelle valeur du mot : ");
            scanf("%s", new_word);
            fflush(stdin);
            modify_word_in_dictionary(dictionary, word, new_word);
            printf("Le mot '%s' a été modifié en '%s'.\n", word, new_word);
            printf("\n");
            break;
        case 4:
            printf("Entrez le mot à rechercher : ");
            scanf("%s", word);
            fflush(stdin);
            if (find_word(dictionary, word) != NULL) {
                printf("Le mot '%s' a été trouvé dans le dictionnaire.\n", word);
            } else {
                printf("Le mot '%s' n'a pas été trouvé dans le dictionnaire.\n", word);
            }
            printf("\n");
            break;
        case 5:
            printf("Entrez le préfixe pour les suggestions : ");
            scanf("%s", input);
            fflush(stdin);
            autocomplete_word(input);
            printf("\n");
            break;
        case 6:
            autocomplete_activated = 1;
            printf("L'autocomplétion a été activée.\n");
            printf("\n");
            break;
        case 7:
            autocomplete_activated = 0;
            printf("L'autocomplétion a été désactivée.\n");
            printf("\n");
            break;
        case 8:
            printf("Entrez le mot à mettre à jour : ");
            scanf("%s", word);
            fflush(stdin);
            update_dictionary(word);
            printf("Les informations du mot '%s' ont été mises à jour.\n", word);
            printf("\n");
            break;
        case 9:
            printf("Entrez le mot à ajouter aux prédictions : ");
            scanf("%s", word);
            fflush(stdin);
            add_word_to_prediction(prediction_dict, word);
            printf("Le mot '%s' a été ajouté aux prédictions.\n", word);
            printf("\n");
            break;
        case 10:
            printf("Entrez le mot à autocompléter : ");
            scanf("%s", input);
            fflush(stdin);
            autocomplete_word(input);
            printf("\n");
            break;
        case 11:
            printf("Merci d'avoir utilisé notre programme !\n");
            printf("\n");
            break;
        default:
            printf("Choix invalide.\n");
            printf("\n");
            break;
    }
} while (choice != 11);

// Libération de la mémoire allouée
unload_dictionary(dictionary);
unload_dictionary(prediction_dict);

return 0;
}



