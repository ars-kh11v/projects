#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>

void split(char* string, char* delimiters, char*** tokens, int* tokensCount);

int main(){
	char* string = strdup("Hello Arslan, Misha	Artem");
	char* delimiters = strdup(" \n\t,");
	char** tokens = NULL;
	int tokensCount = 0;
	
	split(string, delimiters, &tokens, &tokensCount);
	
	for (int i = 0; i < tokensCount; i++){
		printf("%s\n", tokens[i]);
	}
	free(string);
	free(delimiters);
	free(tokens);  
}	

void split(char* string, char* delimiters, char*** tokens, int* tokensCount){
	int i = 0, j = 0;
	while (string[i]){
		j = 0;
		while(delimiters[j]){
			if( string[i] == delimiters[j] ){
				(*tokensCount)++;	
				string[i] = 0xa; 
				break;
			}
			j++;
		}
		i++;
	}
	(*tokensCount)++;
	*tokens = (char**) calloc(*tokensCount, sizeof(char*));
	i = 0;
	j = 0;
	(*tokens)[j++] = string;
	while (string[i]){
		if ( string[i] == 0xa){
			string[i] = 0;
			(*tokens)[j++] = &string[i+1];
		}
		i++;
	}
}
