CC = arm-none-linux-gnueabi-gcc

all: send cis-driver

cis-driver: cis-driver.c
	$(CC) cis-driver.c -std=c99 -static -o cis-driver

send: send.c
	$(CC) send.c -std=c99 -static -o send

clean:
	rm -rf send
