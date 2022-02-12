# Image-O-Matic
A C project to manipulate PNG images using the [lodepng](https://lodev.org/lodepng/) library

To run it, download the repo and run `make` inside the directory. Afterwards, run `./Imageomatic`.


## Commands

- `A` or `B` followed by file name, loads an image into register `A` or `B` respectively
- `+ <filename>` writes the image in register `A` to a new file with name `<filname>`
- `C` copies the content of register `B` to register `A`
- `Z` terminates the program
- `Q` turn the image to grayscale. Only for register `A`

![Gray scale image](img/gray.png)

- `N` turns the image in. regsiter `A` to a negative image. Resulting image will be stored in register `A`.

![Gray scale image](img/negativo.png)

- `H` scales the image in register `A` to half its original size. Resulting image will be stored in register `A`.

![Gray scale image](img/metade.png)

- `P <color> <width> <height>` creates a new image with just the color `<color>`. `<color>` can be hexadecimal or a name of a color inside `cores.txt`. Resulting image will be stored in register `A`.

![Gray scale image](img/pintar.png)

- `R` rotates the image in register `A` 90 degrees.  Resulting image will be stored in register `A`.

![Gray scale image](img/rotacao_90.png)

- `O` posterizes the image (reduced color pallete) in register `A`. Resulting image will be stored in register `A`.

![Gray scale image](img/poster.png)

- `G <width> <height>` creates a image simulating the waves created by a drop of water. Resulting image will be stored in register `A`.

![Gray scale image](img/goticula.png)

- `D` blurs the image in register `A`. This is not gaussian blur. Resulting image will be stored in register `A`.

![Gray scale image](img/desfocado.png)

- `M` places a mask onto the image saved in register `A`. The mask is saved in register `B`. The mask and the image must have the same size. Resulting image will be stored in register `A`.

![Gray scale image](img/mascarar.png)

- `F <width> <height> <scale>` creates an image with a graph representation of a function. Resulting image will be stored in register `A`.

![Gray scale image](img/funcao.png)

- `T` converts the image in register `A` using a dithering algorithm. Resulting image will be stored in register `A`.

![Gray scale image](img/matizacao.png)

- `E` conceals a message into the image in register `A`, also known as steganography. It uses 6-bit ASCII tables to store the text in the two least-significant bits of the R, G and B components of the image. Resulting image will be stored in register `A`.
