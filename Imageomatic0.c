/*
    Linguagens e Ambientes de Programação - Projeto de 2020/2021

    Imageomatic0 module body

max width = 100 columns
tab = 4 spaces
0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

Código oferecido.

Este ficheiro não pode ser alterado!
*/


#include "Imageomatic.h"


/*** TYPE Int2 ***/

Int2 int2Zero = {0, 0};
Int2 int2Error = {-1, -1};

Int2 int2(int x, int y)
{
	Int2 t = {x, y};
	return t;
}

bool int2Equals(Int2 a, Int2 b)
{
	return a.x == b.x && a.y == b.y;
}

bool int2LessThan(Int2 a, Int2 b)
{
	return a.x < b.x && a.y < b.y;
}

bool int2LessThanOrEqual(Int2 a, Int2 b)
{
	return a.x <= b.x && a.y <= b.y;
}

bool int2IsError(Int2 n)
{
	return int2Equals(n, int2Error);
}

bool int2Between(Int2 a, Int2 i, Int2 b)
{
	return int2LessThanOrEqual(a, i) && int2LessThan(i, b);
}

bool int2Between2(Int2 a, Int2 i, Int2 b)
{
	return int2LessThanOrEqual(a, i) && int2LessThanOrEqual(i, b);
}

Int2 int2Min(Int2 a, Int2 b)
{
	return int2(a.x<b.x?a.x:b.x, a.y<b.y?a.y:b.y);
}

Int2 int2Max(Int2 a, Int2 b)
{
	return int2(a.x<b.x?b.x:a.x, a.y<b.y?b.y:a.y);
}

Int2 int2Add(Int2 a, Int2 b)
{
	return int2(a.x + b.x, a.y + b.y);
}

Int2 int2Half(Int2 a)
{
	return int2(a.x / 2, a.y / 2);
}

double int2Distance(Int2 a, Int2 b)
{
	double x_dist = abs(a.x - b.x);
	double y_dist = abs(a.y - b.y);
	return sqrt(x_dist * x_dist + y_dist * y_dist);
}


/*** TYPE Pixel ***/

Pixel white = {255, 255, 255};
Pixel red = {255, 0, 0};
Pixel green = {0, 255, 0};
Pixel gray = {127, 127, 127};
Pixel blue = {0, 0, 255};
Pixel black = {0, 0, 0};

static int colorNormalize(int colorComponent)
{
	if( colorComponent < 0 )
		return 0;
	else if( colorComponent > MAX_COLOR )
		return MAX_COLOR;
	else
		return colorComponent;
}

Pixel pixel(int red, int green, int blue)
{
	Pixel p = {
			colorNormalize(red),
			colorNormalize(green),
			colorNormalize(blue)
	};
	return p;
}

Pixel pixelGray(int b)
{
	return pixel(b, b, b);
}


bool pixelEquals(Pixel a, Pixel b)
{
	return a.red == b.red
			&& a.green == b.green
			&& a.blue == b.blue;
}

int pixelGrayAverage(Pixel p)
{
	return (p.red + p.green + p.blue)/3;
}



/*** TYPE Image ***/

Int2 imageLoad(String filename, Image res)
{
	Byte *mem;
	unsigned int w, h;
	int error = lodepng_decode24_file(&mem, &w, &h, filename);
	Int2 n = int2(w, h);
	Pixel *p = (Pixel *)mem;
	if( error != 0 )
		return int2Error;
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		res[i.x][i.y] = *p++;
	}
	free(mem);
	return n;
}

bool imageStore(String filename, Image img, Int2 n)
{
	Byte *mem = malloc(MAX_X * MAX_Y * sizeof(Pixel));
	Pixel *p = (Pixel *)mem;
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		*p++ = img[i.x][i.y];
	}
	if( lodepng_encode24_file(filename, mem, n.x, n.y) != 0 )	
		return false;
	free(mem);
	return true;
}


/* MESSAGES */

static void message(String mens, String arg)
{
	if( strcmp(arg, "") == 0 )
		printf("%s.\n", mens);
	else
		printf("%s \"%s\".\n", mens, arg);
}

static bool error(String err)
{
	printf("ERRO: %s!\n", err);
	return false;
}


/* COMMAND LINE */

