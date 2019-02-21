Author: Hannah Lucki, Jay Piamjariyakul

Date of creation: March 11, 2018

***

This program, written in C, allows one to manipulate and convert PGM files, from ASCII to binary (and vice-versa), and rotate/flip them.

Also, one can view the binary contents of a specific file (in hexadecimal), whether by direct referencing or via stdin.

Additionally, the program also allows the user to posterize the image at two levels, between 0 and the maximum color depth.

NOTE: The program supports a maximum size of 512x512; any width or height greater than that causes the algorithm to produce erroneous results.

***

OPTIONAL OBJECTIVE: Image Posterization

Posterization is a process in which an image would be passed to a 'filter' selecting which colors should be set to a higher value, or if it should be set to a lower value.

This is done by utlizing a threshold value of the image's color depth divided by 2. Essentially, for each 'pixel'  in the image:

- Any pixel of integer value (either ASCII or binary) less than the threshold value gets stored as 0 (or equivalent) in the output image.

- Any pixel of integer value greater than or equal to the threshold value gets stored as the maximum possible color (the color depth) in the output.

This, therefore, returns a binary image of two colors, one of integer value 0 and another of the color depth.

***

To compile the code:

1) Open the terminal.

2) With the full directory of the folder containing the coupled files, type into the terminal:

	cd "[full directory of folder]"

3) At the current directory, type into the terminal:

	make

***

With the executable file 'pnmdump.exe' in such directory, type into the terminal, with your input files:

- For displaying program version:

	./pnmdump.exe --version
	
- For displaying usage instructions:

	./pnmdump.exe --usage

- For conversion from P2 to P5:

	./pnmdump.exe --P2toP5 [INPUT] [OUTPUT]

- For conversion from P5 to P2:
	
	./pnmdump.exe --P5toP2 [INPUT] [OUTPUT]
	
- For flipping the image via the y=x axis:

	./pnmdump.exe --rotate [INPUT] [OUTPUT]
	
- For rotating the image clockwise by 90 degrees:

	./pnmdump.exe --rotate90 [INPUT] [OUTPUT]

- For viewing the hexdump of an input file:
	
	./pnmdump.exe --hexdump [INPUT]
	
* Optionally, one can input the file via stdin:

	./pnmdump.exe --hexdump < [INPUT]
	
* One could also view the hexdump of a specific shell command:

	[COMMAND] | pnmdump.exe --hexdump
	
Replace [INPUT] or [OUTPUT] with the directory to the input file & where one wants the output file to be, respectively.

For --hexdump, replace [COMMAND] with the shell command to read the hexdump from.

OPTIONAL COMMANDS:

* One could also posterize an image & output such image to a new file:

	./pnmdump.exe --posterize [INPUT] [OUTPUT]

***

Example output (from directory of extracted zip file):

$ ./pnmdump.exe --hexdump "tests\hello.c"

0000000  2F /  2A *  20    68 h  65 e  6C l  6C l  6F o
0000008  2E .  63 c  20    2A *  2F /  0A .  0A .  23 #
0000010  69 i  6E n  63 c  6C l  75 u  64 d  65 e  20
0000018  3C <  73 s  74 t  64 d  69 i  6F o  2E .  68 h
0000020  3E >  0A .  0A .  69 i  6E n  74 t  20    6D m
0000028  61 a  69 i  6E n  28 (  69 i  6E n  74 t  20
0000030  61 a  72 r  67 g  63 c  2C ,  20    63 c  68 h
0000038  61 a  72 r  20    2A *  61 a  72 r  67 g  76 v
0000040  5B [  5D ]  29 )  0A .  7B {  0A .  20    20
0000048  70 p  75 u  74 t  73 s  28 (  22 "  48 H  65 e
0000050  6C l  6C l  6F o  20    77 w  6F o  72 r  6C l
0000058  64 d  21 !  5C \  6E n  22 "  29 )  3B ;  0A .
0000060  20    20    72 r  65 e  74 t  75 u  72 r  6E n
0000068  20    30 0  3B ;  0A .  7D }  0A .
000006e

****

CHANGELOG

16/02/18

- Began making hexdump program

- Completed version and usage programs

19/02/18

- Instructions changed - revised output format

22/02/18

- Completed hexdump program

23/02/18

- Began making file conversion program

- Modified "usage" to include new additional commands

01/03/18

- Completed P2toP5

- Began making P5toP2

05/03/18

- Complete rework of the program

- Tidied up code

- Re-implemented hexdump, version, and usage programs

- Began rework of P2toP5 and P5toP2

07/03/18

- Completed P2toP5 and P5toP2 programs

- Began flip program

09/03/18

- Completed flip program

- Began rotation program

11/03/18

- Completed rotation program

- First draft complete

- Makefile & README files created

- pnmdumpFinal v1 complete

15/03/18

- Fixed vital error in charConvertP2toP5()

- Removed imagePictureInt[][] array from charConvertP2toP5()

- Moved declaration of imagePictureInt[][] to charConvertRotate()

- Removed passing parameter of imagePictureInt[][] from charConvertMain()

- pnmdumpFinal v2 complete

19/03/18

- Added additional command "posterize"

- pnmdumpFinal v3 complete