/*  https://github.com/retrogav/UnderstandingUnixLinuxProgramming-Molay

    who1.c version 0.3
    A version of the who program
    Open, read UTMP file and show results

    Version history
    0.1 first ver
    0.2 supress empty records
        formats time correctly
    0.3 align formating of logon time to look the same as who           
*/

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> // for exit()
#include <time.h>

#define SHOWHOST // include remote machine on output

void showtime(long timeval);
void show_info(struct utmp *utbufp);

int main()
{
    struct utmp utbuf; // read info into here
    int utmpfd;        // file descriptor
    int reclen = sizeof(utbuf);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
    {
        perror(UTMP_FILE); // UTMP_FILE is in utmp.h
        exit(1);
    }
    while (read(utmpfd, &utbuf, reclen) == reclen)
    {
        show_info(&utbuf);
    }
    close(utmpfd);
    return 0;
}

/*  displays contents of the utmp struct in human readable form.
    displays nothing if record has no username
    note - these sizes should not be hardwired
*/
void show_info(struct utmp *utbufp)
{
    if (utbufp->ut_type != USER_PROCESS)
    {
        return;
    }

    /*  the printf formatting used here works as follows -
        -           left-justify within the given field width. Right
                    justification is the default
        number      minimum number of characters to be printed. If the value
                    to be printed is shorter than this number, the result is
                    padded with blank spaces
        .number     precision. The minimum number of digits to be written
    */   
    printf("%-8.8s", utbufp->ut_name); // the logname
    printf(" ");
    printf("%-12.8s", utbufp->ut_line); // the tty
    printf(" ");
    showtime(utbufp->ut_tv.tv_sec); // display time    
    printf(" ");
#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0')
    {
        printf("(%s)", utbufp->ut_host); // the host
    }
#endif
    printf("\n");
}

/*  displays time in a format fit for human consumption.
*/
void showtime(long timeval)
{
    struct tm * timeinfo;
    char buffer [80];
    
    timeinfo = localtime(&timeval);

    strftime(buffer, 80, "%Y-%m-%d %I:%M", timeinfo);    
    printf("%12s", buffer);
}