int stringSplit(String s, String parts[], int maxParts)
{
	int i, j=0, p=0;
	bool insideQuotes = false;
	for( i = 0 ; s[i] != '\0' ; i++ ) {
		if( insideQuotes ) {
			if( s[i] == '"' || s[i] == '\n' ) {
				insideQuotes = false;
				if( p != 0 ) {
					parts[j][p] = '\0';
					j++;
					p=0;
				}
			}
			else {
				parts[j][p] = s[i];
				p++;
			}
		}
		else {
			if( s[i] == '"' )
				insideQuotes = true;
			else if( isspace(s[i]) ) {
				if( p != 0 ) {
					parts[j][p] = '\0';
					j++;
					p=0;
				}
			}
			else {
				parts[j][p] = s[i];
				p++;
			}
		}
	}
	return j;
}

void stringToUpperCase(String s)
{
	int i;
	for( i = 0 ; s[i] != '\0' ; i++ ) {
		s[i] = toupper(s[i]);
	}
}

#define INVALID_INT		-1

int stringToInt(String s)
{
	int i, n = 0;
	for( i = 0 ; s[i] != '\0' ; i++ ) {
		if( isdigit(s[i]) ) {
			n = n * 10 + (s[i] - '0');
		}
		else
			return INVALID_INT;
	}
	return n;
}


/* INTERPRETER */

#define INT_INFINITE	9999999
#define MAX_POSTER		8
#define MAX_PARTS   	6

static Image ia, ib, ic;
static Int2 na, nb, nc;
static String prefix, postfix;
String colorsFileName;

static bool validateCommand(String parts[], int nParts,
			String expectedCommand, int expectedNParts)
{
	if( strcmp(parts[0], expectedCommand) != 0 )
		return error("Comando invalido");
	else if( nParts != expectedNParts )
		return error("Numero de argumentos incorreto");
	else
		return true;
}

static bool validateImage(char nome) {
	if( nome == 'A' && int2IsError(na) )
		return error("Imagem A indefinida");
	else if( nome == 'B' && int2IsError(nb) )
		return error("Imagem B indefinida");
	else
		return true;
}

static bool validateIntX(String s) {
	int i = stringToInt(s);
	if( 0 < i && i <= MAX_X )
		return true;
	else
		return error("Esperava-se um inteiro valido X");
}

static bool validateIntY(String s) {
	int i = stringToInt(s);
	if( 0 < i && i <= MAX_Y )
		return true;
	else
		return error("Esperava-se um inteiro valido Y");
}

static bool validateInt(String s, int max) {
	int i = stringToInt(s);
	if( 0 <= i && i <= max )
		return true;
	else
		return error("Esperava-se um inteiro valido");
}

