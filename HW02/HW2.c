/*##############################################################################
#   CSE244_HW2_Berkant_Erbey_141044030
#   Berkant Erbey
#   141044030
#   07.03.2017
#   Find the given string, in given directory and sub-directories
#	and write the (log file) string's first char's coordinates.
##############################################################################*/

/*#########################		Include Section		##########################*/
#include <stdlib.h>/*exit*/
#include <stdio.h>/*printf,fprintf,fscanf,fopen,fclose,fseek,ftell,*/
#include <string.h>/*strlen*/
#include <dirent.h>/*Directory Functions*/
#include <unistd.h>
#include <time.h>/*wait*/
#include <sys/stat.h>
#include <sys/param.h>/*pid_t*/
/*############################################################################*/

/*#########################		Define Section		##########################*/
#define MAX_PATH_LENGHT 4095	/*For Ubuntu 14.04 64-bit default(4095)*/
#define LOG_FILE "log.txt"		/*For storing result in log.txt*/
/*############################################################################*/

/*##########################	Functions			##########################*/
void findAndPrint(const char* searchingStr,const char* fileName);
void dirSearch(const char* searchingStr,const char* dirPath);
int isDirectory(const char *dirPath);/*Reference:Unix System Programming page189*/


/*############################################################################*/


int main(int argc, char const *argv[])
{

    if(argc != 3)/*Usage Control*/
    {
        fprintf(stderr,"Usage should be like this ./exe ece folderName");
    }
    else
    {
		dirSearch(argv[1],argv[2]);
        /*findAndPrint(argv[1],argv[2]); dirSearch in icine koy*/
		
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
	FILE *outFilePtr;

	inFilePtr = fopen(fileName,"r");
	/*printf("%s\n",fileName);*/
	outFilePtr = fopen(LOG_FILE,"a+");
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
			/*fprintf(stderr,"%c\n",readedChar);*/
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
							fprintf(outFilePtr,"%s: [%d,%d] %s first character is found.\n",fileName,occRow,occCol,searchingStr);
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
	/*out kapa*/
	fclose(outFilePtr);

	return;
}
void dirSearch(const char* searchingStr,const char* dirPath)
{
	char tempDirPath[MAX_PATH_LENGHT];/*For storing directory's path*/
	DIR *inputDirPtr;/*InputDirectoryPointer*/
	struct dirent *dirStr;/*Directory Structure*/
	pid_t pidDirectory,pidFile;/*For Two Fork(File and Directory)*/
	FILE *outFilePtr;/*For log file*/

	if((inputDirPtr = opendir(dirPath)) == NULL)/*Directory Opening Control*/
	{
		fprintf(stderr,"Directory %s  can't opening",dirPath);
		exit(1);
	}
	else
	{
		strcpy(tempDirPath,dirPath);
		/*printf("1->%s\n",tempDirPath);*/
		while((dirStr = readdir(inputDirPtr)) != NULL)
		{
			/*perror("while");*/
			/*fprintf(stderr, "%s\n", dirStr->d_name);*/
			if(strcmp(dirStr->d_name,".") != 0  && strcmp(dirStr->d_name,"..") != 0)
			{/*If it is a directory(which is not current dir and upper directory)*/
				sprintf(tempDirPath,"%s/%s",dirPath,dirStr->d_name);
				/*printf("2->%s\n",tempDirPath);*/

				if(isDirectory(tempDirPath))
				{
					pidDirectory = fork();/*FORK*/
					if(pidDirectory == -1)/*Fork Error*/
					{
						fprintf(stderr, "***Fork cant done properly***\n");
						exit(1);
					}
					if(pidDirectory == 0)/*Child Process*/
					{
						dirSearch(searchingStr,tempDirPath);/*Recursive Call*/
						exit(1);
					}
				}
				else
				{
					pidFile = fork();/*FORK*/
					if(pidFile == -1)/*Fork Error*/
					{
						fprintf(stderr, "***Fork cant done properly***\n");
						exit(1);
					}
					if(pidFile == 0)/*If pid=0 this is a child process*/
					{
						findAndPrint(searchingStr,tempDirPath);
						exit(0);
					}
				}
				/*perror("wait");*/
				/*wait(NULL);*/
			}
		}
		wait(NULL);
	}
	return;
}
int isDirectory(const char *dirPath)
{
	struct stat statbuf;

	if(stat(dirPath,&statbuf) == -1)
		return 0;
	else
		return S_ISDIR(statbuf.st_mode);
}

/*##############################################################################
                                END OF HW2
##############################################################################*/
