// Matthew Rossouw, Sam Chapman, Justin Lou, Mia Pahljina 
// z5220299, z5218557, *justin zid*, *mia zid* (respectively)
// HS1511, Queenwood Tute, Bryan Moh
// 15/05/18
// Game.c for KI

// File that defines all features and functions for a Knowledge Island
// game, implemented according to game.h. Designed to interface with a
// Mechanical Turk

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

void exchangeStudents(Game g, int player, action a, int exchangeRate);
int isLegalParamaters(action a);
int hasCampus(Game g, int player, path location);
int prevARC(g, path);
vertex pathTrace(path pathToCheck)

typedef struct _game {
  int disciplines[NUM_OF_HEXAGONS];                            //What discipline each hex tile is
  int rollNeeded[NUM_OF_HEXAGONS];
  int turnCount;                                               //Number of terms passed
  int KPIpoints[NUM_UNIS];                                     //The KPI points of each uni
  int patents[NUM_UNIS];                                       //The number of patents each uni has
  int publications[NUM_UNIS];                                  //The number of publications each uni has
  int students[NUM_UNIS][NUM_DISCIPLINES];
  int currentRoll;
  int mostPublications;                                        //Uni with the most publications
  int mostARCs;                                                //Uni with the most ARCs
  int GO8s[NUM_UNIS];                                          //The number of GO8s each uni has
  int arcs[NUM_UNIS];                                          //The number of arcs each uni has
  int campuses[NUM_UNIS];                                      //The number of campuses each uni has
  /*int totalGO8s;                                             //Total number of GO8s
  int totalArcs;                                               //Total number of arcs
  int totalCampuses;  */                                       //Total number of arcs
  int GO8Locs[NUM_UNIS][NUM_OF_VERTICES];                      //Locations of the GO8s
  int arcLocs[NUM_UNIS][NUM_ANGLED_ARCS + NUM_VERTICAL_ARCS];  //Locations of the arcs
  int campusLocs[NUM_UNIS][NUM_OF_VERTICES];                   //Locations of the arcs
} game;

typedef struct _vertex *link; 

typedef struct _map {
   // hexagon stores an int between 1-5 for the discipline it stores
   int hexagon[NUM_OF_HEXAGONS];
   vertex vertice[NUM_OF_VERTICES];
} Map;

typedef struct _road {
   char *origin;
   int owner[NUM_OF_UNIS];
} road;

typedef struct _vertex {
   int disciplines[3];
   link paths[3];
   int campus[NUM_OF_UNIS];
   int roadL = 0;
   int roadR = 0;
   int roadD = 0;
   // l,r,b
} vertex;
 
Map newMap (Game g) {
  Map m = memcpyalloc(sizeof(Map));
  int i = 0;
  int c = 0;

  // Loading disciplines into hexagons
  while (i < NUM_OF_HEXAGONS) {
    m->hexagon[i] = getDiscipline(i);
    i++;
  }

  m->vertice[0].paths = [m|v[1], m|v[4], NULL];
  m->vertice[0].disciplines = [m->hexagon[8], NULL, NULL];
  m->vertice[1].paths = [NULL, m|v[5], m|v[1]];
  m->vertice[1].disciplines = [m->hexagon[8], NULL, NULL];
  m->vertice[2].paths = [m|v];
  m->vertice[2].disciplines = [m->hexagon[4], NULL, NULL];

  //DIFFERENT WAY

   // first row
   while(h<= ROW_ONE) {
      v->disciplines = hexagon[i];
      v = v.next;
      i++
   }
   while (second row){
      i = 
   }



  }


Game newGame (int discipline[], int dice[]){

   Game g = memcpyalloc(sizeof(game));//Mallocs the game, we gotta find a way to clear this.
   assert(g != NULL);

   memset(g, 0, sizeof(game));

   memcpy(g->disciplines, discipline, NUM_OF_HEXAGONS);
   memcpy(g->rollNeeded, dice, NUM_OF_HEXAGONS);

   g->turnCount = -1;
   g->mostPublications = NO_ONE;
   g->mostARCs = NO_ONE;

   //These are -1 as 1 gets automatically added every thing they're called
   g->totalGO8s = -1;
   g->totalArcs = -1;
   g->totalCampuses = -1;

   return g;
}
 
void disposeGame (Game g){
   free(g);
}
 
