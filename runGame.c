//pseudo code 
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Game.h"


#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}
                

int rollDice(void);
action decideAction(Game g);
int isGameOver(Game g);
int diceValue(void);

int main(){
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    Game g = newGame(disciplines, dice); 
    int diceValue;

    while (!isGameOver(g)) {      
        diceValue = rollDice();     //simulate throw of two dice - we will discuss how to do this in tutorial
                                    // or you may wish to google it and find out how you could do it! 
        diceValue += rollDice();    //roll second dice 
    
        throwDice(g,diceValue);      
    
        action move; //human or mechanicalTurk(AI) decide what they want to do     

        //loop until player action is PASS or player wins     
        while (move.actionCode != PASS) { 
            move = decideAction(g); 

            assert( isLegalAction(g, move) );   //assert that action is legal 

            if (move.actionCode == START_SPINOFF) { 
             //decide if outcome is patent or publication                     
            }         
            makeAction(g, move);             
        } 
    } 

    return EXIT_SUCCESS;
}


int rollDice(void){
    int num;
    printf("What num is the dice?");
    scanf("%d", &num);
    return num;
}

action decideAction(Game g){

    action move;

    int actionCode;
    printf("What is the actioncode");
    scanf("%d", &actionCode);
    move.actionCode = actionCode;

    path destination;
    printf("What is the path");
    scanf("%s", destination);
    strncpy(move.destination, destination, PATH_LIMIT);

    if(actionCode == RETRAIN_STUDENTS) {
        int disciplineFrom;
        printf("Where do you want to move people from?");
        scanf("%d", &disciplineFrom);
        move.disciplineFrom = disciplineFrom;

        int disciplineTo;
        printf("Where do you want to move people to?");
        scanf("%d", &disciplineTo);
    }

    return move;
}

int isGameOver(Game g){

    int isOver = FALSE;
    int playerNum = 1;
    while (playerNum <= NUM_UNIS){
        if(getKPIpoints(g, playerNum)){
            isOver = TRUE;
        }
        playerNum++;
    }

    return isOver;
}

//print statistics 
// free memory