/*  https://github.com/retrogav/UnderstandingUnixLinuxProgramming-Molay

    utmplib.c version 0.1
    Functions to buffer reads from a utmp file.
    Reads NRECS per read and then doles them out from the buffer

    Version history
    0.1 first ver
*/

#include <stdio.h>
#include <fcntl.h> // for open()
#include <unistd.h>  // for read(), close()
#include <utmp.h> // for utmp struct

#include "who1.h"

#define BUFFERSIZE  4096
#define NRECS       16
#define UTSIZE      (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE]; // storage
static int num_recs; // number stored
static int cur_rec; // next to go
static int fd_utmp = -1; // read from

int utmp_reload();

// opens the file. Returns -1 on error
int utmp_open(char *filename) {
    fd_utmp = open(filename, O_RDONLY);
    cur_rec = num_recs = 0; // no records yet
    return fd_utmp; // report
}

// returns pointer to the next struct. Returns NULL on EOF
struct utmp * utmp_next() {    
    struct utmp *recp;
    
    // checks if there is an error
    if (fd_utmp == -1) {
        return NULLUT;
    }
    
    // any more ?
    if (cur_rec == num_recs && utmp_reload() == 0) { 
        return NULLUT;
    }
    
    // get address of next record
    recp = (struct utmp *) &utmpbuf[cur_rec * UTSIZE]; 
    
    cur_rec++;
    return recp;
}

// reads the next bunch of records into the buffer
int utmp_reload() {
    int amt_read;
    
    // read them in
    amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);

    // the number of records that were read
    num_recs = amt_read / UTSIZE;

    // reset pointer
    cur_rec = 0;

    return num_recs;    
}

// closes the file
void utmp_close() {
    
    // do not close if its not open
    if (fd_utmp != -1) {
        close(fd_utmp);
    }
}
