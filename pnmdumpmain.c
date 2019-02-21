//Leave 3 lines between each section; makes things MUCH neater

//NB: Use exit() to test breakpoints!


/// Libraries

#include <stdio.h>
#include <string.h> // deals with strings stored in character's arrays. eg: strlen, strcmp
#include <stdlib.h> //used for exiting with error code & atoi/itoa
#include <math.h>


/// Global Vars

const double valVersion = 1; //sets version value; at current version, set to 1
//changes version if necessary



/// Functions

void usagePrint(FILE *channel);
void runHexDump(FILE *fileReader); //hex values
int printCorrupt(FILE* fileInput, FILE* fileOutput);
int convertFileMain(FILE *fileInput, FILE *fileOutput, char* convertType); //conversion of P2 to P5
int convertFileP2toP5(FILE *fileInput, FILE *fileOutput, unsigned char rowPicture[512], char *tokenCurrent, int countRow, int countColumn, int valColor, int valWidth, int valHeight);
int convertFileP5toP2(FILE *fileInput, FILE *fileOutput, int valWidth);
int convertFileRotate(char imageTypeMain[3], FILE *fileInput, FILE *fileOutput, unsigned char rowPicture[2048], char *tokenCurrent, int countRow, int countColumn, int valColor, int valWidth, int valHeight, const char* rotateType);
int convertFilePosterize(char imageTypeMain[3], FILE *fileInput, FILE *fileOutput, unsigned char rowPicture[2048], char *tokenCurrent, int countRow, int countColumn, int valColor, int valWidth, int valHeight);

/// Main

int main(int argc, char *argv[])
{   //if number of args is 2, check whether arguments are usage, version or hexdump (stdin)
    if (argc == 2)
    {
        if ((strcmp(argv[1], "--usage")) == 0)  //checks whether argument is "--usage"
            usagePrint(stdout); //prints usage
        else if ((strcmp(argv[1], "--version")) == 0) //checks whether argument is "--version"
            fprintf(stdout, "%.01f\n", valVersion); //prints version of program
        else if ((strcmp(argv[1], "--hexdump")) == 0) //checks whether argument is "--hexdump"
            runHexDump(stdin);//runs hexdump with stdin stream
    }

    //if number of args is 3, check if it is for hexdump (file)
    else if ((argc == 3) && (strcmp(argv[1], "--hexdump") == 0))
    {
        FILE* fileInput = fopen(argv[2], "rb"); //reads file & check if it exists

        if(fileInput == NULL)	{//if file does not exist, return error
            fprintf(stderr, "No such file: \"%s\"\n", argv[2]); //returns error formatted to specs
            return 1;
        }//exits with code 1
        else
            runHexDump(fileInput);//should it exist, runs hexdump with file stream
    }

    //checks if there are 3 arguments and if the 2nd argument is either for conversion, rotation, or posterization function
    else if ((argc >= 4) && (strcmp(argv[1], "--P2toP5") == 0 || strcmp(argv[1], "--P5toP2") == 0 || strcmp(argv[1], "--rotate") == 0 || strcmp(argv[1], "--rotate90") == 0 || strcmp(argv[1], "--posterize") == 0))
    {
        //reads binary file with dir specified in 2nd argument
        FILE* fileInput = fopen(argv[2], "rb");
        //writes binary file to dir specified in 3rd argument
        FILE* fileOutput = fopen(argv[3], "wb");

        if((fileInput == NULL) || (fileOutput == NULL))
        {   // null has zero value. so it is checking whether the file is empty or not
            fprintf(stderr, "No such file: \"%s\"\n", argv[2]); //returns error formatted to specs
            return 1;
        } //exits with code 1
        else
        {
            int isSuccess = 0; //defines return code to be returned by function
            if (strcmp(argv[1], "--P2toP5") == 0)
                isSuccess = convertFileMain(fileInput, fileOutput, "P2toP5");//If file exists and is readable, run P2toP5
            else if (strcmp(argv[1], "--P5toP2") == 0)
                isSuccess = convertFileMain(fileInput, fileOutput, "P5toP2"); //runs P5toP2
            else if (strcmp(argv[1], "--rotate") == 0)
                isSuccess = convertFileMain(fileInput, fileOutput, "rotate"); //runs file rotation (flip?)
            else if (strcmp(argv[1], "--rotate90") == 0)
                isSuccess = convertFileMain(fileInput, fileOutput, "rotate90"); //runs file ROTATION by 90 degrees
            else if (strcmp(argv[1], "--posterize") == 0)
                isSuccess = convertFileMain(fileInput, fileOutput, "posterize"); //runs picture posterization
            else
            {//returns error message if command is nonexistent
                fprintf(stderr, "pnmdump: bad arguments\n"); //returns error message
                usagePrint(stderr);
                return 1;
            }
            return isSuccess;
        } //exit with error code returned by function
    }
    else
    {
        fprintf(stderr, "pnmdump: bad arguments\n"); //returns error message
        usagePrint(stderr);
        return 1;
    }
    return 0; // end of main
}



