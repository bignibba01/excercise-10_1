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
#include <time.h>
#include "entity.h"

void drawMenu();							//disegna il menu principale
int getNumberPlayer();	//probabile problema					//prende il numero dei giocatori
void registerPlayer();						//inserisce il giocatore nella coda
int rollDice();								//tira il dado (1-6)

struct Player* head = NULL;

int main() {

	while (true) {
		_Bool end = false;		//esce dal ciclo continuo
		_Bool flag = true;		//esce dal ciclo dei controlli
		int oper = 0;			//operazione scelta dall utente

		drawMenu();
		do {
			printf("Scegli operazione: ");
			scanf(" %d", &oper);
			fflush(stdin);

			if (oper >= 1 && oper <= 4)
				flag = false;
			else
				printf("Errore -> Operazione non valida\n");
		} while (flag);

		switch (oper) {
		case 1: {
			break;
			}
		case 2: {
			registerPlayer();
			system("pause");
			break;
		}
		case 3: {
			printAllElement(head);
			system("pause");
			break;
		}
		case 4: {
			end = true;
			break;
		}
		default: {
			printf("Errore -> L'operazione non esiste\n");
		}
		}

		if (end)
			break;		//esce dal ciclo continuo

		system("cls");
		
	}

	return 0;
}

void drawMenu() {
	printf("1)Inizia partita!\n2)Aggiungi giocatori (%d correnti)!\n3)Lista dei giocatori\n4)Esci\n", getNumberPlayer());
}

int getNumberPlayer() {
	int tmpNumber = 0;

	while (head != NULL) {
		tmpNumber = head->id;
		head = head->next;
	}

	return tmpNumber;
}

void registerPlayer() {

	int tmpId = 0;
	char tmpColor[20] = { 0 };

	if (head == NULL) {			//se il primo elemento della lista è nullo allora inserisce il primo valore
		head = (struct Player*)malloc(sizeof(struct Player));			//alloca lo spazio

		head->id = 1;
		head->next = NULL;			//attribuisce al puntatore all elemento successivo il valore NULL perche ancora non esiste
	}
	else {		//se invece il primo elemento già esiste allora esegue un push
		struct Player* tmp = head;
		while (tmp != NULL) {
			tmpId = tmp->id + 1;			//preleva l'id dall ultimo elemento della lista e lo incrementa
			tmp = tmp->next;
		}
		if (tmpId == 6)				//6 perche tmpId = tmp->id + 1 (5+1)
			printf("Numero massimo di giocatori raggiunto!\n");
		else
			pushElement(head, tmpId);
	}
}

int rollDice() {
	srand(time(NULL));

	return rand() % 6 + 1;
}