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

#ifdef __MINGW32__
	#define NEED_COLOR_FIX
#endif
#ifdef NEED_COLOR_FIX
	#include <windows.h>
	#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void drawMenu();							//disegna il menu principale
char* getColor(int);
char* getColorCode(int);
int getNumberPlayer();						//prende il numero dei giocatori
void printAvailableColor();					//stampa i colori disponibili
void registerPlayer();						//inserisce il giocatore nella coda
int rollDice();								//tira il dado (1-6)

struct Player* head = NULL;

int main() {

	#ifdef NEED_COLOR_FIX				//procedimento per abilitare i colori su windows
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle != INVALID_HANDLE_VALUE) {
		DWORD mode = 0;
		if (GetConsoleMode(handle, &mode)) {
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(handle, mode);
		}
	}
	#endif

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
				printf("\33[31mErrore->Operazione non valida\n" reset);
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
			printf(RED"Errore -> L'operazione non esiste\n"reset);
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

char* getColor(int value) {
	switch (value) {
	case 1: {
		return "Red";
	}
	case 2: {
		return "Green";
	}
	case 3: {
		return "Yellow";
	}
	case 4: {
		return "Blue";
	}
	case 5: {
		return "Purple";
	}
	case 6: {
		return "Cyan";
	}
	case 7: {
		return "White";
	}
	}
}

char* getColorCode(int value) {
	switch (value) {
	case 1: {
		return "\33[31m";
	}
	case 2: {
		return "\33[32m";
	}
	case 3: {
		return "\33[33m";
	}
	case 4: {
		return "\33[34m";
	}
	case 5: {
		return "\33[35m";
	}
	case 6: {
		return "\33[36m";
	}
	case 7: {
		return "\33[37m";
	}
	}
}

int getNumberPlayer() {
	int tmpNumber = 0;

	struct Player* tmp = head;

	while (tmp != NULL) {
		tmpNumber = tmp->id;
		tmp = tmp->next;
	}

	return tmpNumber;
}

void printAvailableColor(struct Player* head) {
	struct Player* tmp = head;
	int colors[7] = { 0 };

	for (int i = 0; i < sizeof(colors) / sizeof(int); i++)
		colors[i] = i + 1;
	
	while (tmp != NULL) {

		for (int i = 0; i < sizeof(colors) / sizeof(int); i++) {
			if (colors[i] == tmp->color)
				colors[i] = -1;
		}

		tmp = tmp->next;
	}

	for (int i = 0; i < sizeof(colors) / sizeof(int); i++) {
		if (colors[i] != -1)
			printf("%d = %s\n", i + 1, getColor(colors[i]));
	}

}

void registerPlayer() {

	int tmpId = 0;
	char tmpColor[20] = { 0 };

	printf("Scegli il colore del giocatore!\n");
	printAvailableColor(head);

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