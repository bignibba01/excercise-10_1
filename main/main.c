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

_Bool checkFreeColor(short int);			//scorre tutta la coda dei giocatori per vedere se un colore è gia stato selezionato
void drawMapGame();							//disegna la mappa di gioco
void drawMenu();							//disegna il menu principale
char* getColor(int);						//prende il nome del colore assegnato ad un valore intero
char* getColorCode(int);					//prende il codice del colore per disegnarlo nel terminale
int getNumberPlayer();						//prende il numero dei giocatori
void printAvailableColor();					//stampa i colori disponibili
void registerPlayer();						//inserisce il giocatore nella coda
int rollDice();								//tira il dado (1-6)

struct Player* queue = NULL;
struct Cell cells[10][10] = { 0 };

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
				printf("%sErrore->Operazione non valida\n" reset, getColorCode(1));
		} while (flag);

		switch (oper) {
		case 1: {
			if (getNumberPlayer() == -1) {
				printf("%sErrore -> Devi inserire almeno due giocatori per poter giocare.\n"reset, getColorCode(1));
				system("pause");
			}
			else {
				drawMapGame(queue);
				system("pause");

			}
			break;
			}
		case 2: {
			registerPlayer();
			system("pause");
			break;
		}
		case 3: {
			printQueueAllElement(queue);
			system("pause");
			break;
		}
		case 4: {
			end = true;
			break;
		}
		default: {
			printf("%sErrore -> L'operazione non esiste\n"reset, getColorCode(1));
		}
		}

		if (end)
			break;		//esce dal ciclo continuo

		system("cls");
		
	}

	return 0;
}

_Bool checkFreeColor(short int color) {
	struct Player* tmp = queue;

	while (tmp != NULL) {
		if (tmp->color == color) {
			printf("%sErrore -> Colore gia' in uso da un altro giocatore.\n"reset, getColorCode(1));
			return false;
		}
		tmp = tmp->next;
	}
	return true;
}

void drawMapGame(struct Player* queue) {
	_Bool flag = false;
	int number = 0;
	for (int i = 0; i < 10; i++) {
		if (i == 0)
			printf(" _____");
		else if (i == 9)
			printf("_____");
		else
			printf("______");
	}
	puts("_");				//prima riga disegnata

	short int indexX = 0, indexY = 0;

	for (int k = 0; k < 10; k++) {
		for (int j = 0; j < 3; j++) {
			if (j == 2) {
				for (int i = 0; i < 10; i++) {
					printf("|_____");
				}
			}
			else {
				for (int i = 0; i < 10; i++) {		//draw 10 cells, j è il numero della riga della casella
					if (j == 0) {
						flag = true;
						printf("|%5d", ++number);
						cells[indexX][indexY++].numberCell = number;		//salvo il valore della cella nella matrice per conoscere la cella di destinazione
					}
					else if (j == 1) {		//sono al centro della casella
						struct Player* tmp = queue;
						flag = false;

						while (tmp != NULL) {

							if (getCoords(tmp->coords, tmp).x == 0)
								printf("|  %s%c  "reset, getColorCode(tmp->color), 254);
							tmp = tmp->next;
						}

					}else
						printf("|     ");
				}
			}
			if (flag) {
				indexY = 0;
				indexX += 1;
			}
			puts("|");
		}
	}
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
		return "\33[31m";		//red
	}
	case 2: {
		return "\33[32m";		//green
	}
	case 3: {
		return "\33[33m";		//yellow
	}
	case 4: {
		return "\33[34m";		//blue
	}
	case 5: {
		return "\33[35m";		//purple
	}
	case 6: {
		return "\33[36m";		//cyan
	}
	case 7: {
		return "\33[37m";		//white
	}
	default: {
		return "\33[0m";
	}
	}
}

int getNumberPlayer() {
	int tmpNumber = 0;
	struct Player* tmp = queue;		//salvo una copia della testa della lista

	while (tmp != NULL) {
		tmpNumber = tmp->id;
		tmp = tmp->next;
	}

	return tmpNumber;
}

void printAvailableColor(struct Player* queue) {
	struct Player* tmp = queue;
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

	short int tmpId = 0;
	short int tmpColor = 0;

	struct Player* tmp = queue;
	while (tmp != NULL) {
		tmpId = tmp->id + 1;			//preleva l'id dall ultimo elemento della lista e lo incrementa
		tmp = tmp->next;
	}
	if (tmpId == 6) {				//6 perche tmpId = tmp->id + 1 (5+1)
		printf("Numero massimo di giocatori raggiunto!\n");
	}
	else {
		_Bool flag = false;
		printf("Scegli il colore del giocatore!\n");

		do {
			printAvailableColor(queue);
			scanf(" %hd", &tmpColor);
			
			if (tmpColor >= 7 || tmpColor <= 0) {
				flag = false;
				printf("%sErrore -> Il valore non e' valido.\n"reset, getColorCode(1));
			}else
				flag = checkFreeColor(tmpColor);
		} while (!flag);

		if (queue == NULL) {			//se il primo elemento della lista è nullo allora inserisce il primo valore
			queue = (struct Player*)malloc(sizeof(struct Player));			//alloca lo spazio

			queue->id = 1;
			queue->color = tmpColor;
			queue->coords.x = 0;
			queue->coords.y = 0;
			queue->next = NULL;			//attribuisce al puntatore all elemento successivo il valore NULL perche ancora non esiste
		}
		else {		//se invece il primo elemento già esiste allora esegue un push
			pushQueueElement(queue, tmpId, tmpColor);
		}
	}
}

int rollDice() {
	srand(time(NULL));

	return rand() % 6 + 1;
}