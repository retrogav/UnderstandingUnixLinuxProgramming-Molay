/*  https://github.com/retrogav/UnderstandingUnixLinuxProgramming-Molay

    who1.c version 0.3
    A version of the who program
    Open, read UTMP file and show results

    Version history
    0.1 first ver
    0.2 supress empty records
        formats time correctly
    0.3 align formating of logon time to look the same as who
    0.4 added buffering with an array of structs

    To compile -
    cc who1.c utmplib.c -o who1
*/

#include <stdio.h>
#include <utmp.h> // for utmp struct
#include <stdlib.h> // for exit()
#include <time.h> // for strftime()

#include "who1.h"

#define SHOWHOST // include remote machine on output

void show_info(struct utmp *utbufp);
void showtime(long timeval);

int main() {
    // utmp strut is about login records. The record info gets read into it
    struct utmp *utbufp; // holds pointer to next record 
    
    if (utmp_open(UTMP_FILE) == -1) {
        perror(UTMP_FILE);
        exit(1);
    }
    
    while ((utbufp = utmp_next()) != NULLUT) {
        show_info(utbufp);        
    }

    utmp_close();    

    return 0;
}

/*  displays contents of the utmp struct in human readable form.
    displays nothing if record has no username
    note - these sizes should not be hardwired
*/
void show_info(struct utmp *utbufp) {
    if (utbufp->ut_type != USER_PROCESS) {
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
    printf("%-8.8s", utbufp->ut_user); // the logname
    printf(" ");
    printf("%-12.8s", utbufp->ut_line); // the tty    
    printf(" ");
    showtime(utbufp->ut_tv.tv_sec); // display time    
    printf(" ");
#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0') {
        printf("(%s)", utbufp->ut_host); // the host
    }
#endif
    printf("\n");
}

/*  displays time in a format fit for human consumption */
void showtime(long timeval) {
    struct tm * timeinfo;
    char buffer [80];
    
    timeinfo = localtime(&timeval);

    /* strftime() uses the struct tm. 
    Character sequences in the format specification -
        %Y  The year as a decimal number
        %m  The  month (01 to 12)
        %d  The  day  of  the  month (01 to 31)
        %H  The hour using a 24-hour clock (00 to 23)
        %M  The minute (00 to 59)
    */
    strftime(buffer, 80, "%Y-%m-%d %H:%M", timeinfo);    
    printf("%12s", buffer);
}
