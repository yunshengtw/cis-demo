CC = arm-none-linux-gnueabi-gcc

all: send cis-driver

cis-driver: cis-driver.c
	$(CC) cis-driver.c -std=c99 -static -o cis-driver

send: send.c
	$(CC) send.c -std=c99 -static -o send

upload-cis-driver: cis-driver
	scp cis-driver root@10.1.1.11:~/cis-driver

upload-bit: system.bit
	scp system.bit root@10.1.1.11:~/system.bit

download-pic:
	scp -r root@10.1.1.11:~/pic ./

clean:
	rm -rf send
.PHONY: clean
