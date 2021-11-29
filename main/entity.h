#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

struct  Coord{		//coordinates of each entity
	int x;
	int y;
};

struct Player {		//player entity
	int id;
	//char* color;
	//struct Coord position;
	struct Player* next;
};

typedef struct Box {		//box entity
	short int jumpToBox;			//indica il numero della cella dove saltare, se è -1 vuol dire che è una casella che non fa nulla
	struct Coord position;
};

struct Coord getBoxPosition(struct Box);					//return Box entity position
struct Coord getPlayerPOsition(struct Player);			//return PLayer entity position

struct Coord getBoxPosition(struct Box box) {
	return box.position;
}

//struct Coord getPlayerPosition(struct Player player) {
//	return player.position;
//}

#endif // ENTITY_H_INCLUDED
