/*
*  Game.c
*  Filled in version of game.h
*/
 

#define NUM_OF_HEXAGONS 19
#define NUM_DISCIPLINES 6
#define NUM_ANGLED_ARCS 44
#define NUM_VERTICAL_ARCS 24

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "Game.h"

void exchangeStudents(Game g, int player, action a, int exchangeRate);
int isLegalParamaters(action a);

typedef struct _game {
    int disciplines[NUM_OF_HEXAGONS];
    int rollNeeded[NUM_OF_HEXAGONS];
    int turnCount;
    int KPIpoints[NUM_UNIS];
    int patents[NUM_UNIS];
    int publications[NUM_UNIS];
    int students[NUM_UNIS][NUM_DISCIPLINES];
    int currentRoll;
    int verticalArcs[NUM_VERTICAL_ARCS];
    int angledArcs[NUM_ANGLED_ARCS];
    int mostPublications;
    int mostARCs;
    int GO8s[NUM_UNIS];
    int arcs[NUM_UNIS];
    int campuses[NUM_UNIS];
} game;
 

Game newGame (int discipline[], int dice[]){

   Game g = malloc(sizeof(game));//Mallocs the game, we gotta find a way to clear this.
   memcpy(g->disciplines, discipline, NUM_OF_HEXAGONS);
   memcpy(g->rollNeeded, dice, NUM_OF_HEXAGONS);

   g->turnCount = -1;
   g->mostPublications = NO_ONE;
   g->mostARCs = NO_ONE;

   return g;
}
 
void disposeGame (Game g){
   free(g);
}
 
void makeAction (Game g, action a){
   assert(isLegalAction (g, a) == TRUE);

   int move = a.actionCode;
   int player = getWhoseTurn(g);
   assert(player > 0);

   if (move == PASS){

   } else if (move == BUILD_CAMPUS){
      //g->destination
      
   } else if (move == BUILD_GO8){
      
   } else if (move == OBTAIN_ARC){
      
   } else if (move == START_SPINOFF){
      
   } else if (move == OBTAIN_ARC){
      
   } else if (move == START_SPINOFF){
      
   } else if (move == OBTAIN_PUBLICATION){
      
   } else if (move == OBTAIN_IP_PATENT){
      
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
int getARC(Game g, path pathToEdge);
 
// returns TRUE if it is legal for the current
// player to make the specified action, FALSE otherwise.
//
// "legal" means everything is legal:
//   * that the action code is a valid action code which is legal to
//     be made at this time
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
/*int isLegalAction (Game g, action a){
   


}*/

 
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

	if (disciplineFrom = STUDENT_MTV){
		if( getCampus(g, "R")  == player || 
		getCampus(g, "RR")
   }
}


int hasCampus(Game g, int player, path location);
	} else if (disciplineFrom == STUDENT_BQN) {
		if (hasCampus (g, player, "LRRLLRLR") ) {
			exchangeRate = 2;
		} else if (hasCampus (g, player, "LRRLLRLRR") ) {
			exchangeRate = 2;
		}
	} else if (disciplineFrom == STUDENT_MJ) {
		if (hasCampus (g, player, "LRRLLRLRRLRR") ) {
			exchangeRate = 2;
		} else if (hasCampus (g, player, "LRRLLRLRRLRRL") ) {
			exchangeRate = 2;
		}
	} else if (disciplineFrom == STUDENT_BPS) {
		if (hasCampus (g, player, "LRRLLRLRRLRRLRLRRL") ){
			exchangeRate = 2;
		} else if (hasCampus (g, player, "LRRLLRLRRLRRLRLRRLR") ) {
			exchangeRate = 2;
		}
	}

	return exchangeRate;
}

int hasCampus(Game g, int player, path location){
	int result;
	int campus getCampus(g, pathlocation);
	if(campus == player || campus == player + 3){
		result = TRUE;
	} else {
		result == FALSE;
	}
	return result;
}
>>>>>>> 5afe961a215a771db4b6eaaf5bd2d5f470c528ff

int isLegalParamaters(action a) {
   int c = 0;
   int pass = FALSE;
   if (a.actionCode >= 0 && a.actionCode <=7) {
      if (a.actionCode == 1)   
         if (a.disciplineFrom >= 0 && a.disciplineFrom <= 5){
            if (a.disciplineTo >= 0 && a.disciplineTo <= 5) {
               while (c <= PATH_LIMIT && a.destination[c] != '\0') {
                  if ((a.destination[c] == 'L' || a.destination[c] == 'R'
                      || a.destination[c] == 'B') && pass == TRUE) {
                     pass = TRUE;
                  } else {
                     pass = FALSE;
                  }
               }
            }
         }
      }
   }
