/*  who1.c version 0.2
    A version of the who program
    Open, read UTMP file and show results

    Version history
    0.1 first ver
    0.2 supress empty records
        formats time correctly            
*/

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> // for exit()
#include <time.h>
#include <string.h> /* for strcpy() */

#define SHOWHOST // include remote machine on output

void showtime(long);
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
    if(utbufp->ut_host[0] != '\0')
    {
        printf("(%s)", utbufp->ut_host); // the host
    }
#endif
    printf("\n");
}

/*  displays time in a format fitfor human consumption.
    uses ctime to build a string then picks parts out of it.
    Note: %12.12s prints a string 12 chars wide and limits
    it to 12 chars.
*/
void showtime(long timeval)
{
    char *cp; // to hold address of time
    cp = ctime(&timeval); // convert time to string
                          // string looks like
                          // Mon Dec 21 11:12:20 2020
                          // 012345678901234567890123
    printf("%12.12s", cp + 4); // pick 12 chars from pos 4     

}

