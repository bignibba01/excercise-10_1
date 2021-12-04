#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#define reset "\33[0m"

struct Player {		//player entity
	short int id;
	short int color;
	struct Player* next;
};

void popFirstElement(struct Player**);				//toglie un valore dalla testa della coda (FIFO)
void printAllElement(struct Player*);				//visualizza tutta la coda
void pushElement(struct Player*, int);			//aggiunge un valore alla coda dei giocatori

void popFirstElement(struct Player** head) {
	struct Player* next_node = NULL;

	if (*head == NULL) {
		return -1;
	}

	next_node = (*head)->next;
	free(*head);
	*head = next_node;
}

void printAllElement(struct Player* head) {
	while (head != NULL) {
		printf("%d\n", head->id);
		head = head->next;
	}
}

void pushElement(struct Player* head, int id) {

	while (head->next != NULL) {
		head = head->next;
	}

	head->next = (struct Player*)malloc(sizeof(struct Player));
	head->next->id = id;
	head->next->next = NULL;
}

#endif // ENTITY_H_INCLUDED
