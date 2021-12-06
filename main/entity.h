#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#define reset "\33[0m"

struct Coord {
	short int numberCell;			//per facilitare il salto salta direttamente nella casella numerata
	short int x;		//indice x
	short int y;		//indice y
};

struct Cell {
	short int jumptoBox;			//numero delle posizioni da saltare
	struct Coord coords;			//coordinate (indici) della casella
};

struct Player {		//player entity
	short int id;				//id del giocatore
	short int color;			//colore scelto
	struct Coord coords;		//coordinate attuali
	struct Player* next;		//puntatore all elemento della lista successivo
};

struct Coord getCoords(struct Coord);
void popQueueFirstElement(struct Player**);				//toglie un valore dalla testa della coda (FIFO)
void printQueueAllElement(struct Player*);				//visualizza tutta la coda
void pushQueueElement(struct Player*, short int, short int);				//aggiunge un valore alla coda dei giocatori

struct Coord getCoords(struct Coord coord, struct Player* player) {
	coord.x = player->coords.x;
	coord.y = player->coords.y;
	return coord;
}

void popQueueFirstElement(struct Player** head) {
	struct Player* next_node = NULL;

	if (*head == NULL) {
		return -1;
	}

	next_node = (*head)->next;
	free(*head);
	*head = next_node;
}

void printQueueAllElement(struct Player* head) {
	while (head != NULL) {
		printf("%d\n", head->id);
		head = head->next;
	}
}

void pushQueueElement(struct Player* head, short int id, short int color) {

	while (head->next != NULL) {
		head = head->next;
	}

	head->next = (struct Player*)malloc(sizeof(struct Player));
	head->next->id = id;
	head->next->color = color;
	head->next->next = NULL;
}

#endif // ENTITY_H_INCLUDED