void makeAction (Game g, action a){
   assert(isLegalAction (g, a) == TRUE);
   assert(a.actionCode != START_SPINOFF);
   assert(isLegalAction (g, a));

   int move = a.actionCode;
   int player = getWhoseTurn(g);
   assert(player > 0);

   //PLEASE NOTE: NONE OF THESE CURRENTLY USE THE NEW PATH SYSTEM OR CHECK VALIDITY
   if (move == PASS){
      // DO NOTHING
   } else if (move == BUILD_CAMPUS){
      g->campuses[player] += 1;
      g->campusLocs[player][campuses[player]] = g->destination;
      
   } else if (move == BUILD_GO8){
      g->GO8s[player] += 1;
      g->GO8Locs[player][GO8s[player]] = g->destination;
      
   } else if (move == OBTAIN_ARC){
      g->arcs[player] += 1;
      g->arcLocs[player][arcs[player]] = g->destination;
      
   } else if (move == START_SPINOFF){
      int rand = rand() % 3;
      //If rand is 0, give IP Patent. If not, give research publication
      if (rand == 0){
         g->patents[player] += 1;
      } else{
         g->publications[player] += 1;
      }


   } else if (move == RETRAIN_STUDENTS){
      int rate = getExchangeRate (g, player, a.disciplineFrom, a.disciplineTo);
      exchangeStudents(g, player, a, rate);
   }
}

void throwDice (Game g, int diceScore){
   //Turning all MTV and M$ students to ThDs
   if(diceScore == 7){
      int i = 1;
      while(i <= NUM_UNIS){
         int numMMoney = g->students[i][STUDENT_MMONEY];
         int numMTV = g->students[i][STUDENT_MTV];

         g->students[i][STUDENT_THD] += numMTV + numMMoney;
         g->students[i][STUDENT_MTV] = 0;
         g->students[i][STUDENT_MMONEY] = 0;
      }
   }
   g->currentRoll = diceScore;
   g->turnCount++;
}

//Exchange rate is the number of students for a discipline for 1 of another e.g. 3 means 3 students give 1 in return
void exchangeStudents(Game g, int player, action a, int exchangeRate){
   assert(a.actionCode == RETRAIN_STUDENTS);
   int to = g->students[player][a.disciplineTo];
   int from = g->students[player][a.disciplineFrom];
   assert(from != STUDENT_THD);

   while(to >= exchangeRate){
      from++;
      to -= exchangeRate;
   }
   g->students[player][a.disciplineTo] =  to;
   g->students[player][a.disciplineFrom] = from;
} 
/* **** Functions which GET data about the game aka GETTERS **** */
 
path coordToString(game g, coord coordinate) {
  // works out the best path to a coordinate first

}


int getDiscipline (Game g, int regionID){
	assert(regionID < NUM_OF_HEXAGONS);
	return g->disciplines[regionID];
}
 

int getDiceValue (Game g, int regionID){
	assert(regionID < NUM_OF_HEXAGONS);
	return g->rollNeeded[regionID];
}
 
// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game
// has started. 
int getMostARCs (Game g){   
   int UniWithMost = g->mostARCs;
   int MostARCs = getARCs(g, UniWithMost);
   int uni = 1;
   while (uni <= NUM_UNIS) {
      int uniARCs = getARCs(g, uni);

      if (uniARCs < MostARCs) {
         MostARCs = uniARCs;
         UniWithMost = uni;
      }
      uni++;
   }
   g->mostARCs = UniWithMost;
   return UniWithMost;
}
 
// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is made.
int getMostPublications (Game g){
   int UniWithMost = g->mostPublications;
	int MostPublications = getPublications(g, UniWithMost);
	int uni = 1;
	while (uni <= NUM_UNIS) {
		int uniPublications = getPublications(g, uni);

		if (uniPublications < MostPublications) {
			MostPublications = uniPublications;
			UniWithMost = uni;
		}
		uni++;
	}
   g->mostPublications = UniWithMost;
	return UniWithMost;
}
 
// return the current turn number of the game -1,0,1, ..
int getTurnNumber (Game g){
   int turnNumber = g->turnCount;
   return (turnNumber);
}
 
// return the player id of the player whose turn it is
// the result of this function is NO_ONE during Terra Nullis
int getWhoseTurn (Game g){

   int playerTurn;

   if (g->turnCount != -1){
      playerTurn = ((g->turnCount)%3) + 1;
   } else {
      playerTurn = NO_ONE;
   }
   
   return (playerTurn);
}
 
