# 355-project-2 - Konane

Directory structure:

	  Main directory:
	       - All C and H files needed to compile the program
	       - Makefile
	       - Driver for testing against other agents (ND.pl, knmovecheck)
	       - Board configurations D5.txt, E4.txt and newgame.txt
	       - Konane random agent program

	  test_boards:
	       - .txt files of board configurations for testing

          ProjectReport:
	       - final report for project

	  mini_report:
	       - Latex file for mini report
	       - Makefile for latex file

Program use:

	1. Run make to compile the program
	2. Run ./konane.exe board_filename start_colour<B|W>
	       ex. ./konane.exe newgame.txt B
	       ex. ./konane.exe newgame.txt W
	3. Options:
	       -v prints out details of program execution
	       -h prints a usage message

Program use with driver:

	1. Run make to compile the program
	2. Run ./ND.pl konane.exe other_program_name to play as black or ./ND.pl other_program_name konane.exe to play as white
