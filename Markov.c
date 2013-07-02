//Andrew Christoforakis and Arin Schwartz

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "model.c"
#include "sim.c"

int alphabetSize(char *s){
	bool ASCII[256];
	//intialize all values to false
	int i;
	for (i=0; i < 256; i++)
		ASCII[i] = false;
	//iterate over array
	while (*s != '\0'){
		ASCII[(int) *s] = true;
		s++;
	}
	///count number of true values
	int t = 0, j;
	for (j=0; j < 256; j++){
		if (ASCII[j])
			t++;
	}
	return t;
}

//takes k+1 length pattern, forms k-length substring to compare
double laplace(HashTable h, char* pattern){
	assert(h != NULL);
	//makes it clear that substring is length k
	int k = strlen(pattern)-1;
	//makes it clear we're accounting for null terminator
	char substring[k+1];
	strncpy(substring,pattern,k);
	substring[k] = '\0';
	int countPattern = getCount(h,pattern);
	int countSubstring = getCount(h,substring);
	return log((double) (countPattern+1)/(countSubstring+h->alphabetSize));
}

//takes hash table, input string to compare to model, kth order, and alphabet size
double likelihood(HashTable h, char* sequence, int k){
	assert(h != NULL);
	assert(sequence != NULL);
	char pattern[k+2];
	double total= 0.0;
	int i, j, length = strlen(sequence);
	for (i=0; i < length; i++){
		//initialize pattern
		for (j=0; j < k+1; j++)
			pattern[j] = sequence[(i+j)%length];
		pattern[k+1] = '\0';
		total += laplace(h,pattern);
	}
	return total/strlen(sequence);
}

HashTable makeModel(char* text, int k){
	int s = alphabetSize(text);
	HashTable h = mkHashTable(s);
	int i, j, count1=0, count2=0, length = strlen(text);
	for (i=0; i < length; i++){
		char pattern1[k+1];
		char pattern2[k+2];
		for (j=0; j<k; j++)
			pattern1[j] = pattern2[j] = text[(i+j)%length];
		pattern1[k] = '\0';
		pattern2[k] = text[(i+k)%length];
		/* need to manually set last char to null char
		 * or else get odd characters. For loop only
		 * correctly constructs substrings of length
		 * k, pads the rest of space with nonsense.
		 * This was the simplest fix.
		 */ 
		pattern2[k+1] = '\0';
		count1 = getCount(h,pattern1);
		count2 = getCount(h,pattern2);
		setKey(h,pattern1,count1+1);
		setKey(h,pattern2,count2+1);
	}	
	return h;
}

int main(int argc, char *argv[]){

	if (argc < 2) {
		fprintf(stderr, "usage: <filename>\n");
		exit(2);
	}

	FILE *f1 = fopen(argv[1], "r");
	if (f1 == NULL) {
		fprintf(stderr, "Failed to open: %s\n", argv[1]);
		exit(2);
	}

        FILE *f2 = fopen(argv[2], "r");
        if (f2 == NULL) {
                fprintf(stderr, "Failed to open: %s\n", argv[2]);
                exit(2);
        }

        FILE *f3 = fopen(argv[3], "r");
        if (f3 == NULL) {
                fprintf(stderr, "Failed to open: %s\n", argv[3]);
                exit(2);
        }

	//for sake of comparing answers
	int length = strlen(argv[3]);
	char outFile[length+1];
	int i;
	for (i=0; i < length-3; i++)
		outFile[i] = argv[3][i];
	outFile[length-3] = 'o';
	outFile[length-2] = 'u';
	outFile[length-1] = 't';
	outFile[length] = '\0';

	FILE *f4;
	char* answerString;

	if (outFile != NULL){
		f4 = fopen(outFile,"r");
		if (f4 == NULL){
			fprintf(stderr, "Failed to open: %s\n", argv[4]);
			exit(2);
		}
		answerString = getText(f4);
	}

	char *model1Text = getText(f1);
	char *model2Text = getText(f2);
	char *mysteryQuote = getText(f3);
        //makes output line up with answer string for obama/mccain comparison
        for (i=0; i < 14; i++)
                argv[3]++;	

	int k = 2;
	HashTable m1 = makeModel(model1Text,k);
	HashTable m2 = makeModel(model2Text,k);
	double l1 = likelihood(m1,mysteryQuote,k);
	double l2 = likelihood(m2,mysteryQuote,k);

	printf("%s %.6f %.6f %.6f (our estimate)\n",argv[3],l1,l2,l1-l2);
	printf("%s (answer file)\n",answerString);
	freeHashTable(m1);
	freeHashTable(m2);

	return 0;

}