// return the contents of the given vertex (ie campus code or
// VACANT_VERTEX)
int getCampus(Game g, path pathToVertex);
 
// the contents of the given edge (ie ARC code or vacent ARC)
int getARC(Game g, path pathToEdge) {
  int contents = VACANT_ARC;
    if (insideMap(g, path) == TRUE) {

    }

  // To be completed


  return contents;
}
 
// returns TRUE if it is legal for the current
// player to make the specified action, FALSE otherwise.
//
// "legal" means everything is legal:
//   ////////* that the action code is a valid action code which is legal to
//   ////////  be made at this time DONE
//   * that any path is well formed and legal ie consisting only of
//     the legal direction characters and of a legal length,
//     and which does not leave the island into the sea at any stage.
//   * that disciplines mentioned in any retraining actions are valid
//     discipline numbers, and that the university has sufficient
//     students of the correct type to perform the retraining
//
// eg when placing a campus consider such things as:
//   * is the path a well formed legal path
//   * does it lead to a vacent vertex?
//   * under the rules of the game are they allowed to place a
//     campus at that vertex?  (eg is it adjacent to one of their ARCs?)
//   * does the player have the 4 specific students required to pay for
//     that campus?
// It is not legal to make any action during Terra Nullis ie
// before the game has started.
// It is not legal for a player to make the moves OBTAIN_PUBLICATION
// or OBTAIN_IP_PATENT (they can make the move START_SPINOFF)
// you can assume that any pths passed in are NULL terminated strings.
int isLegalAction (Game g, action a){
   int legality = FALSE;
   int player = getWhoseTurn(g); 
   if (isLegalParamaters(a) == TRUE) {
      
      // If player wants to pass
      if (a.actionCode == PASS) {
        legality = TRUE;

      } else if (a. actionCode == BUILD_CAMPUS) {
         // Check that player has enough resources to build Campus
         // 1x BPS, 1x B?, 1x MJ, 1x MTV
         if (g->students[player][STUDENT_BPS] >= 1 &&
             g->students[player][STUDENT_BQN] >= 1 &&
             g->students[player][STUDENT_MJ]  >= 1 &&
             g->students[player][STUDENT_MTV] >= 1 ){

            // Checks that the vertex below is vacant
            if (getCampus(g, a.path) == VACANT_VERTEX) {
              // !!! Can add another function here to check path validity !!!
              
              //Checks if the edge before this vertex is owned by the player
              if (getARC(g, path) == (player)) {
                legality = TRUE;
              }
            }
         }
      } else if (a.actionCode == BUILD_GO8) {
         // Check that the player has enough resources tp upgrade
         // 2x MJ, 3x M$
         if (g->students[player][STUDENT_MJ]     >= 2 &&
             g->students[player][STUDENT_MMONEY] >= 3 ){

             //Checks that the vertice below is valid
            if (getCampus(g, a.path) == (player)) {
              legality = TRUE;
              // Could add a line here to check path before, but i
              // think this is redundant.
         }
         
      } else if (a.actionCode == OBTAIN_ARC) {
         // Check that the player has enough resources
         // 1x B?, 1x BPS
         if (g->students[player][STUDENT_BQN] >= 1 &&
             g->students[player][STUDENT_BPS] >= 1 ){
            if (getARC(g, path) == VACANT_ARC) {
              if (prevARC(g, path) == player) {
              legality == TRUE;
              
              }
            }
         }
      } else if (a.actionCode == START_SPINOFF) {
         // Check that the player has enough resources
         // 1x MJ, 1x MTV, 1x M$
         if (g->students[player][STUDENT_MJ]     >= 1 &&
             g->students[player][STUDENT_MTV]    >= 1 &&
             g->students[player][STUDENT_MMONEY] >= 1 ){
             legality = TRUE;
         }
      } else if (a.actionCode == OBTAIN_PUBLICATION) {
         // isLegalAction is only called for player actions.
         // players are never allowed to call this action 
         legality = FALSE;
      } else if (a.actionCode == OBTAIN_IP_PATENT) {
         // isLegalAction is only called for player actions.
         // players are never allowed to call this action 
         legality = FALSE;
      } else if (a.actionCode == RETRAIN_STUDENTS) {
         // Checks that the player has enough students to 
         // satisfy the exchange rate
         if (g->students[player][a.disciplineFrom] >= 
             getExchangeRate(g, player, a.disciplineFrom, a.disciplineTo)) {
               legality = TRUE;
         }
      }
   }
   // Actions cannot be made before the game starts
   if (turnCount < 1);
      legality == FALSE;

   return legality;
}

 
// --- get data about a specified player ---
 
// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, int player){
   player--;
   return g->KPIpoints[player];
}
 
