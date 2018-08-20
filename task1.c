#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 10000
typedef struct
{
    int indexCurent;
    char cells[MAX+1];
} Tape;

typedef struct
{
    char nameState[100];

} State;

typedef struct
{
    State currentState;
    State nextState;
    char readSymbolTape1;
    char readSymbolTape2;
    char writeSymbolTape1;
    char writeSymbolTape2;
    char directionTape1;
    char directionTape2;

} Transition;

typedef struct
{
    int noStates;
    int noFinaleStates;
    State *finaleStates;
    State *firstState;
    State *states;
    Tape *tape1;
    Tape *tape2;
    int noTransitions;
    Transition *transitions;

} TuringMachineWithTwoTapes;

int readStates(TuringMachineWithTwoTapes *MT, FILE *fp)
{
	int i;
	fscanf(fp, "%d", &MT->noStates);
    MT->states = (State*) malloc(sizeof(State) * MT->noStates);

    if (MT->states == NULL)
    {
    	fprintf(stderr, "Error\n");
    	return -1;
    }

    for (i = 0; i < MT->noStates; i++)
    {
        fscanf(fp, "%s", MT->states[i].nameState);
    }

    fscanf(fp, "%d", &MT->noFinaleStates);
    MT->finaleStates = (State* ) malloc(sizeof(State) * MT->noFinaleStates);

    for (i = 0; i < MT->noFinaleStates; i++)
    {
        fscanf(fp, "%s", MT->finaleStates[i].nameState);
    }

    MT->firstState = (State*) malloc(sizeof(char)*3);

    if (MT->firstState == NULL)
    {
    	fprintf(stderr, "Error\n");
    	return -1;
    }

    fscanf(fp, "%s", MT->firstState->nameState);
    return 0;
}

int readTransitions(TuringMachineWithTwoTapes *MT, FILE *fp)
{

	int i;

	fscanf(fp, "%d", &MT->noTransitions);
    MT->transitions = (Transition*) malloc(sizeof(Transition) * MT->noTransitions);

    if (MT->transitions == NULL)
    {
    	fprintf(stderr, "Error\n");
    	return -1;
    }

	for (i = 0; i < MT->noTransitions; i++)
    {
        fscanf(fp, "%s %c %c %s %c %c %c %c", MT->transitions[i].currentState.nameState, 
        	&(MT->transitions[i].readSymbolTape1),&(MT->transitions[i].readSymbolTape2), 
        	MT->transitions[i].nextState.nameState,&(MT->transitions[i].writeSymbolTape1),
            &(MT->transitions[i].directionTape1), &(MT->transitions[i].writeSymbolTape2), 
            &(MT->transitions[i].directionTape2));
    }
    return 0;
}

int readTapes(TuringMachineWithTwoTapes *MT, FILE *fp)
{	

    int i;

    MT->tape1 = (Tape*) malloc(sizeof(Tape));
    MT->tape2 = (Tape*) malloc(sizeof(Tape));

    if (MT->tape1 == NULL || MT->tape2 == NULL)
    {
        fprintf(stderr, "Error\n");
        return -1;
    }

    MT->tape1->indexCurent = 1;
    MT->tape2->indexCurent = 1;
    fscanf(fp, "%s", MT->tape1->cells);
    fscanf(fp, "%s", MT->tape2->cells);

    for (i = strlen(MT->tape1->cells) - 1; i < MAX; i++)
    {
        MT->tape1->cells[i] = '#';
    }
    MT->tape1->cells[MAX+1] = '\0';

    for (i = strlen(MT->tape2->cells) - 1; i < MAX; i++)
    {
        MT->tape2->cells[i] = '#';
    }
    MT->tape2->cells[MAX+1] = '\0';

    return 0;
}

