CC=gcc
CFLAGS=-O3 -g
 
TARGET=test mandel
 
all: $(TARGET)
 
# Suppression de la règle de compilation de libppm.so existante, ajout de la règle correcte pour créer libppm.so à partir de ppm.
libppm.so : ppm.c
	$(CC) $(CFLAGS) -fpic -shared ppm.c -o libppm.so
 
# J'ai ajouté "-L. -lppm" pour que le programme mandel soit lié correctement
 
# Suppression de $(LDFLAGS) de la règle test, ajout de la bonne syntaxe pour lier avec libppm.so
test: main.c libppm.so
	$(CC) $(CFLAGS) main.c -L. -lppm -o test

# Suppression de $(LDFLAGS) de la règle mandel, ajout de la bonne syntaxe pour lier avec libppm.so
mandel: mandel.c libppm.so
	$(CC) $(CFLAGS) mandel.c -L. -lppm -o mandel 
 

clean:
	 rm -fr $(TARGET) *.so
