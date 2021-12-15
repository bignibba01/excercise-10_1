/*
Si scriva un programma C, adeguatamente commentato, per la generazione di partite del "Gioco dell'oca" (Snakes & Ladders nella versione inglese).
Il gioco consistente nel far competere più giocatori al raggiungimento della casella finale, avanzando di volta in volta tra caselle diverse tramite il tiro di un dado a 6 facce.
---MORE INFO ONLINE---
*/

#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
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

int askQuestion();											//chiede all' utente una domanda se si trova in una delle caselle per saltarne altre
_Bool checkAnswer(short int, char[255]);					//controlla se la risposta inserita è esatta o meno
_Bool checkFreeColor(short int);							//scorre tutta la coda dei giocatori per vedere se un colore è gia stato selezionato
void defineCellStatus();									//definisce l' azione che le caselle compiono
struct Coord drawMapGame(struct Player*);					//disegna la mappa di gioco
void drawMenu();											//disegna il menu principale
char* getColor(int);										//prende il nome del colore assegnato ad un valore intero
char* getColorCode(int);									//prende il codice del colore per disegnarlo nel terminale
int getNumberPlayer();										//prende il numero dei giocatori
int getNumberOfQuestion();									//prende il numero delle domande presenti nel file
void prepareNumberCells();									//prepara la matrice con i numeri
void printAvailableColor();									//stampa i colori disponibili
void registerPlayer();										//inserisce il giocatore nella coda
char* readUntilQuestion(short int);							//legge il file fino all'inizio di quella domanda
int rollDice();												//tira il dado (1-6)
void setPlayers(struct Player* queue);						//start position of player

struct Player* queue = NULL;
struct Cell cells[10][10] = { 0 };
char *questionPath = "..\\File\\FileDomande.txt", *answerPath = "..\\File\\FileRisposte.txt";

