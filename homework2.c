#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int n = 5;

	pid_t first_parent_pid = getpid();
	printf("First parent process id: %d\n", first_parent_pid);

	for (int i = 0; i < n; i++)
	{
		pid_t new_pid = fork();	// fork in parent

		if (new_pid == 0) // child
			printf("Parent %d created child %d\n", getppid(), getpid()); 

		else // parent
		{
			int status;
			pid_t termin_child_pid = wait(&status); // save return status from child

			printf("Parent %d terminated child %d status: %d\n",
					getpid(), termin_child_pid, WEXITSTATUS(status));
	
			exit(42);
		}
	}

	return 0;
}

