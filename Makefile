

make:
	cc -std=c11 -o Imageomatic Imageomatic0.c Imageomatic.c lodepng.c -lm

start: clean make
	./Imageomatic

clean:
	rm -f Imageomatic
