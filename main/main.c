/*
Si scriva un programma C, adeguatamente commentato, per la generazione di partite del "Gioco dell'oca" (Snakes & Ladders nella versione inglese).
Il gioco consistente nel far competere pi� giocatori al raggiungimento della casella finale, avanzando di volta in volta tra caselle diverse tramite il tiro di un dado a 6 facce.
---MORE INFO ONLINE---
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entity.h"

void drawMenu();							//disegna il menu principale
void registerPlayer(struct Player*);		//aggiungi i giocatori al gioco

int main() {
	struct Player* head = NULL;


	while (true) {

		char id[20];
		printf("Add value: ");
		scanf(" %[^\n]s", id);
		fflush(stdin);

		if(strcmp(id, "-1") != 0){

			if (head == NULL) {			//se il primo elemento della lista � nullo allora inserisce il primo valore
				head = (struct Player*)malloc(sizeof(struct Player));			//alloca lo spazio
				strcpy(head->color, id);
				head->next = NULL;			//attribuisce al puntatore all elemento successivo il valore NULL perche ancora non esiste
			}
			else {		//se invece il primo elemento gi� esiste allora esegue un push
				pushElement(head, id);
			}
		}
		else {
			popFirstElement(&head);
		}
		printAllElement(head);
		system("pause");
	}

	return 0;
}

void drawMenu() {

}
