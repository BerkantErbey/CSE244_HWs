/*##############################################################################
#   CSE244_HW1_Berkant_Erbey_141044030
#   Berkant Erbey
#   141044030
#   01.03.2017 23.33
#   Find the given string, in given file and write the string's first char's
#   coordinates.
##############################################################################*/


/*#########################		Include Section		##########################*/
#include <stdlib.h>/*exit*/
#include <stdio.h>/*printf,fprintf,fscanf,fopen,fclose,fseek,ftell,*/
#include <string.h>/*strlen*/
/*############################################################################*/

/*##########################	Functions			##########################*/
void findAndPrint(const char* searchingStr,const char* fileName);
/*############################################################################*/


int main(int argc, char const *argv[])
{
    
    if(argc != 3)/*Usage Control*/
    {
        fprintf(stderr,"***Wrong usage for this program***\n");
        fprintf(stderr,"This programs finds the given string in given text file\n");
        fprintf(stderr,"and counts the occurence and coordinates this occurences\n");
        fprintf(stderr,"(strings first character's coordinates).\n");
        fprintf(stderr,"Usage should be like this ./list string searchFile.txt");
    }
    else
    {
        findAndPrint(argv[1],argv[2]);
    }
    return 0;

}
void findAndPrint(const char* searchingStr,const char* fileName)
{
	
	int columnFlag = 0,enterFlag = 0;/*Flag for cursor movement and row col infos*/
	int returnCursor;/*Returning the (last occurence+1) index*/
	int flag = 1;/*LoopControlVariable*/
    int occRow = 1,occCol = 1;/*Occurence(for first letter) Indexes*/
    int currRow = 1,currCol = 1;/*Current Indexes*/
	int loopVar = 1;/*LoopVariable*/
	char readedChar;/*One character readed in text file*/
	int searchingStrSize,stringCounter = 0;

	FILE *inFilePtr;
	inFilePtr = fopen(fileName,"r");

	if(inFilePtr == NULL)/*File opening Control*/
    {
        fprintf(stderr,"***Error occurred in opening file***");
        exit(1);
    }
    else
    {
		searchingStrSize = (int)strlen(searchingStr);

		while(fscanf(inFilePtr,"%c",&readedChar) != EOF)/*EndOfFile Control*/
		{
			if(readedChar == '\n')
			{
				++currRow;
				currCol = 1;/*New Line's first column*/
			}
			else if(readedChar == '\t' || readedChar == ' ' || readedChar != searchingStr[0])
					++currCol;
			else if(readedChar == searchingStr[0])/*searchingStr's first char found*/
			{
				returnCursor = ftell(inFilePtr);
				occRow = currRow;/*If first char found then current coordinates assigned to occurrence coordinates*/
				occCol = currCol;
				++currCol;
				do
				{
					if(fscanf(inFilePtr,"%c",&readedChar)!= EOF)/*EndOfFile Control*/
					{
						if(searchingStrSize == loopVar)/*searchingString Found*/
						{
							++stringCounter;
							printf("[%d,%d] contains first letter of %s\n",occRow,occCol,searchingStr);
							fseek(inFilePtr,returnCursor,SEEK_SET);/*After str found return the last occurence's position's one move forward*/
							flag = 0;
						}
						else if(readedChar == searchingStr[loopVar])
						{
							flag = 1;
							++columnFlag;
							++currCol;
							++loopVar;
						}
						else if(readedChar == '\t' || readedChar == ' ')
						{
							flag = 1;
							++columnFlag;
							++currCol;
						}
						else if(readedChar == '\n')
						{
							flag = 1;
							++enterFlag;
							++currRow;
							currCol = 1;
						}
						else if(1)/*Reads the one character but not contain the string's characters*/
						{/*End Of Loop Condition*/
							flag = 0;
							++columnFlag;
							++currCol;
							fseek(inFilePtr,returnCursor,SEEK_SET);/*After str found return the last occurence's position's one move forward*/
						}
					}
					else
						flag = 0;
				}while(flag);
				/*Initializes the flags and other variables after the loop*/
				currCol -= columnFlag;
				currRow -= enterFlag;
				columnFlag = 0;
				enterFlag = 0;
				loopVar = 1;
			}
		}
	}
	fclose(inFilePtr);

	if(stringCounter > 0)/*String founded in text*/
	{
		printf("-------------------------------------------------\n");
		printf("%s found %d times in %s\n",searchingStr,stringCounter,fileName);
	}
	else
		fprintf(stderr,"%s not found in %s\n",searchingStr,fileName);

	return;

}
/*##############################################################################
                                END OF HW1
##############################################################################*/
