tests: pnmdump
	python "tests/runtests-1.0.py" ./pnmdump.exe

pnmdump: pnmdumpmain.c
	gcc pnmdumpmain.c -std=c99 -Wall -o pnmdump.exe