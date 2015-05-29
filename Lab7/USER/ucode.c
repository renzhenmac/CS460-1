#ifndef UCODE_C
#define UCODE_C

#include "utypes.h"

int show_menu()
{
    int i;
    printf("***************** Menu *******************\n");

    for (i = 0; commands[i].command; i++)
    {
        printf("  %s  ", commands[i].command);
    }

    printf("\n******************************************\n");
    
    return 1;
}

int help()
{
    int i;
    
    printf("\nAvailable Commands: \n");
    for (i = 0; commands[i].command; i++)
    {
        printf(" - %s: %s: %s\n", commands[i].command, commands[i].name, commands[i].help);
    }
    
    return 1;
}

int find_cmd(char *name)
{
    int i;

    for (i = 0; commands[i].command; i++)
    {
        if (!strcmp(commands[i].command, name))
            return i;
    }
    
    return -1;
}

int sgetpid()
{
    int pid = syscall(0, 0, 0);
    printf("PID: %d\n", pid);
    return pid;
}

int getpid()
{
   return syscall(0, 0, 0);
}

int ps()
{
   return syscall(1, 0, 0);
}

int chname()
{
    char s[64];
    printf("input new name : ");
    gets(s);
    return syscall(2, s, 0);
}

int kfork()
{   
    int child, pid;
    pid = getpid();
    printf("Proc[%d] enter kernel to kfork a child\n", pid); 
    child = syscall(7, 0, 0);
    printf("Proc[%d] kforked a child %d\n", pid, child);
}    

int kswitch()
{
    return syscall(4,0,0);
}

int wait()
{
    int child, exitValue;
    printf("Proc[%d] enter Kernel to wait for a child to die\n", getpid());
    child = syscall(5, &exitValue, 0);
    printf("Proc[%d] back from wait, dead child=%d", getpid(), child);
    if (child >= 0)
        printf(", exitValue=%d", exitValue);
    printf("\n"); 
} 

int geti()
{
    char s[16];
    return atoi(gets(s));
}

int exit()
{
   int exitValue;
   printf("Exit value? ");
   exitValue = geti();
   printf("Entering kernel to die with exit value=%d\n", exitValue);
   _exit(exitValue);
}

int _exit(int exitValue)
{
    return syscall(6, exitValue, 0);
}


int getc()
{
    return syscall(90, 0, 0) & 0x7F;
}

int putc(char c)
{
    return syscall(91, c, 0, 0);
}

int invalid(char *name)
{
    printf("Invalid command : %s!\n", name);
}

int kmode()
{
	printf("Proc[%d] goining into KMode ....\n", getpid());
    syscall(3, 0, 0);
	printf("Proc[%d] back from the Kernel!\n", getpid());
}

int exec()
{
	int r;
  	char filename[32];
  	printf("enter exec filename : ");
  	gets(filename);
	printf("entered filename is %s\n", filename);
  	r = syscall(8, filename, 0, 0);
  	printf("exec failed\n");
}

int pipe() {
    if (syscall(30, pd, 0) >= 0) {
        printf("proc %d created a pipe with fd = %d %d\n", 
                getpid(), pd[0], pd[1]);
    } else {
        printf("Failed to create pipe.\n");
    }
}

void show_pipe() {
    int i;
    
    pfd();
    printf("Pipe? ");
    i = geti();
    syscall(35, i);
}

int pfd() {
    syscall(34,0,0,0);
}

int read_pipe() {
    char fds[32], buf[1024]; 
    int fd, n, nbytes;
    pfd();
    
    printf("FD? ");
    fd = geti();
    printf("Bytes? ");
    nbytes = geti();
    
    n = syscall(31, fd, buf, nbytes);
    
    if (n>=0) {
        printf("proc %d back to Umode, read %d bytes from pipe:\n",
               getpid(), n);
        buf[n]=0;
        printf("'%s'\n", buf);
    } else {
        printf("read pipe failed\n");
    }
}

int write_pipe() {
    char fds[16], buf[1024]; 
    int fd, n, nbytes;
    pfd();
    printf("FD? ");
    fd = geti();
    printf("String? ");
    gets(buf, 1024);
    //sscanf(fds, "%d %s", &fd, buf);
    nbytes = strlen(buf);
    //printf("fd=%d nbytes=%d : %s\n", fd, nbytes, buf);
    
    n = syscall(32, fd, buf, nbytes);
    
    if (n>=0) {
        printf("proc %d back to Umode, wrote %d bytes to pipe\n", getpid(),n);
    } else {
        printf("write pipe failed\n");
    }
}

int close_pipe() {
    char s[16]; 
    int fd;
    
    pfd();
    printf("FD? ");
    gets(s);
    fd = atoi(s);
    syscall(33, fd);
}

int sout()
{
  syscall(9, "serial line from Umode\r\n");
}


int sin()
{
  char uline[64];
  syscall(10, uline, 0);
  printf("uline=%s\n", uline);
}

#endif