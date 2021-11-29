/*
Si scriva un programma C, adeguatamente commentato, per la generazione di partite del "Gioco dell'oca" (Snakes & Ladders nella versione inglese).
Il gioco consistente nel far competere più giocatori al raggiungimento della casella finale, avanzando di volta in volta tra caselle diverse tramite il tiro di un dado a 6 facce.
---MORE INFO ONLINE---
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entity.h"

void drawMenu();							//disegna il menu principale
void popFirstElement(struct Player**);		//toglie un valore dalla testa della coda (FIFO)
void printAllElement(struct Player*);		//visualizza tutta la coda
void pushElement(struct Player*, char*);			//aggiunge un valore alla coda dei giocatori
void registerPlayer(struct Player*);		//aggiungi i giocatori al gioco

int main() {
	struct Player* head = NULL;

	//head = (struct Player*)malloc(sizeof(struct Player));

	while (true) {

		char id[20];
		printf("Add value: ");
		scanf(" %[^\n]s", id);
		fflush(stdin);


		if(strcmp(id, "-1") != 0){

			if (head == NULL) {			//se il primo elemento della lista è nullo allora inserisce il primo valore
				head = (struct Player*)malloc(sizeof(struct Player));			//alloca lo spazio
				strcpy(head->color, id);
				head->next = NULL;			//attribuisce al puntatore all elemento successivo il valore NULL perche ancora non esiste
			}
			else {		//se invece il primo elemento già esiste allora esegue un push
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

void popFirstElement(struct Player** head) {
	//int retval = -1;
	struct Player* next_node = NULL;

	if (*head == NULL) {
		return -1;
	}

	next_node = (*head)->next;
	//retval = (*head)->id;
	free(*head);
	*head = next_node;
}

void printAllElement(struct Player* head) {
	while (head != NULL) {
		printf("%s\n", head->color);
		head = head->next;
	}
}

void pushElement(struct Player* head, char id[20]) {

	while (head->next != NULL) {
		head = head->next;
	}

	head->next = (struct Player*)malloc(sizeof(struct Player));
	strcpy(head->next->color, id);
	head->next->next = NULL;
}
