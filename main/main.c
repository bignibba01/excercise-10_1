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

_Bool checkFreeColor(short int);				//scorre tutta la coda dei giocatori per vedere se un colore è gia stato selezionato
void drawMapGame();								//disegna la mappa di gioco
void drawMenu();								//disegna il menu principale
char* getColor(int);							//prende il nome del colore assegnato ad un valore intero
char* getColorCode(int);						//prende il codice del colore per disegnarlo nel terminale
int getNumberPlayer();							//prende il numero dei giocatori
void prepareNumberCells();						//prepara la matrice con i numeri
void printAvailableColor();						//stampa i colori disponibili
void registerPlayer();							//inserisce il giocatore nella coda
int rollDice();									//tira il dado (1-6)
void setPlayers(struct Player*);				//start position of player
void startTurn(struct Player*);					//gestisce il turno dei giocatori

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
				setPlayers(queue);
				prepareNumberCells();
				while (true) {
					drawMapGame();
					startTurn(queue);
					system("pause");

				}

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

void drawMapGame() {
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

	short int indexX = 0, indexY = 0, x = 0, y = 0;			//indici per la gestione dell' output delle celle
	char start = 192, midCorner = 193, mid = 196, endChar = 217;		//caratteri ASCII per disegnare la tabella

	for (int k = 0; k < 10; k++) {			//ciclo for per ogni colonna
		for (int j = 0; j < 3; j++) {		//ciclo for per le 3 righe di ogni cella
			if (j == 2) {					//ultima riga di ogni cella
				for (int i = 0; i < 10; i++) {
					if (k != 9)
						printf("|_____");			//stampa l'ultima riga di ogni cella
					else if (k == 9)			//mi serve per chiudere l'ultima riga della tabella
						printf("|     ");
				}
			}
			else {
				for (int i = 0; i < 10; i++) {		//draw 10 cells, j è il numero della riga della casella
					if (j == 0) {			//prima riga in alto di ogni cella
						flag = true;		//flag per incrementare gli indici della matrice di output
						printf("|%5d", cells[indexX][indexY++].coords.numberCell);			//stampo il numero della cella		
					}
					else if (j == 1) {		//sono al centro della casella
						struct Player* tmp = queue;
						flag = false;
						printf("|");		//stampo il corpo della tabella

						for (int i = 0; i < 5; i++) {			//stampo i valori di ogni cella
							if (tmp != NULL) {
								if (tmp->coords.numberCell == cells[x][y].coords.numberCell)		//se coincidono i numeri della cella allora stampa il quadratino
									printf("%s%c"reset, getColorCode(tmp->color), 254);
								else
									printf(" ");			//se non coincidono stampa uno spazio vuoto
								tmp = tmp->next;
							}
							else {
								printf(" ");			//se la lista non contiene valori stampa vuoto
							}
						}
						y++;			//incremento l'indice della matrice
					}
				}
			}
			if (flag) {
				indexY = 0;
				indexX += 1;
			}
			else if (!flag) {
				x++;
				y = 0;
			}
			puts("|");		//inserisce l'ultima barra alla fine di ogni riga
		}
	}

	for (int i = 0; i < 10; i++) {		//stampa l'ultima riga della tabella con caratteri ascii particolari
		if (i == 0)
			printf("%c%c%c%c%c%c", start, mid, mid, mid, mid, mid);
		else if (i == 9)
			printf("%c%c%c%c%c%c%c\n", midCorner, mid, mid, mid, mid, mid, endChar);
		else
			printf("%c%c%c%c%c%c", midCorner, mid, mid, mid, mid, mid);
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

void prepareNumberCells() {
	int x = 0, y = 0, number = 0;
	_Bool flag = true;
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (y == -1) {
				flag = true;
				y++;
			}
			if (y == 10) {
				flag = false;
				y--;
			}

			number += 1;
			cells[x][y].coords.numberCell = number;
			
			if (flag) {
				y++;
			}
			else {
				y--;
			}
			
		}
		x++;
	}
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

void setPlayers(struct Player* queue) {

	while (queue != NULL) {
		queue->coords.numberCell = 1;
		queue->coords.x = 0;
		queue->coords.y = 0;
		queue = queue->next;
	}

}

void startTurn(struct Player* queue) {
	struct Player tmp;
	tmp = popQueueFirstElement(&queue);
	pushToTail(queue, tmp.id, tmp.color, tmp.coords.numberCell);
}