void usagePrint(FILE *channel) /*Only prints instructions, therefore there is no need to return any value therefore we use VOID instead of any other data type*/
{
    fprintf(channel, "Usage:\n");
    fprintf(channel, "./pnmdump.exe --version\n");
    fprintf(channel, "./pnmdump.exe --usage\n");
    fprintf(channel, "./pnmdump.exe --hexdump [FILE]\n");
}



void runHexDump(FILE *fileReader) //takes from called original dump
{
    int bytesRead = 0; //indicates the size of blocks to read
    unsigned char chunk[8]; //allocates size of blocks to read, in chunks of 8 bytes
    int totalSize = 0; //Counts total size of file

    if (fileReader == NULL)
        fprintf(stderr, "Enter valid input\n"); //returns error message if file doesn't exist
    else
    {
        for(;;)	//loops until reader reads EOF
        {
            bytesRead = fread(chunk, 1, 8, fileReader); // chunks of 8 bytes
            if ((bytesRead < 8) && (bytesRead <= 0)) //if chunk read at end has less than 8 bytes, terminate
                break;
            fprintf(stdout, "%07x", totalSize); //prints current size
            totalSize = totalSize + bytesRead; //increments to remainder if incomplete
            //bytesRead is either = 8 or < 8

            for(int count=0; count < bytesRead; count++)
            {   //checks if character in chunk is valid
                fprintf(stdout, "  ");
                fprintf(stdout, "%02X ", chunk[count]); //prints hex value of char
                if (((int)chunk[count] >= 32) && ((int)chunk[count] <= 126)) //invalid character replaced with '.'
                    putc(chunk[count], stdout); //prints char
                else
                    putc('.', stdout);
            }

            fprintf(stdout, "\n");
			//if bytes being read is less than 8, terminate since read final remaining characters
            if (bytesRead < 8)
                break;
        }
        fprintf(stdout, "%07x\n", totalSize);
        fclose(fileReader);//close IO reader
    }
}
//combined runHexDump() and runHexStdin()




int printCorrupt(FILE* fileInput, FILE* fileOutput)
{   //prints text used to signify corrupted files
    fprintf(stderr, "Corrupted input file\n");
    fclose(fileInput);
    fclose(fileOutput);
    return 1; //returns 1 to calling function, which then returns 1 to main()
}




