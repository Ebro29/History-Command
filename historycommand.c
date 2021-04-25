//Include the needed header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

//Declare constant variable
#define MAXIMUMLINE 80

//Structure definition
typedef struct OurNode
{
    //Structure variables
    char *args[MAXIMUMLINE/2];
    int identifier;
    struct OurNode *nextPointer;
    struct OurNode *backPointer;
}
ourhistory;

//Function removeNode()
void removeNode(ourhistory *ournode)
{
    //Initialize
    ourhistory *backPointer = ournode->backPointer;
    ourhistory *nextPointer = ournode->nextPointer;
    backPointer->nextPointer=nextPointer;
    nextPointer->backPointer=backPointer;

    //Remove
    free(ournode);
}

//Function generateNode()
ourhistory* generateNode(int historyCounter, char inputArr[])
{
    //Initialize
    ourhistory *ournode;
    strcpy(ournode->args,inputArr);
    ournode->identifier = historyCounter;

    //Return
    return ournode;
}

//Function insertNode()
void insertNode(ourhistory *Initial, ourhistory *Final, ourhistory *present, int historySize)
{
    //Initialize
    ourhistory *nextToFinal = Final->backPointer;
    present->nextPointer = Final;
    present->backPointer = nextToFinal;
    nextToFinal->nextPointer = present;
    Final->backPointer = present;

    //Loop
    while (nextToFinal->backPointer != NULL)
    {
        //Check condition
        if (memcmp(nextToFinal->args, present->args, MAXIMUMLINE))
        {
            //Function call
            removeNode(nextToFinal);
        }

        //Update
        nextToFinal = nextToFinal->backPointer;
    }

    //Declare and initialize
    int counter = 0;
    ourhistory *ourworkingNode = Initial;

    //Loop
    while (Initial->nextPointer != NULL)
    {
        //Update
        counter++;
        Initial = Initial->nextPointer;
    }
    ourworkingNode = Initial;

    //Check condition
    if (historySize > counter)
    {
        //Declare and initialize
        int toDelete = historySize - counter;
        int i1;

        //Loop
        for (i1 = 0; i1 < toDelete; i1++)
        {
            //Function call
            removeNode(ourworkingNode->nextPointer);
        }
    }
}

//Function setup()
void setup(char inputBuffer[], char *args[],int *background)
{
    //Declare variables
    int len, i1, j1, starter;

    //Initialize
    len = read(STDIN_FILENO, inputBuffer, MAXIMUMLINE);
    j1 = 0;
    starter = -1;

    //Check condition
    if (len == 0)

        //Exit
        exit(0);
    if (len < 0)
    {
        perror("error reading command");

        //Exit
        exit(-1);
    }

    //Loop
    for (i1 = 0; i1 < len; i1++)
    {
        //Switch case
        switch (inputBuffer[i1])
        {
            //Case
            case ' ':
            case '\t' :

                //Check condition
                if(starter != -1)
                {
                    //Update
                    args[j1] = &inputBuffer[starter];
                    j1++;
                }

                //Update
                inputBuffer[i1] = '\0';
                starter = -1;

            //End of case
            break;

            //Case
            case '\n':

                //Check condition
                if (starter != -1)
                {
                    //Update
                    args[j1] = &inputBuffer[starter];
                    j1++;
                }

                //Update
                inputBuffer[i1] = '\0';
                args[j1] = NULL;

            //End of case
            break;

            //Case
            case '&':

                //Update
                *background = 1;
                inputBuffer[i1] = '\0';

            //End of case
            break;

            //Default case
            default :

                //Check condition
                if (starter == -1)

                    //Update
                    starter = i1;
        }
    }

    //Update
    args[j1] = NULL;
}

//Driver
int main(void)
{
    //Declare and initialize
    char inputBuffer[MAXIMUMLINE];
    char *args[MAXIMUMLINE/2+1];
    int background;
    int counter = 1;
    ourhistory *Initial = (ourhistory *)malloc(sizeof(ourhistory));
    ourhistory *Final = (ourhistory *)malloc(sizeof(ourhistory));
    Initial->identifier = 0;
    Initial->nextPointer = Final;
    Final->identifier = -1;
    Final->backPointer = Initial;

    //Loop
    while (1)
    {
        //Initialize
        background = 0;

        //Print
        printf("CSE2431Sh$");

        //Flush
        fflush(0);

        //Function call
        setup(inputBuffer, args, &background);

        //Process id
        pid_t ourpid = fork();

        //Check condition
        if (ourpid < 0)

            //Print
            printf("Fork Failed\n");

        //Check condition
        else if (ourpid == 0)
        {
            //Check condition
            if (strcmp(inputBuffer,"ourhistory") == 0 || strcmp(inputBuffer,"h") == 0)
            {
                //Print
                printf("History:\n");
            }

            //Check condition
            else if (strcmp(inputBuffer,"r") == 0)
            {
                //Initialize
                int ourReturn = execvp("ourhistory",args);

                //Check condition
                if (ourReturn == -1)
                {
                    //Print
                    printf("ourhistory: item not found\n");

                    //Exit
                    exit(-1);
                }

                //Print
                printf("r valid");
            }

            //Check condition
            else if (strcmp(inputBuffer,"rr") == 0)
            {
                //Print
                printf("rr");

                //Initialize
                int ourReturn = execvp("ourhistory",args);

                //Check condition
                if (ourReturn == -1)
                {
                    //Print
                    printf("Invalid entry\n");

                    //Exit
                    exit(-1);
                }
            }

            //Otherwise
            else
            {
                //Initialize
                int ourReturn = execvp(*args, args);

                //Check condition
                if (ourReturn == -1)
                {
                    //Print
                    printf("Invalid entry: %s\n", inputBuffer);

                    //Exit
                    exit(-1);
                }
            }

            //Exit
            exit(0);
        }

        //Otherwise
        else
        {
            //Check condition
            if (background == 0)
            {
                //Wait
                wait();
            }
        }
    }
}

