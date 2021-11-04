.PHONY: clean

all: serpinski_triangle_1 serpinski_triangle_2 serpinski_square

serpinski_triangle_1: serpinski_triangle_1.c
	gcc -O2 serpinski_triangle_1.c -o serpinski_triangle_1 -lglut -lGL -lGLU

serpinski_triangle_2: serpinski_triangle_2.c
	gcc -O2 serpinski_triangle_2.c -o serpinski_triangle_2 -lm -lglut -lGL -lGLU

serpinski_square: serpinski_square.c
	gcc -O2 serpinski_square.c -o serpinski_square -lglut -lGL -lGLU

clean:
	rm serpinski_triangle_1 serpinski_triangle_2 serpinski_square
