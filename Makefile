CC = arm-none-linux-gnueabi-gcc
IP_ADDR ?= 10.1.1.11

all: send cis-driver

cis-driver: cis-driver.c
	$(CC) cis-driver.c -std=c99 -static -o cis-driver

send: send.c
	$(CC) send.c -std=c99 -static -o send

upload-cis-driver: cis-driver
	scp cis-driver root@$(IP_ADDR):~/cis-driver

upload-bit: system.bit
	scp system.bit root@$(IP_ADDR):~/system.bit

download-pic:
	scp -r root@$(IP_ADDR):~/pic ./
	rm -rf pic

clean:
	rm -rf send
.PHONY: clean
