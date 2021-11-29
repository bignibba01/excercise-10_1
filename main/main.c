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

int main() {
	struct Player* head = NULL;

	head = (struct Player*)malloc(sizeof(struct Player));
	while (true) {
		//drawMenu();

		head->id = 2;
		head->next = (struct Player*)malloc(sizeof(struct Player));
		head->next->id = 3;
		head->next->next = (struct Player*)malloc(sizeof(struct Player));
		head->next->next->id = 4;
		head->next->next->next = NULL;

		struct Player* current = head;

		while (current != NULL) {
			printf("%d\n", current->id);
			current = current->next;
		}

		system("pause");
	}

	return 0;
}