#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dictionary.h"
#include "autocompletion.h"


//Fonction pour générer des suggestions de mots à partir d'un préfixe
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


//Fonction pour activer la saisie prédictive
void activate_autocomplete() {
    autocomplete_activated = 1;
}

//Fonction pour désactiver la saisie prédictive
void deactivate_autocomplete() {
    autocomplete_activated = 0;
}

//Fonction pour mettre à jour le dictionnaire de prédiction
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


//Fonction principale de l'autocomplétion de mots 
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

//fonction pour afficher le menu
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