int main() {
	srand(time(NULL));

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

			if (getNumberPlayer() <= 1) {
				printf("%sErrore -> Devi inserire almeno due giocatori per poter giocare.\n"reset, getColorCode(1));
				system("pause");
			}
			else {
				struct Coord currentPlayer = { 0 };
				setPlayers(queue);
				prepareNumberCells();
				defineCellStatus();

				while (true) {
					currentPlayer = drawMapGame(queue);			//disegna la mappa di gioco
					struct Player tryMeem = tryPop(&queue);				//giocatore che dovrà giocare il turno
					_Bool questionFlag = false;

					printf("Turno del giocatore: %s%d\n"reset, getColorCode(tryMeem.color), tryMeem.id);
					printf("NumeroCasella: %d\n", currentPlayer.numberCell);

					//se non è nulla di queste vuol dire che è una casella vuota
					if (cells[currentPlayer.x][currentPlayer.y].status >= 1) {			//vuol dire che il giocatore si trova su una casella di salto
						if (cells[currentPlayer.x][currentPlayer.y].status == 1) {				//la casella fa andare indietro il giocatore
							tryMeem.coords.numberCell = cells[currentPlayer.x][currentPlayer.y].jumptoBox;
						}
						else if (cells[currentPlayer.x][currentPlayer.y].status == 2) {				//la casella fa andare avanti il giocatore se risponde correttamente ad una domanda
							int numberQuestion = askQuestion();
							printf("Risposta: ");
							char risp[255];
							scanf(" %s", risp);
							fflush(stdin);
							for (int i = 0; i < strlen(risp); i++)
								risp[i] = tolower(risp[i]);

							if ((questionFlag = checkAnswer(numberQuestion, risp)) == true) {
								printf("Risposta esatta!\n");				//salta in avanti
								tryMeem.coords.numberCell = cells[currentPlayer.x][currentPlayer.y].jumptoBox;
							}
							else {
								printf("Risposta sbagliata!\n");				//non salta
							}


						}
					}else if (cells[currentPlayer.x][currentPlayer.y].status == -1){			//salta il turno
						printf("Fermo!\n");
					}
					if (!questionFlag) {				//se non ha risposto correttamente alla domanda salta in avanti senza tirare il dado
						printf("Tira il dado! ");
						system("pause");
						int number = rollDice();
						tryMeem.coords.numberCell += number;

						printf("%s%d\n"reset, getColorCode(tryMeem.color), number);
					}
					pushTurnQueue(queue, tryMeem.id, tryMeem.color, tryMeem.coords.numberCell);			//metto in coda l' elemento tolto ocn il pop precedente
					system("pause");
					system("cls");
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

int askQuestion() {
	int numberQuestion = getNumberOfQuestion();

	int question = rand() % numberQuestion + 1;
	char* x = readUntilQuestion(question);
	return question;
}

_Bool checkAnswer(short int question, char risp[255]) {
	FILE* answerFile = NULL;

	if ((answerFile = fopen(answerPath, "r")) == NULL) {
		printf("%sError -> File not found\n"reset, getColorCode(1));
	}

	char res, buffer[255];
	int cont = 0;
	while (true) {				//scorre tutto il file e ogni volta che trova uno \n incrementa un contatore delle domande
		if (cont == question - 1) {
			break;
		}
		res = getc(answerFile);
		if (res == '\n') {
			cont++;
		}
		else if (res == EOF) {
			cont++;
			break;
		}

	}

	int i = 0;
	while (true) {			//legge la domanda
		res = getc(answerFile);
		if (res == '\n' || res == EOF)
			break;
		else {
			buffer[i++] = tolower(res);
		}

	}
	buffer[i] = '\0';

	if (strcmp(buffer, risp) == 0)
		return true;
	else
		return false;

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

void defineCellStatus(){
	int num = 0, casella = 0;

	for (int i = 0; i < 7; i++) {					//7 per indicare il numero di salti in avanti che saranno presenti nel tabellone
		_Bool flag = false;
		int x = 0, y = 0, cont = 0;

		while (true) {
			
			casella = rand() % (99 - 2) + 2;

			for (int j = 0; j < 10; j++) {
				for (int k = 0; k < 10; k++)
					if (cells[j][k].coords.numberCell == casella) {
						x = j;
						y = k;
						flag = true;
						break;
					}
				if (flag)					//se ha trovato la casella esce dai cicli
					break;
			}
			if (cells[x][y].status == 0)			//se status è 0 vuol dire che le caselle non hanno alcuna azione
				break;
			if (cont++ == 5)			///limite per cercare una casella
				break;
		}
		cells[x][y].status = -1;			//-1 vuol dire che il giocatore salterà un turno

	}//FINE caselle che saltano turni

	for (int i = 0; i < 7; i++){					//7 per indicare il numero di salti in avanti che saranno presenti nel tabellone
		_Bool flag = false;
		int x = 0, y = 0, cont = 0;

		while (true) {
			casella = rand() % 100 + 1;						//numero casuale da 1 a 100 compresi
			for (int j = 0; j < 10; j++) {
				for (int k = 0; k < 10; k++)
					if (cells[j][k].coords.numberCell == casella) {
						x = j;
						y = k;
						flag = true;
						break;
					}
				if (flag)					//se ha trovato la casella esce dai cicli
					break;
			}
			if (cells[x][y].status == 0 && casella < 70)
				break;
			if (cont++ == 5)			///limite per cercare una casella
				break;
		}
		cont = 0;
		while (true){
			num = rand() % (26 - 5) + 5;				//numero casuale da 5 a 25 per definire il numero di caselle che salta il giocatore
			int sum = cells[x][y].coords.numberCell + num;
			if (sum <= 100 && sum >= 1){
				cells[x][y].jumptoBox = sum;
				cells[x][y].status = 2;				//2 vuol dire che farà dei salti
				break;
			}
			if (cont++ == 5)			///limite per cercare una casella
				break;
		}
	}//FINE caselle che saltano avanti

	for (int i = 0; i < 7; i++) {					//7 per indicare il numero di salti indietro che saranno presenti nel tabellone
		_Bool flag = false;
		int x = 0, y = 0, cont = 0;

		while (true) {
			casella = rand() % 100 + 1;						//numero casuale da 1 a 100 compresi

			for (int j = 0; j < 10; j++) {
				for (int k = 0; k < 10; k++)
					if (cells[j][k].coords.numberCell == casella) {
						x = j;
						y = k;
						flag = true;
						break;
					}
				if (flag)					//se ha trovato la casella esce dai cicli
					break;
			}
			if (cells[x][y].status == 0 && casella > 26)
				break;
			if (cont++ == 5)			///limite per cercare una casella
				break;
		}
		cont = 0;
		while (true) {
			num = rand() % (26 - 5) + 5;				//numero casuale da 5 a 25 per definire il numero di caselle che salta il giocatore
			int diff = cells[x][y].coords.numberCell - num;
			if (diff <= 100 && diff >= 1) {
				cells[x][y].jumptoBox = diff;
				cells[x][y].status = 1;						//1 vuol dire che farà dei salti indietro
				break;
			}
			if (cont++ == 5)			///limite per cercare una casella
				break;
		}
	}//FINE caselle che saltano indietro

}

struct Coord drawMapGame(struct Player* queue) {
	struct Coord playerCoords = { 0 };
	short int indexX = 0, indexY = 0, x = 0, y = 0;			//indici per la gestione dell' output delle celle
	char start = 192, midCorner = 193, mid = 196, endChar = 217;		//caratteri ASCII per disegnare la tabella

	for (int i = 0; i < 10; i++) {
		if (i == 0)
			printf(" _____");
		else if (i == 9)
			printf("_____");
		else
			printf("______");
	}
	puts("_");				//prima riga disegnata

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

						if (cells[indexX][indexY].status == 0)					//indica una cella vuota, quindi il colore del numero sarà bianco
							printf("|%s%5d"reset, getColorCode(7), cells[indexX][indexY].coords.numberCell);

						else if (cells[indexX][indexY].status == -1)			//indica un salto di turno, quindi il colore del numero sarà rosso
							printf("|%s%5d"reset, getColorCode(1), cells[indexX][indexY].coords.numberCell);

						else if (cells[indexX][indexY].status == 1)				//indica un salto indietro, quindi il colore del numero sarà giallo
							printf("|%s%5d"reset, getColorCode(3), cells[indexX][indexY].coords.numberCell);

						else if (cells[indexX][indexY].status == 2)				//indica un salto in avanti, quindi il colore del numero sarà verde
							printf("|%s%5d"reset, getColorCode(2), cells[indexX][indexY].coords.numberCell);

						indexY++;				//incremento il suo indice
					}
					else if (j == 1) {		//sono al centro della casella
						struct Player* tmp = queue;
						printf("|");		//stampo il corpo della tabella
						int firstPlayerId = tmp->id;			//mi serve per conoscere quale sarà il giocatore che dovrà giocare e prendere le sue informazioni
						for (int i = 0; i < 5; i++) {			//stampo i valori di ogni cella
							if (tmp != NULL) {
								if (tmp->coords.numberCell == cells[x][y].coords.numberCell) {		//se coincidono i numeri della cella allora stampa il quadratino
									printf("%s%d"reset, getColorCode(tmp->color), tmp->id);
									tmp->coords.x = x;
									tmp->coords.y = y;
									if (tmp->id == firstPlayerId) {
										playerCoords.numberCell = tmp->coords.numberCell;
										playerCoords.x = tmp->coords.x;
										playerCoords.y = tmp->coords.y;
									}
								}
								else
									printf(" ");			//se non coincidono stampa uno spazio vuoto
								tmp = tmp->next;
							}
							else {
								printf(" ");			//se la lista non contiene valori stampa vuoto
							}
						}
						y++;
					}
				}
			}
			if (j == 0) {
				indexY = 0;
				indexX += 1;
			}
			else if (j == 1) {
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
	return playerCoords;
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

int getNumberOfQuestion() {
	FILE* questionFile;

	if ((questionFile = fopen(questionPath, "r")) == NULL) {
		printf("%sError -> File not found\n"reset, getColorCode(1));
	}

	char res;
	int cont = 0;
	while (true) {
		res = getc(questionFile);
		if (res == '\n') {
			cont++;
		}
		else if (res == EOF) {
			cont++;
			break;
		}

	}
	return cont;
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
			cells[x][y].status = 0;

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

char* readUntilQuestion(short int numberQuestion) {
	FILE* questionFile;
	char* question = NULL;

	if ((questionFile = fopen(questionPath, "r")) == NULL) {
		printf("%sError -> File not found\n"reset, getColorCode(1));
	}

	char res, buffer[255];
	int cont = 0;
	while (true) {				//scorre tutto il file e ogni volta che trova uno \n incrementa un contatore delle domande
		if (cont == numberQuestion - 1) {
			break;
		}
		res = getc(questionFile);
		if (res == '\n') {
			cont++;
		}
		else if (res == EOF) {
			cont++;
			break;
		}

	}
	int i = 0;
	while (true) {			//legge la domanda
		res = getc(questionFile);
		if (res == '\n' || res == EOF)
			break;
		else {
			buffer[i++] = res;
		}

	}
	buffer[i] = '\0';

	printf("%s\n", buffer);

	return *buffer;
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
			}
			else
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
	
	/*time_t start = clock(), current = 0;

	do {
		printf("%d\r", rand() % 6 + 1);
		current = clock();
	} while ((((float)current - (float)start) / CLOCKS_PER_SEC) <= 3);*/
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
