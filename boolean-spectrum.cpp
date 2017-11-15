#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define dim 6

void makefunction_int(char str[], int fnint[]);
void makefunction_boolean(char str[], unsigned int fn[]);
void f_spectrum(int fnint[],  int fspec[]);
unsigned nonlinearity(int fspec[]);
void weight_distribution(int fspec[]);

unsigned int size;


main()
{
	unsigned int i, j, k, nl;
	size = (unsigned int)pow(2, dim); //size is the number of element in the domain of the Boolean function.

	char str[size];
	unsigned int fn[size];
	int fnint[size], fspec[size];
	FILE *fp;

	fp = fopen("bent6.txt", "r"); //the input funcitons are stored in function1.txt.

	for (k = 0; k < 1; k++){
		fscanf(fp, "%s\n", str);
		makefunction_int(str, fnint);
		makefunction_boolean(str, fn);

		f_spectrum(fnint, fspec);
     	for (i = 0; i < size; i++) printf("%d ", fspec[i]);
    	printf("\n");

        nl = nonlinearity(fspec);

		printf("Nonlinearity = %d\n", nl);

		weight_distribution(fspec);
	}
	fclose(fp);
}

//converts character string to character form of Boolean function
void makefunction_int(char str[], int fnint[]){
	unsigned int  i, size;
	size = (unsigned int)pow(2, dim);

	for (i = 0; i < size; i++){
		if (str[i] == '1'){
			fnint[i] = -1;
		}
		if (str[i] == '0'){
			fnint[i] = 1;
		}
	}
	return;
}

//converts character string to Boolean function
void makefunction_boolean(char str[], unsigned int fn[]){
	unsigned int  i;
	for (i = 0; i < size; i++){
		if (str[i] == '1'){
			fn[i] = 1;
		}
		if (str[i] == '0'){
			fn[i] = 0;
		}
	}
	return;
}


void f_spectrum(int fnint[],  int fspec[]){
	unsigned int i, j, k, tmp, p2dim, p2jplus1, p2j;

	for ( i = 0; i < size;  i++) fspec[i] = fnint[i];
	p2dim = (unsigned int)pow( 2, dim );
	for ( j = 0; j < dim; j++ ){
		p2jplus1 = (unsigned int)pow( 2, j+1 );
		p2j = (unsigned int)pow( 2, j );
		for ( k = 0; k < p2dim;  k = k + p2jplus1){
			for ( i = k; i < k + p2j; i++){
				tmp = fspec[i];
				fspec[i] = tmp + fspec[i + p2j];
				fspec[i + p2j] = tmp - fspec[i + p2j];
			}
		}
	}
	return;
}


unsigned nonlinearity(int fspec[]){
	unsigned i;
	unsigned max, nl1;
	int fspecabs[size];
	size = (unsigned int)pow(2, dim);

	for (i = 0;  i < size;  i++) fspecabs[i] = abs(fspec[i]);
	max = 0;
	for (i = 0;  i < size;  i++){
		if (max < fspecabs[i]) max = fspecabs[i];
	}
	nl1 =  (unsigned int)pow(2, dim -1) - max/2;
	return nl1;
}

void weight_distribution(int fspec[]){
	unsigned i;

	int wtdist[2*size + 1];

	for (i = 0;  i < 2*size+1;  i++) wtdist[i] = 0;

	for (i = 0;  i < size;  i++){
		wtdist[(size + fspec[i])]++;
	}

	for (i = 0; i < 2*size + 1; i++){
		if (wtdist[i] != 0) printf("%d, %d\n", (i - size), wtdist[i]);
	}
	return;
}
