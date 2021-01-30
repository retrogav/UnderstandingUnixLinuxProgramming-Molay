/*  https://github.com/retrogav/UnderstandingUnixLinuxProgramming-Molay

    more01.c version 0.2
    read and print 24 lines then pause for a few special commands

    Version history
    0.1 first ver
    0.2 changed user input to read from /dev/tty (keyboard) instead of from
        the same stream as the data

    To compile-
    cc more01.c -o more01        
*/

#include <stdio.h>
#include <stdlib.h> // for exit()

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *fp);
int see_more(FILE *cmd);

int main(int ac, char *av[]) {
    FILE *fp;
    if (ac == 1) {
        do_more(stdin);
    }
    else {
        while (--ac) {
            if ((fp = fopen(*++av, "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            }
            else {
                exit(1);
            }
        }
    }
    return 0;
}

// read PAGELEN lines, then call see_more() for further instructions
void do_more(FILE *fp) {
    char line[LINELEN];
    int num_of_lines = 0;    
    int reply;
    FILE *fp_tty;

    fp_tty = fopen("/dev/tty", "r");                // command stream
    if (fp_tty == NULL) {                           // if open fails    
        exit(1);
    }

    while (fgets(line, LINELEN, fp)) {              // more input    
        if (num_of_lines == PAGELEN) {              // full screen?        
            reply = see_more(fp_tty);               // pass the file
            if (reply == 0) {                       // user typed q            
                break;
            }
            num_of_lines -= reply;                  // reset count
        }
        if (fputs(line, stdout) == EOF) {           // show line        
            exit(1);                                // or die
        }
        num_of_lines++;                             // count it
    }
}

/*  print message, wait for response, return # of lines to advance
    q means no, space means yes, CR means one line
*/
int see_more(FILE *cmd) {
    int c;

    printf("\033[7m more? \033[m");                 // reverse on a vt100
    while ((c = getc(cmd)) != EOF) {                // reads from tty    
        if (c == 'q') {
            return 0;
        }
        if (c == ' ') {                             // space shows next page        
            return PAGELEN;                         // how many to show
        }
        if (c == '\n') {                            // enter key shows 1 line        
            return 1;
        }
    }
    return 0;
}