/*
	Linguagens e Ambientes de Programação - Projeto de 2020/2021

	Imageomatic module body

largura maxima = 100 colunas
tab = 4 espaços
0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

	Este ficheiro constitui apenas um ponto de partida para o
	seu trabalho. Todo este ficheiro pode e deve ser alterado
	à vontade, a começar por este comentário.


 IDENTIFICAÇÃO DOS AUTORES -
	Aluno 1: numero, nome
	Aluno 2: numero, nome

Comentarios:

?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????

*/

#include "Imageomatic.h"



/*** TYPE Int2 ***/

/* More Int2 functions, in case you need them */




/*** TYPE Pixel ***/

/* More Pixel functions, in case you need them */




/*** TYPE Image ***/

void initialization(void)
{
	// This function is automatically called when the interpreter starts.
	// If you need to perform some initialization, this is the place
	// to write the initialization code.
}

Int2 imageCopy(Image img, Int2 n, Image res)
{
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		res[i.x][i.y] = img[i.x][i.y];
	}
	return n;
}

Int2 imageGrayscale(Image img, Int2 n, Image res) {
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		Pixel p = img[i.x][i.y];
		int gray = pixelGrayAverage(p);
		res[i.x][i.y] = pixelGray(gray);
	}
	return n;
}

Int2 imageNegative(Image img, Int2 n, Image res) {
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
	  Pixel p = img[i.x][i.y];
		Pixel new = pixel(MAX_COLOR - p.red, MAX_COLOR - p.green , MAX_COLOR - p.blue);
		res[i.x][i.y] = new;
	}
	return n;
}

//nao funciona para imagens de tamanho impar
Int2 imageHalf(Image img, Int2 n, Image res) {
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y = i.y+2)
	for(i.x = 0; i.x < n.x; i.x = i.x+2) {
		res[i.x/2][i.y/2] = img[i.x][i.y];
	}
	i.y = n.y/2; i.x = n.x/2;
	return i;
}


Int2 imagePaint(String cor, Int2 n, Image res) {

	FILE * file = fopen(colorsFileName, "r");

	Pixel p = black;
	String color_name;
	int hex = 0;
	int color = 0;
	bool hasColor = false;

	// Iterate over the entire file
	while (!hasColor && fscanf(file, "%x %s", &color, color_name) != EOF) {
		if (strcmp(cor, color_name) == 0) {
			hasColor = true;
			break;
		}
	}

	// Check if cor is an hex string
	if (strlen(cor) == 6 && sscanf(cor, "%x", &color) != EOF)
		hasColor = true;

	if (hasColor) {
		int red = (color & 0xFF0000) >> 16;
		int green = (color & 0x00FF00) >> 8;
		int blue = color & 0x0000FF;

		p = pixel(red, green, blue);
	}

	// Paint the entire thing
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		res[i.x][i.y] = p;
	}

	return n;
}

Int2 imageRotation90(Image img, Int2 n, Image res)
{
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		res[(n.y-1) - i.y][i.x] = img[i.x][i.y];
	}
	return int2(n.y, n.x);
}

Int2 imagePosterize(Image img, Int2 n, int factor, Image res)
{

	int G = 256 / (1 << factor);
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		Pixel p = img[i.x][i.y];
		Pixel new = pixel(G*(p.red / G), G*(p.green / G), G*(p.blue / G));

		res[i.x][i.y] = new;
	}
	return n;
}

Int2 imageDroplet(Int2 n, Image res) {
	Int2 i;
	Int2 center = int2Half(n);
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		double dist = int2Distance(center, i);
		int gray = 0.7 * MAX_COLOR + 0.3 * sin(dist / 20.0) * MAX_COLOR;
		res[i.x][i.y] = pixelGray(gray);
	}
	return n;
}

Int2 cropToImageBoundaries(Int2 current, Int2 limits)
{
	if (current.x < 0)
		current.x = 0;
	else if (current.x > limits.x)
		current.x = limits.x;

	if (current.y < 0)
		current.y = 0;
	else if (current.y > limits.y)
		current.y = limits.y;

	return current;
}

Int2 imageBlur(Image img, Int2 n, int nivel, Image res)
{
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		Pixel p = img[i.x][i.y];

		// Generate matrix Origin, LimitX and LimitY
		int levels = 2 * nivel + 1;
		Int2 O;
		int LX, LY;

		O = int2(i.x - nivel, i.y - nivel);
		LX = O.x + levels;
		LY = O.y + levels;

		if (LX > n.x)
			LX = n.x;
		if (LY > n.y)
			LY = n.y;

		// Verify if origin is within the image and cut it to fit it, if needed
		O = cropToImageBoundaries(O, n);

		// Iterate over all pixels in the sub-matrix
		Int2 j;
		int pixel_count = 0;
		int avgR = 0, avgG = 0, avgB = 0;
		for (j.y = O.y; j.y < LY; j.y++)
		for (j.x = O.x; j.x < LX; j.x++) {
			Pixel c = img[j.x][j.y];
			avgR = avgR + c.red;
			avgG = avgG + c.green;
			avgB = avgB + c.blue;
			pixel_count++;
		}

		// Replace with the average value
		Pixel new = pixel(avgR / pixel_count, avgG / pixel_count, avgB / pixel_count);
		res[i.x][i.y] = new;
	}
	return n;
}

