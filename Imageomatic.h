/*
    Linguagens e Ambientes de Programação - Projeto de 2020/2021

    Imageomatic module interface

max width = 100 columns
tab = 4 spaces
0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

Este ficheiro não pode ser alterado!
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "lodepng.h"


/*** TYPE String ***/

#define MAX_STRING		256
typedef char String[MAX_STRING];


/*** TYPE Int2 ***/

/* This type is used to represent the two coordinates of a pixel in an image,
   and also to represent the two dimensions of a rectangle */

typedef struct {
	int x, y;
} Int2;

extern Int2 int2Zero, int2Error;

extern Int2 int2(int x, int y);
extern bool int2Equals(Int2 a, Int2 b);
extern bool int2LessThan(Int2 a, Int2 b);
extern bool int2LessThanOrEqual(Int2 a, Int2 b);
extern bool int2IsError(Int2 n);
extern bool int2Between(Int2 i, Int2 a, Int2 b);
extern bool int2Between2(Int2 i, Int2 a, Int2 b);
extern Int2 int2Min(Int2 a, Int2 b);
extern Int2 int2Max(Int2 a, Int2 b);
extern Int2 int2Add(Int2 a, Int2 b);
extern Int2 int2Half(Int2 a);
extern double int2Distance(Int2 a, Int2 b);


/*** TYPE Pixel ***/

#define MAX_COLOR			255

typedef unsigned char Byte;

typedef struct {
	Byte red, green, blue;
} Pixel;

extern Pixel white, red, green, gray, blue, black;

extern Pixel pixel(int red, int green, int blue);
extern Pixel pixelGray(int b);
extern bool pixelEquals(Pixel a, Pixel b);
extern int pixelGrayAverage(Pixel p);


/*** TYPE Image ***/

#define MAX_X			1024
#define MAX_Y			1024

typedef Pixel Image[MAX_X][MAX_Y];

/* TESTAR APONTADOR PARA MATRIX !!! */

typedef double (DoubleFun)(double);

extern String colorsFileName; // the name of the file of colors 

void initialization(void);
extern Int2 imageLoad(String filename, Image res);
extern bool imageStore(String filename, Image img, Int2 n);

extern Int2 imageCopy(Image img, Int2 n, Image res);
extern Int2 imageGrayscale(Image img, Int2 n, Image res);
extern Int2 imageNegative(Image img, Int2 n, Image res);
extern Int2 imageHalf(Image img, Int2 n, Image res);
extern Int2 imagePaint(String cor, Int2 n, Image res);
extern Int2 imageRotation90(Image img, Int2 n, Image res);
extern Int2 imagePosterize(Image img, Int2 n, int factor, Image res);
extern Int2 imageDroplet(Int2 n, Image res);
extern Int2 imageBlur(Image img, Int2 n, int nivel, Image res);
extern Int2 imageMask(Image img1, Int2 n1, Image img2, Int2 n2, Image res);
extern Int2 imageFunctionPlotting(DoubleFun fun, int scale, Int2 n, Image res);
extern Int2 imageOrderedDithering(Image img, Int2 n, Image res);
extern Int2 imageSteganography(Image img, Int2 n, String s, Image res);

extern void imageTests(void);


