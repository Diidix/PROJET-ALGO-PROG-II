# PROJET-ALGO-PROG-II
SAISIE INTUITIVE

SAISIE INTUITIVE AVEC DICTIONNAIRE DE PREDICTION 

Ce projet permet de créer un dictionnaire de mots en lisant un fichier texte et de fournir une fonctionnalité d'auto-complétion pour aider l'utilisateur à compléter un mot saisi.

FICHIERS FOURNIS

dictionary.c : le code source de la bibliothèque DICTIONARY_H
dictionary.h : le fichier d'en tête en rapport avec les dictionnaires
autocompletion.c : le code source de la bibliothèque AUTOCOMPLETION_H
autocompletion.h : le fichier d'en-tête en rapport avec les fonctions d'autocomplétions
mots_courants.txt : un fichier texte contenant une liste de mots courants pour tester le programme
README.txt : ce fichier

VARIABLES GLOBALES
Les variables globales du programme sont définies au début du fichier autocomplete.c :

MAX_WORD_SIZE : la taille maximale d'un mot (50 caractères)
MAX_SUGGESTIONS : le nombre maximal de suggestions à afficher pour l'auto-complétion (10 suggestions)
DICTIONARY_SIZE : la taille du tableau de hachage utilisé pour stocker les mots dans le dictionnaire (300000 entrées)
file : le nom du fichier contenant les mots à ajouter dans le dictionnaire ("mots_courants.txt")

STRUCTURES UTILISEES

Les structures utilisées sont définies dans le fichier d'en-tête autocomplete.h :

Word : une structure qui représente un mot du dictionnaire, contenant le mot lui-même (word), sa fréquence (frequency), un pointeur vers le mot suivant dans la liste chainée (next), et la date de la dernière utilisation (last_used)

Dictionary : une structure qui contient le tableau de pointeurs vers les mots du dictionnaire (words) et le nombre total de mots dans le dictionnaire (count)

FONCTIONS UTILISEES

Les fonctions utilisées dans le programme sont définies dans le fichier d'en-tête autocomplete.h :

Fonctions pour le dictionnaire

Dictionary *load_dictionary(char *filename) : cette fonction charge le dictionnaire à partir d'un fichier texte. Elle prend en argument le nom du fichier et renvoie un pointeur vers une structure Dictionary contenant les mots du dictionnaire.

void unload_dictionary(Dictionary *dict) : cette fonction décharge le dictionnaire de la mémoire.

int hash_word(char *word) : cette fonction calcule l'index de hachage d'un mot à partir de sa chaîne de caractères.

Word *find_word(Dictionary *dict, char *word) : cette fonction cherche un mot dans le dictionnaire à partir de sa chaîne de caractères et renvoie un pointeur vers la structure Word correspondante.

void add_word_to_dictionary(Dictionary *dict, char *word) : cette fonction ajoute un mot au dictionnaire, en créant une nouvelle structure Word s'il n'existe pas déjà dans le dictionnaire, et en incrémentant sa fréquence et sa date de dernière utilisation sinon.

void remove_word_from_dictionary(Dictionary *dict, char *word) : cette fonction supprime un mot du dictionnaire, en libérant la mémoire allouée pour sa structure Word.

void modify_word_in_dictionary(Dictionary *dict, char *old_word, char *new_word) : cette fonction modifie un mot existant dans le dictionnaire, en supprimant l'ancien mot et en ajoutant le nouveau.

void add_word_to_prediction(Dictionary *dict, char *word) : cette fonction ajoute le mot en paramètre dans le dictionnaire de prédiction en paramètre s'il n'y existe pas déjà. S'il n'y existe pas, on augmente seulement le nombre d'occurence du mot.

Fonctions pour l'auto-complétion

Word **get_suggestions(Dictionary *dict, char *prefix) : cette fonction renvoie un tableau de pointeurs vers les MAX_SUGGESTIONS mots du dictionnaire qui commencent par le préfixe prefix. Les mots sont triés par ordre décroissant de fréquence et de date de dernière utilisation.

void activate_autocomplete() : cette fonction active l'auto-complétion en mettant la variable autocomplete_activated à 1.

void deactivate_autocomplete() : Cette fonction désactive la fonctionnalité d'auto-complétion en mettant la variable autocomplete_activated à 0.

void update_dictionary(char *word) : Cette fonction met à jour le dictionnaire en ajoutant le mot passé en paramètre ou en augmentant son nombre d'occurence s'il est déjà dans le dictionnaire.

void autocomplete_word(char *input) : Cette fonction prend un mot en entrée et renvoie une liste de suggestions de mots à partir de ce mot en utilisant la fonction d'auto-complétion.


INSTRUCTIONS GENERALES D'UTILISATION

1.Verifier que tous les fichiers (header, code source, fichiers fournis) sont dans le même dossier de travail
2.Compiler le code source en exécutant la commande : gcc main.c dictionary.c autocompletion.c -o appli
3.Exécuter le programme en saisissant la commande : ./appli

						OU

 En lieu et place des étapes 1 et 2 : TOUT SIMPLEMENT utilisez le makefile déjà fourni en saisissant make -f makefile dans votre compilateur

4.Le programme affichera un message de chargement du dictionnaire. Patientez jusqu'à ce que le dictionnaire soit chargé avant de saisir un mot.
7.Sélectionnez l'une des suggestions du menu, puis appuyez sur Entrée pour valider la sélection.
8.Suivez les instructions

Note : Le fichier de mots mots_courants.txt peut être remplacé par un fichier contenant des mots différents. Pour cela, modifiez la variable globale file dans le fichier dictionary.c et recompilez le code source en suivant les instructions ci-dessus.

MERCI POUR VOTRE ATTENTION !!!
