#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <assert.h>

char** form_argv(char* line);

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");

    char buf[1024];
    while (fgets(buf, 1024, input) != NULL) {

        char* pol = strchr(buf, '\n');

	if (pol)
            *pol = '\0';
 
	char** argv_new = form_argv(buf);
	
        if (!fork()){ 
            execvp(argv_new[0], argv_new);
	    exit(0);
        }
        wait(NULL);

        free(argv_new);
    }

    fclose(input);
}


char** form_argv(char* line) {
    assert(line);

    int argc = 1;
    char* cur_tok = line;
    for (;;) {
        while (*cur_tok == ' ')
            cur_tok++;

        cur_tok = strchr(cur_tok, ' ');

        if (!cur_tok) break;

        argc++;
        cur_tok++;
    }

    char** argv = (char**) malloc((argc + 1)*sizeof(char*));

    cur_tok = strtok(line, " ");
    for (int i = 0; i < argc; i++) {
        argv[i] = cur_tok;
        cur_tok = strtok(NULL, " ");
    }

    argv[argc] = NULL;

    return argv;
}