// return the number of ARC grants the specified player currently has
int getARCs (Game g, int player){
   assert(player > 0);
  return g->arcs[player-1];
}
 
// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, int player){
   assert(player > 0);
   return g->GO8s[player-1];
}
 
// return the number of normal Campuses the specified player currently has
int getCampuses (Game g, int player){
	assert(player > 0);
	return g->campuses[player-1];
}
 
// return the number of IP Patents the specified player currently has
int getIPs (Game g, int player){
   assert(player > 0);
   player--; //Start 0 indexing, assumed player is not NO_ONE or 0
   return g->patents[player];   
}
 
// return the number of Publications the specified player currently has
int getPublications (Game g, int player){
   assert(player > 0);
   player--; //Start 0 indexing, assumed player is not NO_ONE or 0
   return g->publications[player];
}
 
// return the number of students of the specified discipline type
// the specified player currently has
int getStudents (Game g, int player, int discipline){
   assert(player > 0);
   player--; //Start 0 indexing, assumed player is not NO_ONE or 0
   return g->students[player][discipline];
}
 
// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one
// student of discipline type disciplineTo.  This will depend
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, int player, int disciplineFrom, int disciplineTo){
	assert(disciplineFrom != STUDENT_THD);
	int exchangeRate = 3;

   //Check if a player has campuses at trading locations
	if (disciplineFrom == STUDENT_MTV) {
		if (hasCampus(g, player, "R") || hasCampus(g, player, "RR")) {
			exchangeRate = 2;
      }
	} else if (disciplineFrom == STUDENT_MMONEY) {
		if (hasCampus (g, player, "LR") || hasCampus (g, player, "LRL")) {
			exchangeRate = 2;
		}
	} else if (disciplineFrom == STUDENT_BQN) {
		if (hasCampus (g, player, "LRRLLRLR") || hasCampus (g, player, "LRRLLRLRR")) {
			exchangeRate = 2;
		}
	} else if (disciplineFrom == STUDENT_MJ) {
		if (hasCampus (g, player, "LRRLLRLRRLRR") || hasCampus (g, player, "LRRLLRLRRLRRL")) {
			exchangeRate = 2;
		}
	} else if (disciplineFrom == STUDENT_BPS) {
		if (hasCampus (g, player, "LRRLLRLRRLRRLRLRRL") || 
			hasCampus (g, player, "LRRLLRLRRLRRLRLRRLR")) {
			exchangeRate = 2;
		}
	}

	return exchangeRate;
}

int hasCampus(Game g, int player, path location){
	int result;
	int campus = getCampus(g, location);
	if(campus == player || campus == player + 3){
		result = TRUE;
	} else {
		result = FALSE;
	}
	return result;
}


int isLegalParamaters(action a) {
   int c = 0;
   int pass = TRUE;
   if (a.actionCode <= 0 && a.actionCode >=7) {
      pass = FALSE;
   }
   if (a.disciplineFrom <= 0 && a.disciplineFrom >= 5) {
      pass = FALSE;
   }
   if (a.disciplineTo <= 0 && a.disciplineTo >= 5) {
      pass = FALSE;
   while (c <= PATH_LIMIT && a.destination[c] != '\0') {
      if(( a.destination[c] != 'L' && a.destination[c] != 'R'
        && a.destination[c] != 'B') && pass == TRUE) {
         pass = FALSE;
      }
      c++;
   }
   return pass;
}

int prevARC(game g, path ARC) {
  int i = 0;
  // Counts the length of the string
  while (i < PATH_LIMIT && (ARC[i] == 'L' ||
           ARC[i] == 'R' || ARC[i] == 'B')){
      i++;
   }
   // May need to change the return to -1 if this doesn't account for
   //  the NULL at the end of the string for some reason.
   // This basically removes the last character from the string so it is
   //  one less than the initial string - I.e. the previous ARC
   ARC[i-2] = NULL;
   return getARC(g, ARC);
}

int insideMap (path toCheck) {
  // Checks if a given path is inside of the map 
  int inside = FALSE;


  return inside;
}

vertex pathTrace(path pathToCheck) {
   // Deploys a mechanical scout to follow a given path. Returns the vertex
   // that it lands on. This is dependant on the pathing mechanics we code.
}