Int2 imageMask(Image img1, Int2 n1, Image img2, Int2 n2, Image res) // pre: int2Equals(n1, n2)
{
	Int2 i;
	for(i.y = 0; i.y < n1.y; i.y++)
	for(i.x = 0; i.x < n1.x; i.x++) {
		Pixel a = img1[i.x][i.y];
		Pixel b = img2[i.x][i.y];

		int red = (a.red * b.red)/MAX_COLOR;
		int green = (a.green * b.green)/MAX_COLOR;
		int blue = (a.blue * b.blue)/MAX_COLOR;

		res[i.x][i.y] = pixel(red, green, blue);
	}
	return n1;
}

Int2 imageFunctionPlotting(DoubleFun fun, int scale, Int2 n, Image res)
{
	return int2Error;
}

Int2 imageOrderedDithering(Image img, Int2 n, Image res)
{
	#define INDEX_SIDE  8
	Byte indexMatrix[INDEX_SIDE][INDEX_SIDE] = {
					{ 0, 32,  8, 40,  2, 34, 10, 42},
					{48, 16, 56, 24, 50, 18, 58, 26},
					{12, 44,  4, 36, 14, 46,  6, 28},
					{60, 28, 52, 20, 62, 30, 54, 22},
					{ 3, 35, 11, 43,  1, 33,  9, 41},
					{51, 19, 59, 27, 49, 17, 57, 25},
					{15, 47,  7, 39, 13, 45,  5, 37},
					{63, 31, 55, 23, 61, 29, 53, 21}
			};
		Int2 i;
    for(i.y = 0; i.y < n.y; i.y++)
    for(i.x = 0; i.x < n.x; i.x++) {
      Pixel p = img[i.x][i.y];
      double average =  pixelGrayAverage(p)/4.0;
      int x = i.x % INDEX_SIDE;
      int y = i.y % INDEX_SIDE;

      if (average > (double) indexMatrix[x][y])
          res[i.x][i.y] = white;
      else {
          res[i.x][i.y] = black;
      }
    }
    return n;
}

char convertToSixBits(char c) {
	if (c == '@')
		return '?';

	else if (c >= 'a' && c <= 'z')
		return c - 96;

	else if (c >= 'A' && c <= 'Z')
		return c - 64;

	else if (c >= ' ' && c <= '?')
		return c;

	else
		return '?';
}

Int2 imageSteganography(Image img, Int2 n, String s, Image res)
{
	return int2Error;
}

Int2 imageCompare(Image imgA, Int2 nA, Image imgB, Int2 nB, Image res)
{
	bool isDifferent = false;

	if (int2Equals(nA, nB) == false) {
		printf("Images are different in size!\n");
		printf("Size A\n\tx: %d y: %d", nA.x, nA.y);
		printf("Size B\n\tx: %d y: %d", nB.x, nB.y);
		return nA;
	}

	Int2 i;
	for(i.y = 0; i.y < nA.y; i.y++)
	for(i.x = 0; i.x < nA.x; i.x++) {
		Pixel a = imgA[i.x][i.y];
		Pixel b = imgB[i.x][i.y];

		if (!pixelEquals(a, b)) {
			res[i.x][i.y] = pixel(255, 0, 255);
			isDifferent = true;
		}
	}

	if (isDifferent) {
		printf("Images are different! Stored in A...\n");
		return nA;

	} else {
		printf("Images are the same!\n");
		res = imgA;
		return nA;
	}
}


void imageTests(void)
{
	static Image img, img2, res;
	Int2 n;

	// Q
	n = imageLoad("img/frutos.png", img);
	n = imageGrayscale(img, n, res);
	imageStore("img/cinzento.png", res, n);

	// N
	n = imageLoad("img/frutos.png", img);
	n = imageNegative(img, n, res);
	imageStore("img/negativo.png", res, n);

	// H
	n = imageLoad("img/frutos.png", img);
	n = imageHalf(img, n, res);
	imageStore("img/metade.png", res, n);

	// P
	n = int2(512, 512);
	n = imagePaint("green", n, res);
	imageStore("img/pintar.png", res, n);

	// R
	n = imageLoad("img/frutos.png", img);
	n = imageRotation90(img, n, res);
	imageStore("img/rotacao_90.png", res, n);

	// O
	n = imageLoad("img/frutos.png", img);
	n = imagePosterize(img, n, 3, res);
	imageStore("img/poster.png", res, n);

	// G
	n = int2(512, 512);
	n = imageDroplet(n, res);
	imageStore("img/goticula.png", res, n);

	// D
	n = imageLoad("img/frutos.png", img);
	n = imageBlur(img, n, 5, res);
	imageStore("img/desfocado.png", res, n);

	// M
	n = imageLoad("img/frutos.png", img);
	n = imageDroplet(n, img2);
	n = imageMask(img, n, img2, n, res);
	imageStore("img/mascarar.png", res, n);

	// F
	n = int2(512, 512);
	n = imageFunctionPlotting(sin, 50, n, res);
	imageStore("img/funcao.png", res, n);

	// T
	n = imageLoad("img/frutos.png", img);
	n = imageOrderedDithering(img, n, res);
	imageStore("img/matizacao.png", res, n);

	// E
	n = imageLoad("img/frutos.png", img);
	n = imageSteganography(img, n, "atacamos ao amanhecer", res);
	imageStore("img/esteganografia.png", res, n);
}
