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
	short int status;				//se -1 = salta un turno; se 0 = casella vuota; se 1 = salta caselle indietro; se 2 = salta caselle in avanti
	struct Coord coords;			//coordinate (indici) della casella
};

struct Player {		//player entity
	short int id;				//id del giocatore
	short int color;			//colore scelto
	_Bool isBlocked;
	struct Coord coords;		//coordinate attuali
	struct Player* next;		//puntatore all elemento della lista successivo
};

void changeHeadNumberCell(struct Player**, struct Player);					//aggiunge un elemento alla testa della coda per poter effettuare il salto all'indietro
void popQueueFirstElement(struct Player**);									//toglie un valore dalla testa della coda (FIFO)
void printQueueAllElement(struct Player*);									//visualizza tutta la coda
void pushQueueElement(struct Player*, short int, short int);				//aggiunge un valore alla coda dei giocatori
void pushTurnQueue(struct Player*, struct Player);							//aggiunge i valori del giocatore alla queue
struct Player tryPop(struct Player**);

void changeHeadNumberCell(struct Player** head, struct Player tmp) {
	struct Player* node = NULL;
	node = (struct Player*)malloc(sizeof(struct Player));			//mi creo un elemento temporaneo

	node->id = tmp.id;
	node->color = tmp.color;
	node->coords.numberCell = tmp.coords.numberCell;
	node->isBlocked = tmp.isBlocked;
	node->next = *head;					//assegno i valori e l' elemento puntato che sarebbe la testa corrente della coda

	*head = node;				//modifico l'indirizzo della testa della coda

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

struct Player tryPop(struct Player** head) {
	struct Player* next_node = NULL;
	struct Player tryMeme;

	if (*head == NULL) {
		return;
	}

	next_node = (*head)->next;
	tryMeme = **head;			//salva il giocatore in testa alla coda
	free(*head);
	*head = next_node;
	return tryMeme;
}

void pushQueueElement(struct Player* head, short int id, short int color) {

	while (head->next != NULL) {
		head = head->next;
	}

	head->next = (struct Player*)malloc(sizeof(struct Player));				//inserimento iniziale per assegnare i giocatori
	head->next->id = id;
	head->next->color = color;
	head->next->next = NULL;
}

void pushTurnQueue(struct Player* head, struct Player tryMeem) {

	while (head->next != NULL) {
		head = head->next;
	}

	head->next = (struct Player*)malloc(sizeof(struct Player));					//inserimento di tutti i parametri dei giocatori
	head->next->id = tryMeem.id;
	head->next->color = tryMeem.color;
	head->next->coords.numberCell = tryMeem.coords.numberCell;
	head->next->isBlocked = tryMeem.isBlocked;
	head->next->next = NULL;
}

#endif // ENTITY_H_INCLUDED