int convertFileMain(FILE *fileInput, FILE *fileOutput, char* convertType) //takes from called original dump
{
///Line 1: P2 vs P5

    //reads first line in file (eg P2, P5), and compares it accordingly
    char imageTypeBuffer[3], imageTypeMain[3], lineTemp[512]; //variable storing obtained image type
    //lineTemp simply allows fgets() to read an entire line until /n and skip it

    //This is stored as 3 to allow for \0

    fgets(imageTypeBuffer, sizeof(imageTypeBuffer), fileInput);
    sscanf(imageTypeBuffer, "%s", imageTypeMain);

    /*checks, according to conversion type, if file header/type matches intended input type
    if type requires P2 but file isn't P2, or requires P5 but file isn't P5*/

    if ((strcmp(convertType, "P2toP5") == 0) && (strcmp(imageTypeMain, "P2") == 0))
        fputs("P5\n", fileOutput);
    else if ((strcmp(convertType, "P5toP2") == 0) && (strcmp(imageTypeMain, "P5") == 0))
        fputs("P2\n", fileOutput);
    else if (((strcmp(convertType, "rotate") == 0) || (strcmp(convertType, "rotate90") == 0) || (strcmp(convertType, "posterize") == 0)) && (strcmp(imageTypeMain, "P2") == 0 || strcmp(imageTypeMain, "P5") == 0))
        fprintf(fileOutput, "%s\n", imageTypeMain);
    else
    {   //if files are not in either P2 or P5, and doesn't correspond to intended function
        //thus, returns error message
        int charTypeCurrent; //stores either 2 or 5
        if (strcmp(convertType, "P2toP5") == 0)
            charTypeCurrent = 2;
        else
            charTypeCurrent = 5;
        //returns error code, where main() returns with same value
        fprintf(stderr, "Input is not in P%d format\n", charTypeCurrent);
        return 1;
    }//we don't need to worry about the section here, since if it's erroneous, it would've exited already
    fgets(lineTemp, 512, fileInput); //'jumps' over line

	
///Line 2: Comments


    fprintf(fileOutput, "# Generated by pnmdump.exe\n");//writes arbitrary comment to file
    fgets(lineTemp, 512, fileInput); //'jumps' over comment line


///Line 3: Width & Height

    char charWidth[128], charHeight[128]; //defines strings to read width & height from file
    int valWidth, valHeight; //defines width & height of image to obtain
    char charBuffer[512]; //defines buffer to fetch line

    fgets(charBuffer, sizeof(charBuffer), fileInput); //obtains line string of width & height
    sscanf(charBuffer, "%s %s\n", charWidth, charHeight); //sets values according to format of string

    valWidth = atoi(charWidth); //sets height to as-is integer taken
    valHeight = atoi(charHeight); //sets width to as-is integer taken

    //since rotation causes rows & columns to swap, one also swaps the values written in the file
    if ((strcmp(convertType, "rotate") == 0) || (strcmp(convertType, "rotate90") == 0))
        fprintf(fileOutput, "%d %d\n", valHeight, valWidth);
    else //otherwise, if it is a conversion, simply write the same values
    {
        fprintf(fileOutput, "%d %d\n", valWidth, valHeight);
    }


///Line 4: Color Depth


    int valColor; //defines color depth of image

    fgets(charBuffer, 512, fileInput);	//Retrieves line containing color depth & stores it in buffer
    valColor = atoi(charBuffer); //sets color depth to as-is number obtained from read string
    fprintf(fileOutput, "%d\n", valColor); //prints color depth to new line in file


///Line 5 onwards: Content


    unsigned char rowPicture[2048]; //str containing chars of a single image row
    int countRow = 0, countColumn = 0; //count current row
    //This is where P2toP5, P5toP2, and other conditions branch.
    int convertReturn = 0; //defines whether conversion is successful
    char *tokenCurrent = NULL; //token for reading, used in specific functions, specified below

    //according to user input, runs functions corresponding to such
    if (strcmp(convertType, "P2toP5") == 0)
        convertReturn = convertFileP2toP5(fileInput, fileOutput, rowPicture, tokenCurrent, countRow, countColumn, valColor, valWidth, valHeight);

    if (strcmp(convertType, "P5toP2") == 0)
        convertReturn = convertFileP5toP2(fileInput, fileOutput, valWidth);

    if (strcmp(convertType, "rotate") == 0)
        convertReturn = convertFileRotate(imageTypeMain, fileInput, fileOutput, rowPicture, tokenCurrent, countRow, countColumn, valColor, valWidth, valHeight, "rotate");

    if (strcmp(convertType, "rotate90") == 0)
        convertReturn = convertFileRotate(imageTypeMain, fileInput, fileOutput, rowPicture, tokenCurrent, countRow, countColumn, valColor, valWidth, valHeight, "rotate90");

    if (strcmp(convertType, "posterize") == 0)
        convertReturn = convertFilePosterize(imageTypeMain, fileInput, fileOutput, rowPicture, tokenCurrent, countRow, countColumn, valColor, valWidth, valHeight);

    //when functions are done, close readers/writers and return either 0 or 1 (depending on error status)
    fclose(fileInput);
    fclose(fileOutput);
    return convertReturn;
}



