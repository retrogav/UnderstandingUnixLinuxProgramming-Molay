/*  https://github.com/retrogav/UnderstandingUnixLinuxProgramming-Molay

    logout_tty.c version 0.1
    Marks a utmp record as logged out. Does not blank the username or
    remote host.
    Returns -1 on error, 0 on success.
    
    Note: On my system I get /var/run/utmp: Permission denied
    I am not going to elevate permisions to fully test this program.
    It looks like it will do what it needs to anyway and real reason for the
    program is just to demostrate the use of -
        * opening a file for read/write
        * find a line in the file
        * the use of lseek and write 
    
    Version history
    0.1 first ver
*/

#include <stdio.h>
#include <utmp.h> // for utmp struct
#include <fcntl.h> // for open()
#include <unistd.h>  // for read(), close()
#include <string.h> // for strncmp()
#include <time.h>  // for time()

int logout_tty(char *line);

int main(int argc, char* argv[]) {

    int logout = logout_tty(argv[1]);
    printf("logout_tty() returned: %d\n", logout);

    return 0;
}

int logout_tty(char *line) {    
    int fd;
    struct utmp rec;
    struct timeval tv; // added code. for use within gettimeofday(). 
    int len = sizeof(struct utmp);
    int retval = -1; // assume an error

    if ((fd = open(UTMP_FILE, O_RDWR)) == -1) {
        perror(UTMP_FILE);
        return -1;
    }
    
/* note on utmp and time(). Originally tried using time() to set the time to
ut_tv.tv_sec in the while loop below. time() expects a type time_t. 
But ut_tv.tv_sec in struct utmp is int32_t . Because of this the utmp man page
recommends the use of gettimeofday() instead of time().

*/
    // search and replace
    while (read(fd, &rec, len) == len) {
        if (strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == 0) {
            rec.ut_type = DEAD_PROCESS; // set type
            //if (time(&rec.ut_time) != -1) // problem code. set time
            if ((gettimeofday(&tv, NULL)) != -1) { // added code. set time
                rec.ut_tv.tv_sec = tv.tv_sec; // added code                       
                if (lseek(fd, -len, SEEK_CUR) != -1) { // back up
                    if (write(fd, &rec, len) == len) { // update
                        retval = 0; // success
                    }
                }
                break;
            }
        }
    }
    
    if (close(fd) == -1) {
        retval = -1;
    }
    
    return retval;
}