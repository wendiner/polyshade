polyshade.o:	polyshade.c polyshade.h
	gcc -c polyshade.c
converter.o:	converter.c
	gcc -c -g converter.c
converter:	converter.o	polyshade.o
	gcc converter.o polyshade.o -o converter -g $$(pkg-config allegro-5 allegro_primitives-5 --libs --cflags) -l:libm.so
clean:
	rm converter ./*.o
test:
	DEBUGINFOD_URLS="https://debuginfod.ubuntu.com" gdb -d ../src/glibc-2.39 --args converter teapot.obj teapot.bin