int convertFileP2toP5(FILE *fileInput, FILE *fileOutput, unsigned char rowPicture[512], char *tokenCurrent, int countRow, int countColumn, int valColor, int valWidth, int valHeight)
{
    //Loops through all remaining lines & check the P2 formatting. Currently, this places everything into an array; makes things more organized!
    while ((char *)fgets(((char *)rowPicture), 512, fileInput) != NULL)
    {   //iterates through rows
        //exits when no more chars can be read
        //error if number of rows exceeds given height WHILE there is still text to be read

        if (countRow >= valHeight) //if number of rows obtained are greater or equal to obtained height, yet has data to be read, that means file is corrupt
        {
            int errorReturn = printCorrupt(fileInput, fileOutput);
            return errorReturn;
        }

        tokenCurrent = strtok((char *)rowPicture, " "); //finds first character & separates it by SPACE
        //points to first character in string & stops at space
        while(((const char*)tokenCurrent) != NULL)
        {   //iterates through columns
            //exits if pixel's depth exceeds given color depth, or if the number of columns reaches the width given
            if ((atoi(tokenCurrent) > valColor) || (countColumn >= valWidth))
            {
                int errorReturn = printCorrupt(fileInput, fileOutput);
                return errorReturn;
            }

            fprintf(fileOutput, "%c", (unsigned char)(int)atoi(tokenCurrent));
            tokenCurrent = strtok(NULL, " ");
            countColumn++;
        }
        //when finished, return error if total column count is less than width available
        //this means that a row is not completely filled; this is not wanted!
        if (countColumn < valWidth)
        {
            int errorReturn = printCorrupt(fileInput, fileOutput);
            return errorReturn;
        }
        countColumn = 0; //resets column to position 0
        countRow++;
    }

    if (countRow < valHeight)
    {   //reject if number of rows don't correspond with value given
        int errorReturn = printCorrupt(fileInput, fileOutput);
        return errorReturn;
    }
    return 0;
}



int convertFileP5toP2(FILE *fileInput, FILE *fileOutput, int valWidth)
{
    /*Implemented almost directly from hexDump!*/
	/*Since P5 is a single line of adjacent characters, simply take line & read each character one by one*/
    int bytesRead; //indicates the size of blocks to read
    unsigned char chunk[valWidth]; //allocates size of blocks to read, in chunks of 8 bytes

    for(;;) //writes characters to file
    {
        bytesRead = fread(chunk, 1, valWidth, fileInput); // chunks of 8 bytes

        if (bytesRead < valWidth) //if chunk read at end has less than 8 bytes, terminate
        {
            if (bytesRead == 0)
                break;
            else
            {   //if bytes read is not zero (nothing else to read) and is less than width (has spaces missing)
                int errorReturn = printCorrupt(fileInput, fileOutput);
                return errorReturn;
            }
        }

        for(int count=0; count < bytesRead; count++)
        {   //checks if character in chunk is valid
            fprintf(fileOutput, "%d", chunk[count]); //prints int value

            if (bytesRead < valWidth)
                break;
            if (count < (valWidth - 1))
                fprintf(fileOutput, " ");
            else
                fprintf(fileOutput, "\n");
        }
        //is error if count is less than given width, since that means not everything's filled
    }
    return 0;
}



