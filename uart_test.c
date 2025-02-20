#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main (void)
{
  int file;
  char buf[20];
  size_t nbytes;
  ssize_t bytes_written;
  struct termios oldtio, newtio;

  if ((file = open("/dev/ttyS1", O_RDWR | O_NOCTTY))<0)
  {
      printf("UART: Failed to open the file.\n");
      return;
  }

//save the current serial settings
  tcgetattr(file, &oldtio);

//clear the structure for new port settings
  bzero(&newtio, sizeof(newtio));

  newtio.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
  newtio.c_iflag = IGNPAR | ICRNL;

  tcflush(file, TCIFLUSH);
  tcsetattr(file, TCSANOW, &newtio);

  strcpy(buf, "This is a test\r\n");
  nbytes = strlen(buf);

  while (1)
  {
     bytes_written = write(file, buf, nbytes);
     sleep(10);
  }
  close(file);
}
