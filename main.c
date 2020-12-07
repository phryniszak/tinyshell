#include "shell/shell.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////

// https://www.cmrr.umn.edu/~strupp/serial.html

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

int fd;

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */
int open_port(char *portname)
{
    int fd; // File descriptor for the port

    fd = open(portname, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        // Could not open the port.
        perror("open_port: Unable to open /dev/ttyf1 - ");
    }
    else
    {
        // The FNDELAY option causes the read function to return 0 if no characters are available on the port.
        // To restore normal (blocking) behavior, call fcntl() without the FNDELAY option:
        fcntl(fd, F_SETFL, 0);
    }

    return (fd);
}

///////////////////////////////////////////////////////////////////////////////

int console_putc(char ch)
{
    if (ch == '\n')
    {
        printf("\tout: \\n (%02X)\n", ch);
    }
    else if (ch == '\r')
    {
        printf("\tout: \\r (%02X)\n", ch);
    }
    else
    {
        printf("\tout: %c (%02X)\n", ch, ch);
    }

    return write(fd, &ch, 1);
}

int main(void)
{
    setbuf(stdout, NULL);

    char *portname = "/dev/pts/2";
    fd = open_port(portname);

    if (fd < 0)
    {
        return -1;
    }

    sShellImpl shell_impl = {
        .send_char = console_putc,
    };
    shell_boot(&shell_impl, true);

    while (true)
    {
        char ch;
        size_t nbytes = read(fd, &ch, 1);

        if (nbytes < 1)
        {
            close(fd);
            perror("read from serial port returned EOF");
            return (-1);
        }

        if (ch == '\n')
        {
            printf("in: \\n (%02X)\n", ch);
        }
        else if (ch == '\r')
        {
            printf("in: \\r (%02X)\n", ch);
        }
        else
        {
            printf("in: %c (%02X)\n", ch, ch);
        }

        shell_receive_char(ch);
    }

    close(fd);
}