//180310: There was a bug that freezes/returns a bug when running the baboob file for a rowPicture & its fgets() of 512 & 1024
//This is, simply, due to the memory allocation not compensating for spaces between characters. This is now fixed.
int convertFileRotate(char imageTypeMain[3], FILE *fileInput, FILE *fileOutput, unsigned char rowPicture[2048], char *tokenCurrent, int countRow, int countColumn, int valColor, int valWidth, int valHeight, const char* rotateType)
{
    int imagePictureInt[512][512]; //defines image

    /*Rotation by 90 degrees is flipping image vertically, then flipping by y=x axis*/

    //this section simply sets characters & integers to respective array
    if (strcmp(imageTypeMain, "P2") == 0)
    {
        //Loops through all remaining lines & check the P2 formatting; currently, this places everything into an array; makes things more organized!
        while ((char *)fgets(((char *)rowPicture), 2048, fileInput) != NULL)
        {   //iterates through rows & exits when no more chars can be read
            //error if number of rows exceeds given height WHILE there is still text to be read
            if (countRow >= valHeight)
            {
                int errorReturn = printCorrupt(fileInput, fileOutput);
                return errorReturn;
            }

            tokenCurrent = strtok((char *)rowPicture, " "); //finds first character & separates it by SPACE
            //points to first character in string & stops at space

            while(((const unsigned char*)tokenCurrent) != NULL)	{ //iterates through columns

                //exits if pixel's depth exceeds given color depth, or if the number of columns reaches the width given
                if ((atoi(tokenCurrent) > valColor) || (countColumn >= valWidth))
                {
                    int errorReturn = printCorrupt(fileInput, fileOutput);
                    return errorReturn;
                }

                if (strcmp(rotateType, "rotate90") == 0) //if rotation, set array/image to vertically flip
                    imagePictureInt[(valHeight - 1) - countRow][countColumn] = (int)atoi(tokenCurrent);
                else //if just 'rotating' normally, just set i to j and vice-versa
                    imagePictureInt[countRow][countColumn] = (int)atoi(tokenCurrent);

                tokenCurrent = strtok(NULL, " ");
                countColumn++;
            }

            //when finished, return error if total column count is less than width available; this means that a row is not completely filled; this is not wanted!
            if (countColumn < valWidth)
            {
                int errorReturn = printCorrupt(fileInput, fileOutput);
                return errorReturn;
            }
            countColumn = 0; //resets column to position 0
            countRow++;
        }
    }
    else //if imageTypeMain is P5, read in a similar way to P5toP2
    {
		/*Since P5 is a single line of adjacent characters, simply take line & read each character one by one*/
        int bytesRead;
        unsigned char chunkChar[valWidth];
        for(;;)
        {
            bytesRead = fread(chunkChar, 1, valWidth, fileInput);
            if(bytesRead < valWidth)
                break;

            for (countColumn = 0; countColumn < valWidth; countColumn++)
                if (strcmp(rotateType, "rotate90") == 0) //if rotation type is rotate90, store it as vertically flipped
                    imagePictureInt[(valHeight - 1) - countRow][countColumn] = (int)((int)chunkChar[countColumn]);
                else //otherwise, store it normally
                    imagePictureInt[countRow][countColumn] = (int)chunkChar[countColumn];
            countRow++;
        }
    }

    int rotatedWidth = valHeight; //defines height & width of the rotated image
    int rotatedHeight = valWidth;
    //since one flips the image, one sets valHeight to width, and vice versa

    //this prints out the chars stored in array to its respective positions
    //since it flips by y=x, one uses original array's coordinates & swaps X and Y
	//Therefore program loops through array, instead of each column in row, from row in column
    for (int tempCountColumn = 0; tempCountColumn < rotatedHeight; tempCountColumn++)
    {
        for (int tempCountRow = 0; tempCountRow < rotatedWidth; tempCountRow++)
        {
            if (strcmp(imageTypeMain, "P5") == 0) //for P5, simply print char by char, from array
                fprintf(fileOutput, "%c", (unsigned char)imagePictureInt[tempCountRow][tempCountColumn]);

            else //for P2, print by array & cut to newline when needed
            {
                fprintf(fileOutput, "%d", imagePictureInt[tempCountRow][tempCountColumn]);
                if (tempCountRow < (rotatedWidth - 1))
                    fprintf(fileOutput, " "); //prints out space in characters that are not at end width
                else
                    fprintf(fileOutput, "\n"); //prints out newline at final character of row
            }
        }
    }
    return 0;
}


