#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "autocomplete.h"

#define file "mots_courants.txt"



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

