#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <signal.h>

int main(void)
{
    char buf[128];
    int status = 0;
    long syscall_num = 0;
    pid_t pid;

    memset(buf, 0, sizeof(buf));

    pid = fork();

    if (pid == 0)
	{
        prctl(PR_SET_PDEATHSIG, SIGHUP); 
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        puts("Give me some shellcode, k");
        gets(buf); 
    } 
    else
	{
        while (syscall_num != SYS_execve)
		{
			wait(&status);
			if (WIFEXITED(status) || WIFSIGNALED(status))
				return (puts("child is exiting..."), 0)
			syscall_num = ptrace(PTRACE_PEEKUSER, pid, 44, 0);
		}
        puts("no exec() for you");
        kill(pid, SIGKILL);
    }
    return 0;
}