//OPTIONAL TASK: posterizes image
int convertFilePosterize(char imageTypeMain[3], FILE *fileInput, FILE *fileOutput, unsigned char rowPicture[2048], char *tokenCurrent, int countRow, int countColumn, int valColor, int valWidth, int valHeight)
{
    int imagePictureInt[512][512]; //defines image

    if (strcmp(imageTypeMain, "P2") == 0)
    {
        //Loops through all remaining lines & check the P2 formatting; currently, this places everything into an array; makes things more organized!
        while ((char *)fgets(((char *)rowPicture), 2048, fileInput) != NULL)
        {   //iterates through rows & exits when no more chars can be read
            //error if number of rows exceeds given height WHILE there is still text to be read
            if (countRow >= valHeight)
            {
                int errorReturn = printCorrupt(fileInput, fileOutput);
                return errorReturn;
            }

            tokenCurrent = strtok((char *)rowPicture, " "); //finds first character & separates it by SPACE
            //points to first character in string & stops at space

            while(((const unsigned char*)tokenCurrent) != NULL)	{ //iterates through columns

                //exits if pixel's depth exceeds given color depth, or if the number of columns reaches the width given
                if ((atoi(tokenCurrent) > valColor) || (countColumn >= valWidth))
                {
                    int errorReturn = printCorrupt(fileInput, fileOutput);
                    return errorReturn;
                }

                /*posterizes image by size
                eg if color is bigger than half of obtained valColor, set that color to max color depth
                Otherwise, set it to 0
                this returns a fully-posterized image.*/
                if ((int)atoi(tokenCurrent) > (valColor / 2))
                    imagePictureInt[countRow][countColumn] = valColor;
                else
                    imagePictureInt[countRow][countColumn] = 0;

                tokenCurrent = strtok(NULL, " ");
                countColumn++;
            }

            //when finished, return error if total column count is less than width available; this means that a row is not completely filled; this is not wanted!
            if (countColumn < valWidth)
            {
                int errorReturn = printCorrupt(fileInput, fileOutput);
                return errorReturn;
            }
            countColumn = 0; //resets column to position 0
            countRow++;
        }
    }
    else //if imageTypeMain is P5
    {
		/*Since P5 is a single line of adjacent characters, 
		simply take line & read each character one by one*/
        int bytesRead;
        unsigned char chunkChar[valWidth];
        for(;;)
        {
            bytesRead = fread(chunkChar, 1, valWidth, fileInput);
            if(bytesRead < valWidth)
                break;

            //same as that of P2
            for (countColumn = 0; countColumn < valWidth; countColumn++)
            {
                if ((int)chunkChar[countColumn] > round(valColor / 2))
                    imagePictureInt[countRow][countColumn] = valColor;
                else
                    imagePictureInt[countRow][countColumn] = 0;
            }
            countRow++;
        }
    }

    //this prints out the chars stored in array to its respective positions
    for (int tempCountRow = 0; tempCountRow < valWidth; tempCountRow++)
    {
        for (int tempCountColumn = 0; tempCountColumn < valHeight; tempCountColumn++)
        {
            if (strcmp(imageTypeMain, "P5") == 0) //for P5, simply print char by char, from array
                fprintf(fileOutput, "%c", (unsigned char)imagePictureInt[tempCountRow][tempCountColumn]);

            else //for P2, print by array & cut to newline when needed
            {
                fprintf(fileOutput, "%d", imagePictureInt[tempCountRow][tempCountColumn]);
                if (tempCountRow < (valWidth - 1))
                    fprintf(fileOutput, " ");
                else
                    fprintf(fileOutput, "\n");
            }
        }
    }
    return 0;
}