TuringMachineWithTwoTapes* createMachine(FILE *fp, FILE *out)
{

    TuringMachineWithTwoTapes* MT = (TuringMachineWithTwoTapes*) malloc(sizeof(TuringMachineWithTwoTapes));
    
    if (MT == NULL)
    {
    	fprintf(stderr, "Error\n");
    	return NULL;
    }
    readStates(MT, fp);
    readTransitions(MT, fp);
    readTapes(MT, fp);

    return MT;
}

int checkStateIsFinale(State *finaleStates, State* currentState, int n)
{
	int j;
	for (j = 0; j < n; j++)
        {
            if (strcmp(currentState->nameState, finaleStates[j].nameState) == 0)
            {
                return 0;
            }
        }
    return 1;
}

void printTapes(TuringMachineWithTwoTapes *MT, FILE *out)
{
	
    fprintf(out, "%s", MT->tape1->cells);
  

    fprintf(out, "\n");

   
    fprintf(out, "%s", MT->tape2->cells);

    fprintf(out, "\n");
}

int checkStatesAreTheSame(TuringMachineWithTwoTapes MT, int index, State currentState)
{
	if (strcmp(MT.transitions[index].currentState.nameState, currentState.nameState) == 0 &&
		MT.tape1->cells[MT.tape1->indexCurent] == MT.transitions[index].readSymbolTape1 && 
        MT.tape2->cells[MT.tape2->indexCurent] == MT.transitions[index].readSymbolTape2)
    {
    	return 0;
    }

    else
    {
    	return 1;
    }	
}

void writeSymbolsOnTapes(TuringMachineWithTwoTapes *MT, int index)
{
	MT->tape1->cells[MT->tape1->indexCurent] = MT->transitions[index].writeSymbolTape1;
    MT->tape2->cells[MT->tape2->indexCurent] = MT->transitions[index].writeSymbolTape2;
}

void moveHeads(TuringMachineWithTwoTapes*MT, int index)
{
	 	if (MT->transitions[index].directionTape1 == 'R')
		{
	     	MT->tape1->indexCurent += 1;
	 	}

	 	else if (MT->transitions[index].directionTape1 == 'L')
	 	{
	     	MT->tape1->indexCurent -= 1;
	 	}

	
	 	if (MT->transitions[index].directionTape2 == 'R')
	 	{
	     	MT->tape2->indexCurent += 1;
	 	}

	 
	 	else if (MT->transitions[index].directionTape1 == 'L')
	 	{
	 		MT->tape2->indexCurent -= 1;
	 	}
}

void runMachineWithTwoTapes()
{
	FILE *fp, *out;
	fp = fopen("task1.in", "r");
    out = fopen("task1.out", "w");

    int i, success = 1;
	TuringMachineWithTwoTapes *MT = createMachine(fp, out);
	
	if (MT == NULL)
	{
		fprintf(stderr, "Error\n");
		return;
	} 
    State *currentState = (State*) malloc(sizeof(State));

    if (currentState == NULL)
    {
    	fprintf(stderr, "Error\n");
    	return;
    }

    strcpy(currentState->nameState, MT->firstState->nameState);

    while (checkStateIsFinale(MT->finaleStates, currentState, MT->noFinaleStates) == 1)
    {
    	//printf("%s\n",currentState->nameState);
    	for (i = 0; i < MT->noTransitions; i++)
    	{
        	if(checkStatesAreTheSame(*MT, i, *currentState) == 0)
        	{	
        		writeSymbolsOnTapes(MT, i);
            	strcpy(currentState->nameState, MT->transitions[i].nextState.nameState);
            	moveHeads(MT, i);
            	success = 1;
            	break;
        	}

        	else
        	{
        		success = 0;
        	}
        }		

        if(success == 0)
        {   
        	fprintf(out, "The machine has blocked!\n");
            return;
        }   	  
    }

  	
    printTapes(MT, out);
    fclose(fp);
    fclose(out);

    free(currentState);
    MT = NULL;
}

int main()
{
    runMachineWithTwoTapes();
    return 0;
}
