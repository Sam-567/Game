// This file is just for the sake of neatness while i figure out how to
// generate a map

#define NUM_OF_HEXAGONS 19
#define NUM_DISCIPLINES 6
#define NUM_ANGLED_ARCS 44
#define NUM_VERTICAL_ARCS 24
#define VACANT_ARC 0
#define NUM_OF_VERTICES 52

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "Game.h"

typedef struct _coordinate {
  // coordinates stored as a vertice number and a path leading to it.
  // see diagram included on GitHub with this file
  // e.g. a coordinate can be vertice 1, lead to by left path. 1 L
  unsigned short vert;
  unsigned char path;
} Coord;

typedef int hex;
typedef struct _vertice {
  unsigned int pos;
  unsigned char disciplines[3]
}

typedef struct _map {
  Hex hexagon[NUM_OF_HEXAGONS];
  vertice corner[NUM_OF_VERTICES];
} Map;

map generateMap(game g) {
   Map m = memcpyalloc(sizeof(Map));
   while (i < NUM_OF_HEXAGONS) {
   m->hexagon[i] = getDiscipline(i);
   i++;
  }
   m.vertice[0] m->hexagon[0]
}