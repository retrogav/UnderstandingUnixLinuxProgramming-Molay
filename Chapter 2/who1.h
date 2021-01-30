/*  https://github.com/retrogav/UnderstandingUnixLinuxProgramming-Molay

    who1.h version 0.1
    Header file for who1 program

    Version history
    0.1 first ver
*/

#ifndef WHO1_H	/* prevent multiple inclusions */
#define WHO1_H

int utmp_open(char *filename);
struct utmp * utmp_next();
void utmp_close();

#define NULLUT      ((struct utmp *) NULL)

#endif /* ifndef WHO1_H */ 