/*  https://github.com/retrogav/UnderstandingUnixLinuxProgramming-Molay

    cp1.c version 0.1
    A version of the cp program
    Uses read and write with customizable buffer size

    Version history
    0.1 first ver

    To compile -
    cc cp1.c -o cp1          
*/

#include <stdio.h>
#include <unistd.h> // for read(), close()
#include <fcntl.h> // for open()
#include <stdlib.h> // for exit()

#define BUFFERSIZE  4096
#define COPYMODE    0644

void oops(char *s1, char *s2);

int main(int ac, char *av[]) {
    
    int in_fd;
    int out_fd;
    int n_chars;
    char buf[BUFFERSIZE];

    // check args
    if (ac != 3) {
        fprintf(stderr, "usage: %s source destination\n", *av);
        exit(1);
    }
    
    // open files
    if ((in_fd = open(av[1], O_RDONLY)) == -1) {
        oops("Cannot open", av[1]);
    }
    
    if ((out_fd = creat(av[2], COPYMODE)) == -1) {
        oops("Cannot creat", av[2]);
    }    

    // copy files. read() returns 0 when the end of the file is reached
    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)  {
        if (write(out_fd, buf, n_chars) != n_chars) {
            oops("Write error to", av[2]);
        }
    }

    if (n_chars == -1) {
        oops("Read error from", av[1]);
    }

    // close files
    if (close(in_fd) == -1 || close(out_fd) == -1) {
        oops("Error closing files", "");
    }

    return 0;
}

void oops(char *s1, char *s2) {
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}
