#ifndef WHO1_H	/* prevent multiple inclusions */
#define WHO1_H

int utmp_open(char *filename);
struct utmp * utmp_next();
void utmp_close();

#endif /* ifndef WHO1_H */ 