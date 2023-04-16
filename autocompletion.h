#ifndef AUTOCOMPLETION_H
#define AUTOCOMPLETION_H

//DEFINITION DE LA VARIABLE STATIQUE
static int autocomplete_activated = 0;


// Fonctions pour l'auto-complétion
Word **get_suggestions(Dictionary *dict, char *prefix);
void activate_autocomplete();
void deactivate_autocomplete();
void update_dictionary(char *word);
void autocomplete_word(char *input);

// Fonction pour l'affichage
void display_menu();

#endif