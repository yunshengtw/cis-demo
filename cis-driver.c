#define _BSD_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define ADDR_CONTROL   0x43C00000

#define MAP_SIZE 0x10000
#define MAP_MASK (MAP_SIZE-1)
/* measured in ms */
#define APPROX_FRAME_TIME 100

//global variable
unsigned char pic[129][129] = {};
int device_pointer;

//used variable
void *mapped_base;
off_t base = ADDR_CONTROL;

//function definition
void device_init(void);
void set_phy_addr(void);
void set_enable(void);
void set_disable(void);
unsigned int get_trigger_val(void);
void set_read_mode(void);
void set_addr(unsigned int i, unsigned int j);
unsigned int get_pixel(void);
void write_ppm(void);

static int socket_init(char *host, int port)
{
    int fd;
    struct sockaddr_in server_addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == 0) {
        perror("socket failed");
        exit(1);
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &server_addr.sin_addr) < 0) {
        perror("inet_pton error");
        exit(1);
    }
    if (connect(fd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect error");
        exit(1);
    }

    return fd;
}

int main(int argc, char *argv[])
{
    //volatile unsigned int *controller = (volatile unsigned int*) 0x43C00000;
    //controller[0] = {22'bX, read_mode, enable}
    //controller[1] = {CPU_addr_n, CPU_addr_c}
    //controller[2] = {24'b0, dout}
    //controller[3] = {31'b0, TRIG}
    //the rest of four register is for dwbug(unused)
    char *host;
    const int port = 8887;
    int fd_socket;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./cis-driver <ip>\n");
        exit(1);
    }

    host = argv[1];
    //printf("Socket initilized.\n");

    printf("device init\n");
    device_init();

    printf("set phy addr\n");
    set_phy_addr();

    int frame_num = 0;
    struct timeval start, end;
    int tv_socket = 0;
    while(1)
    {
        gettimeofday(&start, NULL);

        set_enable();
        //usleep(1);
        set_disable();        
        while(get_trigger_val() == (unsigned int)0);

        usleep((APPROX_FRAME_TIME - 20) * 1000);
        
#if 1
        set_enable();
        //usleep(1);
        set_disable();        
        while(get_trigger_val() == (unsigned int)0);
#endif

        //open read mode to fetch data    
        //printf("set_read_mode\n");     
        set_read_mode();
        //usleep(1);

        unsigned int i,j;
        for(i=0; i<129; i++)
        {
            for(j=0; j<129; j++)
            {
                set_addr(i,j);
                //usleep(1);
                pic[i][j] = get_pixel();
            }
            
        }
        gettimeofday(&end, NULL);
        printf("%d ms\n", (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000);

        gettimeofday(&start, NULL);
        fd_socket = socket_init(host, port);
        write(fd_socket, pic, sizeof(pic));
        close(fd_socket);
        gettimeofday(&end, NULL);
        tv_socket = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
        printf("%d socket\n", tv_socket);
#if 0
        int byte_sent = 0;
        while (byte_sent < sizeof(pic)) {
            int amount = 1024;
            if (sizeof(pic) - byte_sent < 1024)
                amount = sizeof(pic) - byte_sent;
            send(fd_socket, pic + byte_sent, amount, 0);
            byte_sent += amount;
        }
#endif

        //write_ppm() ;
    }
    return 0;
}

void write_ppm(void)
{
    static int cnt;
    char fname[64];
    sprintf(fname, "pic/cis_%d.ppm", cnt++);
    FILE* fp = fopen(fname, "wb");
    
    
    //write the first line of header
    int height = 129;
    int width = 129; 

    fprintf(fp, "P6\n%i %i 255\n", width, height);
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            fputc((char)pic[y][x], fp);         //red
            fputc((char)pic[y][x], fp);         //green
            fputc((char)pic[y][x], fp);         //blue
        }
    }
    fclose(fp);
}

void device_init(void)
{
    device_pointer = open("/dev/mem", O_RDWR | O_SYNC);
    
    if(device_pointer < 0)
    {
        printf("device file open error !\n");
        exit(0);
    }
    printf("/dev/mem opened \n");
}

void set_phy_addr(void)
{
    mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, device_pointer, base);
}

void set_enable(void)
{
    *(volatile unsigned int*) (mapped_base) = 0x00000001;
} 

void set_disable(void)
{
    *(volatile unsigned int*) (mapped_base) = 0x00000000;
}

unsigned int get_trigger_val(void)
{
    return *(volatile unsigned int*) (mapped_base + 0x0c);
}

void set_read_mode(void)
{
    *(volatile unsigned int*) (mapped_base) = 0x00000002;
}

void set_addr(unsigned int i, unsigned int j)
{
    *(volatile unsigned int*) (mapped_base + 0x04) = (i<<8) | j;
}

unsigned int get_pixel(void)
{
    return *(volatile unsigned int*) (mapped_base + 0x08);
}

