#ifndef WHO1_H	/* prevent multiple inclusions */
#define WHO1_H

int utmp_open(char *filename);
struct utmp * utmp_next();
void utmp_close();

#define NULLUT      ((struct utmp *) NULL)

#endif /* ifndef WHO1_H */ 