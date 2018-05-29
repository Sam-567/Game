#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Game.h"


#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}

#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

Game makeDefaultGame(void);
void testTurn(void);
void testCampus(void);

int main (int argc, char *argv[]){

   printf("Starting Tests\n");
   testTurn();
   testCampus();
   printf("\nAll tests passed!\n");

	return EXIT_SUCCESS;
}

//Tests get turn
//Tests throwdice
//Tests get whose turn
//Sam Test
void testTurn(void){

   Game g = makeDefaultGame();
   printf("\nTesting turn count manually\n");
   assert(getTurnNumber(g) == -1);
   assert(getWhoseTurn(g) == 0);
   throwDice(g, 6);
   assert(getTurnNumber(g) == 0);
   assert(getWhoseTurn(g) == 1);
   throwDice(g, 6);
   assert(getTurnNumber(g) == 1);
   assert(getWhoseTurn(g) == 2);
   throwDice(g, 6);
   assert(getTurnNumber(g) == 2);
   assert(getWhoseTurn(g) == 3);
   throwDice(g, 6);
   assert(getTurnNumber(g) == 3);
   assert(getWhoseTurn(g) == 1);
   printf("Done testing manually\n");
   disposeGame(g);

   g = makeDefaultGame();
   printf("\nTesting turn count automatically\n");
   int i = -1;
   while(i < 50){
      assert(getTurnNumber(g) == i);
      assert(getWhoseTurn(g) == (i%3)+1);
      throwDice(g, 6);
      i++;
   }
   printf("Done Testing turn count automatically\n");
   disposeGame(g);
}


//Tests parts of makeAction
//Tests parts of pathing
//Sam - tests has campus and get campus as well as make
void testCampus(void){

   printf ("Testing campus functions");

   Game g = makeDefaultGame();

   action a;
   a.actionCode = OBTAIN_ARC;
   strcpy ("R", a.destination);
   makeAction(g, a);

   strcpy ("RL", a.destination);
   makeAction(g, a);

   a.actionCode = BUILD_CAMPUS;
   makeAction(g, a);

   assert (getCampus (g, "RL") == UNI_A);

   //Only used for our Game.c
   //assert (hasCampus (g, UNI_A, "RL") == TRUE);

   assert (getCampus (g, "RLRL") == NO_ONE);
   assert (getCampus (g, "") == UNI_A);
   assert (getCampuses (g, UNI_A) == 3);
}

//Sam Test
Game makeDefaultGame(void){
   int disciplines[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   Game g = newGame (disciplines, dice);
   assert (g != NULL);
   printf("Made a game");
   return g;
}