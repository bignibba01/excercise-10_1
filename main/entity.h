#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

struct Player {		//player entity
	int id;
	char color[20];
	struct Player* next;
};

void popFirstElement(struct Player**);		//toglie un valore dalla testa della coda (FIFO)
void printAllElement(struct Player*);		//visualizza tutta la coda
void pushElement(struct Player*, char*);			//aggiunge un valore alla coda dei giocatori

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
		printf("%s\n", head->color);
		head = head->next;
	}
}

void pushElement(struct Player* head, char color[20]) {

	while (head->next != NULL) {
		head = head->next;
	}

	head->next = (struct Player*)malloc(sizeof(struct Player));
	strcpy(head->next->color, color);
	head->next->next = NULL;
}

#endif // ENTITY_H_INCLUDED
