/*
*  Game.c
*  Filled in version of game.h
*/
 

#define NUM_OF_HEXAGONS 19
#define NUM_PLAYERS 3
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

typedef struct _game {
    int disciplines[NUM_OF_HEXAGONS];
    int rollNeeded[NUM_OF_HEXAGONS];
    int turnCount;
    int KPIpoints[NUM_PLAYERS];
    int patents[NUM_PLAYERS];
    int publications[NUM_PLAYERS];
    int students[NUM_PLAYERS][NUM_DISCIPLINES];
    int currentRoll;
    int verticalArcs[NUM_VERTICAL_ARCS];
    int angledArcs[NUM_ANGLED_ARCS];
} game;
 

Game newGame (int discipline[], int dice[]){

   Game g = malloc(sizeof(game));//Mallocs the game, we gotta find a way to clear this.
   memcpy(g->disciplines, discipline, NUM_OF_HEXAGONS);
   memcpy(g->rollNeeded, dice, NUM_OF_HEXAGONS);
   g->turnCount = -1;
   return g;
}
 
void disposeGame (Game g){
   free(g);
}
 
void makeAction (Game g, action a){
   
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
      while(i <= NUM_PLAYERS){
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
   assert(a.actionCode ==RETRAIN_STUDENTS);
   int to = g->students[player][a.disciplineTo];
   int from = g->students[player][a.disciplineFrom];
   while(to >= exchangeRate){
      from++;
      to -= exchangeRate;
   }
   g->students[player][a.disciplineTo] =  to;
   g->students[player][a.disciplineFrom] = from;
} 
/* **** Functions which GET data about the game aka GETTERS **** */
 
// what type of students are produced by the specified region?
// regionID is the index of the region in the newGame arrays (above)
// see discipline codes above
int getDiscipline (Game g, int regionID){
   return g->disciplines[regionID];
}
 
// what dice value produces students in the specified region?
// 2..12
int getDiceValue (Game g, int regionID){
   return g->rollNeeded[regionID];
}
 
// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game
// has started. 
int getMostARCs (Game g);
 
// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is made.
int getMostPublications (Game g){
	int MostPublications = 0;
	int UniWithMost = 0;
	int i = 1;
	while (i<= NUM_UNIS) {
		int uniPublications = g->publications[i];

		if (uniPublications < MostPublications) {
			MostPublications = uniPublications;
			UniWithMost = i;
		} else if (uniPublications == MostPublications){
			UniWithMost = 0;
		}
		i++;
	}
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
int isLegalAction (Game g, action a);
 
// --- get data about a specified player ---
 
// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, int player);
 
// return the number of ARC grants the specified player currently has
int getARCs (Game g, int player);
 
// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, int player);
 
// return the number of normal Campuses the specified player currently has
int getCampuses (Game g, int player);
 
// return the number of IP Patents the specified player currently has
int getIPs (Game g, int player);
 
// return the number of Publications the specified player currently has
int getPublications (Game g, int player);
 
// return the number of students of the specified discipline type
// the specified player currently has
int getStudents (Game g, int player, int discipline);
 
// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one
// student of discipline type disciplineTo.  This will depend
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, int player,
                    int disciplineFrom, int disciplineTo);

