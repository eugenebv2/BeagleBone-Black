#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void main()
{
	int i = 0;
	int f = 0;
	f=open("/sys/class/gpio/gpio50/direction", O_RDWR);
	write(f,"out",3);
	close(f);

	f=open("/sys/class/gpio/gpio50/value",O_WRONLY);
	for(i=1;i<=20;i++){
		write(f,"1",1);
		sleep(1);
		write(f,"0",1);
		sleep(1);
	}
	close(f);
	printf("LED test program ended.\n\r");
}