static void executarComando(String parts[], int nParts)
{
	char s[4 * MAX_STRING];
	stringToUpperCase(parts[0]);
	char c0 = parts[0][0];
	//char c1 = parts[0][1];
	switch( c0 ) {
		case '+':
			if( validateCommand(parts, nParts, "+", 2)
			&& validateImage('A') ) {
				sprintf(s, "%s%s%s", prefix, parts[1], postfix);
				message("Escrita de A no ficheiro", s);
				if( !imageStore(s, ia, na) )
					error("Nao foi possivel escrever no ficheiro");
			}
			break;
		case 'A':
			if( validateCommand(parts, nParts, "A", 2) ) {
				sprintf(s, "%s%s%s", prefix, parts[1], postfix);
				message("Leitura do ficheiro", s);
				na = imageLoad(s, ia);
				if( int2IsError(na) )
					error("Ficheiro inacessivel");
			}
			break;
		case 'B':
			if( validateCommand(parts, nParts, "B", 2) ) {
				sprintf(s, "%s%s%s", prefix, parts[1], postfix);
				message("Leitura do ficheiro", s);
				nb = imageLoad(s, ib);
				if( int2IsError(nb) )
					error("Ficheiro inacessivel");
			}
			break;
		case 'C':
			if( validateCommand(parts, nParts, "C", 1)
			&& validateImage('B') ) {
				message("Copia B -> A", "");
				na = imageCopy(ib, nb, ia);
			}
			break;
		case 'Z':
			if( validateCommand(parts, nParts, "Z", 1) ) {
				exit(0);
			}
			break;
		case 'Q':
			if( validateCommand(parts, nParts, "Q", 1)
			&& validateImage('A') ) {
				message("Cinzento monocromático", "");
				nc = imageGrayscale(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;
		case 'N':
			if( validateCommand(parts, nParts, "N", 1)
			&& validateImage('A') ) {
				message("Negativo", "");
				nc = imageNegative(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;
		case 'H':
			if( validateCommand(parts, nParts, "H", 1)
			&& validateImage('A') ) {
				message("Metade", "");
				nc = imageHalf(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;
		case 'P':
			if( validateCommand(parts, nParts, "P", 4)
			&& validateIntX(parts[2])
			&& validateIntY(parts[3]) ) {
				Int2 n = int2(stringToInt(parts[2]), stringToInt(parts[3]));
				message("Pintar com a cor", parts[1]);
				na = imagePaint(parts[1], n, ia);
			}
			break;
		case 'R':
			if( validateCommand(parts, nParts, "R", 1)
			&& validateImage('A') ) {
				message("Rotacao 90", "");
				nc = imageRotation90(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;
		case 'O':
			if( validateCommand(parts, nParts, "O", 2)
			&& validateInt(parts[1], MAX_POSTER)
			&& validateImage('A') ) {
				message("Poster", parts[1]);
				nc = imagePosterize(ia, na, stringToInt(parts[1]), ic);
				na = imageCopy(ic, nc, ia);
			}
			break;
		case 'G':
			if(validateCommand(parts, nParts, "G", 3)
			&& validateIntX(parts[1])
			&& validateIntY(parts[2]) ) {
				Int2 n = int2(stringToInt(parts[1]), stringToInt(parts[2]));
				message("Goticula", "");
				na = imageDroplet(n, ia);
			}
			break;
		case 'D':
			if( validateCommand(parts, nParts, "D", 2)
			&& validateImage('A')
			&& validateInt(parts[1], INT_INFINITE)
			 ) {
				message("Desfocar", parts[1]);
				nc = imageBlur(ia, na, stringToInt(parts[1]), ic);
				na = imageCopy(ic, nc, ia);
			}
			break;
		case 'M':
			if( validateCommand(parts, nParts, "M", 1)
			&& validateImage('A') ) {
				message("Mascarar A+B -> A", "");
				if( !int2Equals(na, nb) )
					error("Imagens com diferentes dimensões");
				nc = imageMask(ia, na, ib, nb, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;
		case 'F':
			if(validateCommand(parts, nParts, "F", 4)
			&& validateIntX(parts[1])
			&& validateIntY(parts[2])
			&& validateInt(parts[3], INT_INFINITE) ) {
				Int2 n = int2(stringToInt(parts[1]), stringToInt(parts[2]));
				message("Grafico de funcao", "");
				na = imageFunctionPlotting(sin, stringToInt(parts[3]), n, ia);
			}
			break;
		case 'T':
			if( validateCommand(parts, nParts, "T", 1)
			&& validateImage('A') ) {
				message("Matizacao", parts[1]);
				nc = imageOrderedDithering(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;
		case 'E':
			if( validateCommand(parts, nParts, "E", 2)
			&& validateImage('A') ) {
				message("Esteganografia", parts[1]);
				nc = imageSteganography(ia, na, parts[1], ic);
				na = imageCopy(ic, nc, ia);
			}
			break;
		case '!':
			if( validateCommand(parts, nParts, "!", 1) ) {
				(void)(system("pwd")+1);
				break;
			}
			break;
		case '#':
			if( validateCommand(parts, nParts, "#", 1) ) {
				imageTests();
				break;
			}
			break;
		default:
			validateCommand(parts, nParts, "", 0);
			break;
	}
}

static void initialization2(void)
{
	na = nb = int2Error;
	strcpy(prefix, "img/");     // in Mooshak, the prefix will change
	strcpy(postfix, ".png");
	strcpy(colorsFileName, prefix);
	strcat(colorsFileName, "cores.txt");
	initialization();
}

static void informacao(void)
{
	if( int2IsError(na) )
		printf("A = (largura=?, altura=?)");
	else
		printf("A = (largura=%d, altura=%d)", na.x, na.y);
	printf("   ");
	if( int2IsError(nb) )
		printf("B = (largura=?, altura=?)");
	else
		printf("B = (largura=%d, altura=%d)", nb.x, nb.y);
}

static void interpreter(void)
{
	String parts[MAX_PARTS];
	int nParts;
	String line;
	for(;;) {
		informacao();
		printf("\n> ");
		if( fgets(line, MAX_STRING, stdin) == NULL ) {
			strcpy(line, "Z\n");
			printf("%s", line);
		}
		nParts = stringSplit(line, parts, MAX_PARTS);
#if 0	
		for( int i = 0 ; i < nParts ; i++ )
			printf("%s\n", parts[i]);
#endif
		if( nParts > 0 )
			executarComando(parts, nParts);
	}
}

int main(void)
{
	initialization2();
	interpreter();
	return 0;
}






