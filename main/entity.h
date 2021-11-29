#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

struct Player {		//player entity
	int id;
	char color[20];
	struct Player* next;
};
#endif // ENTITY_H_INCLUDED
