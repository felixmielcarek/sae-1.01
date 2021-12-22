CC = gcc 		
CFLAGS = -Wall		

all : test_sae			

test_sae : test_sae.o sae.o	
	$(CC) -o test_sae test_sae.o sae.o

test_sae.o : test_sae.c sae.h	
	$(CC) -c $(CFLAGS) test_sae.c

sae.o : sae.c sae.h	
	$(CC) -c $(CFLAGS) sae.c

clean :				
	rm -f test_sae *.o

doc :
	doxygen