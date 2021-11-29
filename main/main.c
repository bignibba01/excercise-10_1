/*
Si scriva un programma C, adeguatamente commentato, per la generazione di partite del "Gioco dell'oca" (Snakes & Ladders nella versione inglese).
Il gioco consistente nel far competere più giocatori al raggiungimento della casella finale, avanzando di volta in volta tra caselle diverse tramite il tiro di un dado a 6 facce.
---MORE INFO ONLINE---
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "entity.h"

void drawMenu();					//disegna il menu principale
void pushElement(struct Player*);			//aggiunge un valore alla lista dei giocatori

int main() {
	struct Player* head = NULL;

	//head = (struct Player*)malloc(sizeof(struct Player));

	while (true) {
		int id;
		printf("Add value: ");
		scanf(" %d", &id);

		if (head == NULL) {			//se il primo elemento della lista è nullo allora inserisce il primo valore
			head = (struct Player*)malloc(sizeof(struct Player));			//alloca lo spazio
			head->id = id;
			head->next = NULL;			//attribuisce al puntatore all elemento successivo il valore NULL perche ancora non esiste
		}
		else {		//se invece il primo elemento già esiste allora esegue un push
			pushElement(head, id);
		}

		/*struct Player* current = head;		//just output

		while (current != NULL) {
			printf("%d\n", current->id);
			current = current->next;
		}*/

		system("pause");
	}

	return 0;
}

void drawMenu() {

}

void pushElement(struct Player* head, int id) {

	while (head->next != NULL) {
		head = head->next;
	}

	head->next = (struct Player*)malloc(sizeof(struct Player));
	head->next->id = id;
	head->next->next = NULL;
}