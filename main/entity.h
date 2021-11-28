#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

typedef struct {
	int x;
	int y;
}Coord;

typedef struct {
	int id;
	char* color;
	Coord position;
}Player;

typedef struct {
	short int action;
	Coord position;
}Box;

#endif // ENTITY_H_INCLUDED
