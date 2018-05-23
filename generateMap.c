// This file is just for the sake of neatness while i figure out how to
// generate a map

#define NUM_OF_HEXAGONS 19
#define NUM_DISCIPLINES 6
#define NUM_ANGLED_ARCS 44
#define NUM_VERTICAL_ARCS 24
#define VACANT_ARC 0
#define NUM_OF_VERTICES 52
#define ROW_1 3
#define ROW_2 7
#define ROW_3 12
#define ROW_4 16
#define ROW_5 19

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "Game.h"

vertex disciplineAppend(game g, map m, int vertNum);

typedef struct _vertex *link; 

typedef struct _map {
   // hexagon stores an int between 1-5 for the discipline it stores
   int hexagon[NUM_OF_HEXAGONS];
   vertex vertice[NUM_OF_VERTICES];
} Map;

typedef struct _vertex {
   unsigned long disciplines[3];
   link paths[3];
   int campus[NUM_OF_UNIS];
   int roadL = 0;
   int roadR = 0;
   int roadD = 0;
   // left, right, down
} vertex;
 
Map newMap (Game g) {
   Map m = memcpyalloc(sizeof(Map));
   int i = 0;
   unsigned long prev = NULL;
   // These are counters for rows. 
   // Each one == starting hex of each row
   int r1 = 0;
   int r2 = 4;
   int r3 = 8;
   int r4 = 13;
   int r5 = 17;

   // Loading disciplines into hexagons
   while (i < NUM_OF_HEXAGONS) {
     m->hexagon[i] = getDiscipline(i);
     i++;
   }

   // Setting all vertice disciplines and directions to NULL
    while (c < NUM_OF_VERTICES) {
      int g = 0;
      while (g <=2) {
        m->vertice[i].disciplines[g] = NULL;
        m->vertice[i].paths[g] = NULL;
        g++;
      }
      c++;
  }

   m->vertice->disciplines = disciplineAppend(g, m, 0, m->hexagon[0]);
   
   // Setting first row
   // I know this is inelegant, I will compact this once I get a working
   // pathing system
   while(r1<= 3) {
      m->vertice[r1] = disciplineAppend(m, r1, m->hexagon[0]);
      r1++;
   }
   r1 = 3;
   while (r1<= 5) {
      m->vertice[r1] = disciplineAppend(m, r1, m->hexagon[1]);
      r1++;
   }
   r1 = 5;
   while (r1<= 7) {
      m->vertice[r1] = disciplineAppend(m, r1, m->hexagon[2]);
      r1++;
   }
   while(r1<= 11) {
      m->vertice[r1] = disciplineAppend(m, r1, m->hexagon[0]);
      r1++;
   }
   r1 = 11;
   while (r1<= 13) {
      m->vertice[r1] = disciplineAppend(m, r1, m->hexagon[1]);
      r1++;
   }
   r1 = 13;
   while (r1<= 15) {
      m->vertice[r1] = disciplineAppend(m, r1, m->hexagon[2]);
      r1++;
   }




  vertex disciplineAppend(map m, int vertNum, int discipline) {
    int i = 0;
    vertex v = m->vertice[vertNum]
    // Checking if it is actually possible to append
    // Crashes if no possible places for disciplines
    assert (m->vertice[vertNum].disciplines[0] == NULL ||
            m->vertice[vertNum].disciplines[1] == NULL ||
            m->vertice[vertNum].disciplines[2] == NULL  )
    // Finds first spot that is equal to NULL
    while (i <=3 && m->vertice[vertNum].disciplines[i] != NULL) {
      i++;
    }
    v.disciplines[i] == discipline;

    return v;
  }       
  
    