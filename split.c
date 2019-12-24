#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>

void split(char* string, char* delimiters, char*** tokens, int* tokensCount);

int main(){
	char* string = strdup("Hello                   Arslan,                             Misha;Artem");
	char* delimiters = strdup(" \n\t,;");
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
	char* pc;
	while (string[i]){
		j = 0;
		while(delimiters[j]){
			int flag = 0;
			while( string[i + 1] == delimiters[j] ){
				flag = 1;
				i++;	
			//	string[i] = 0xa;	
			//	break;
			}
			if (flag == 1)
				string[i] = 0xa;
			j++;
		}
		i++;
	}
	pc = string;
	for(;;){
//		pc = strchr(pc,0xa);
//		if (!pc)
//			break;
		
		while (*pc  == 0xa || *pc == ' ')
			pc++;
		printf("%d\n", *pc);
			break;
	}
	(*tokensCount)++;
	*tokens = (char**) calloc(*tokensCount, sizeof(char*));
	i = 0;
	j = 0;
	(*tokens)[j++] = string;
	pc = string;
	for(;;){
		pc = strchr(pc, 0xa);
		if (!pc)
			break;

		while(*pc == 0xa)
			pc++;		
		(*tokens)[j++] = pc;
	}
}
