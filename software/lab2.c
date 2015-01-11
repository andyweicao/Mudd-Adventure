/*
 * Userspace program that communicates with the led_vga device driver
 * primarily through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 
 */


#include <time.h>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>
#include "vga_led.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "fbputchar.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "usbkeyboard.h"


int vga_led_fd;
struct libusb_device_handle *keyboard;
int endpoint_address;


//place the example code below here:

#define mapWidth 47
#define mapHeight 35
#define texWidth 128
#define texHeight 128
#define w 480
#define h 640

#define random(x) (rand()%x)

int worldMap[mapWidth][mapHeight]=
{
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
  {1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0}
};



void write_screen(unsigned int data1[480],unsigned int data2[480])
{ 
  vga_led_screen screen;
  int i;
  for (i = 0 ; i < 480 ; i++) {
  screen.column = i;
  screen.data1 = data1[i];
  screen.data2 = data2[i];
 if (ioctl(vga_led_fd, VGA_LED_WRITE_SCREEN, &screen)) {
      perror("ioctl(VGA_LED_WRITE_SCREEN) failed");
      return;
    }
  }
}


int main()
{
  //vga_led_arg_t vla;
  //int i;
  static const char filename[] = "/dev/vga_led";

  //static unsigned char message[8] = { 0x39, 0x6D, 0x79, 0x79,
				//      0x66, 0x7F, 0x66, 0x3F };

  printf("VGA LED Userspace program started\n");

  if ( (vga_led_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

 struct usb_keyboard_packet packet;
  int transferred;
  char keystate[12];

 if ( (keyboard = openkeyboard(&endpoint_address)) == NULL ) {
    fprintf(stderr, "Did not find a keyboard\n");
    exit(1);
  }







  printf("initial state: ");

  double posX = 13.5, posY = 15.5;  //x and y start position
  double dirX = -1.0, dirY = 0.0; //initial direction vector
  double planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane
  
 
  int counttime=6000;
  int control =0;    // decide whether to shoot
  int wound = 0;
  int delay = 0;     // decide firing time 
  int randa = 5;  // whether meets with a boss	
  int scenario = 0;   // put into the hardware
  int gamereset = 0;

 while(counttime>0)
  {
    int x;
    unsigned int passdata[480];
    unsigned int passdata1[480];
    
    for(x = 0; x < w; x++)
    {
      
      //calculate ray position and direction 
      double cameraX = 2.0 * x/480.0-1.0; //x-coordinate in camera space
      double rayPosX = posX;
      double rayPosY = posY;
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;
      //which box of the map we're in  
      int mapX = (int)rayPosX;
      int mapY = (int)rayPosY;
       
      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;
       
       //length of ray from one x or y-side to next x or y-side
      double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
      double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

      double perpWallDist;
    
      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?
      //calculate step and initial sideDist
      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (rayPosX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (rayPosY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
      }
      //perform DDA
 // printf("hit\n");

      while (hit == 0)
      {
        //jump to next map square, OR in x-direction, OR in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if (worldMap[mapX][mapY] > 0) hit = 1;
      } 

      //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
      if (side == 0)
      perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
      else
      perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);
      
      //Calculate height of line to draw on screen

      int k=h/perpWallDist;
      int lineHeight = abs(k);
       
      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + h / 2;
      if(drawStart < 0)drawStart = 0;
      int drawEnd = lineHeight / 2 + h / 2;
      if(drawEnd >= h)drawEnd = h - 1;
        
      int color = 2;
       
      //give x and y sides different brightness
      if (side == 1) {color = color / 2;}
      if (worldMap[mapX][mapY]==2) {color = 3;}
      //calculate value of wallX
      double wallX; //where exactly the wall was hit
      if (side == 1) wallX = rayPosX + ((mapY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
      else       wallX = rayPosY + ((mapX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
      wallX -= floor((wallX));
       
      //x coordinate on the texture
      int texX = (int)(wallX * (double)(texWidth));
      if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
      if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;


      int lengthH = drawEnd - drawStart;
      int coff=0;
	  coff=(128*8192)/(lengthH);
      int texY = 0;
          texY = 128*texX;
	passdata[x]=(scenario<<22|gamereset<<29|control<<26|color<<20|texX<<10|drawStart);//change SH
	passdata1[x]=(coff<<14|texY);

}
  


    write_screen(passdata,passdata1);
//printf("passdata %320d\n", passdata);
	//printf("good\n");
/*
    int c;
    for (c = 0;c<480;c++)
    {
	printf("%d;",passdata[c]);
    }
*/
    
control = 0; 

  libusb_interrupt_transfer(keyboard, endpoint_address, (unsigned char *) &packet, 6,&transferred, 0);
 

   if ((transferred ==8 && (((packet.keycode[0]!= 0x7f || packet.keycode[1]!= 0x7f) && packet.keycode[0]!=0x80) ||      packet.keycode[5]!=0x0f)))

 {	
	
  	
	//printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x\n ", packet.keycode[0],packet.keycode[1],packet.keycode[2],packet.keycode[3],packet.keycode[4],packet.keycode[5],packet.keycode[6],packet.keycode[7]);

		wound = 0;
		gamereset = 0;
		control = 0;
	if((packet.keycode[1] == 0x00))
	{        int locationx1=(int)posX + dirX * 0.02;
	         int locationy1=(int)posY;
                 int locationx2=(int)posX;
	         int locationy2=(int)posY + dirY * 0.02;
		 if(worldMap[locationx1][locationy1] == 0){
		//posX=posX+((packet.keycode[1] == 0xff)-(packet.keycode[1] == 0x00)+(packet.keycode[5] == 0x04)-(packet.keycode[5] == 0x00))*0.01;}
                if (randa < 10){
			randa = random(100);
			if ( randa > 80){
				scenario = 0x01;}
			else if ( randa >  70){
				scenario = 0x02;}
			else if ( randa > 30){
				scenario = 0x03;}
			else if ( randa > 10){
				scenario = 0x04;}
			else {scenario = 0x00;}
		}
		else { randa = random(100); scenario = 0x00;}
		printf("%02x\n",scenario);
		posX = posX + dirX*0.02;}
		if(worldMap[locationx2][locationy2] == 0){
		posY = posY+ dirY * 0.02;
		}
		

	}
	if((packet.keycode[1] == 0xff))
	{        
	  scenario = 0x00;
		 int locationx1=(int)posX - dirX * 0.02;
	         int locationy1=(int)posY;
                 int locationx2=(int)posX;
	         int locationy2=(int)posY - dirY * 0.02;
		 if(worldMap[locationx1][locationy1] == 0){
		//posX=posX+((packet.keycode[1] == 0xff)-(packet.keycode[1] == 0x00)+(packet.keycode[5] == 0x04)-(packet.keycode[5] == 0x00))*0.01;}
		posX = posX - dirX*0.02;}
		if(worldMap[locationx2][locationy2] == 0){
		posY = posY - dirY * 0.02;
		}
	}
	if((packet.keycode[0] == 0x00))
	{
	  scenario = 0x00;
		//posX=posX+((packet.keycode[0] == 0xff)-(packet.keycode[0] == 0x00)+(packet.keycode[0] == 0x04)-(packet.keycode[0] == 0x00))*0.01;
		//posX=posX-0.01;
		
		double oldDirX = dirX;
      		dirX = dirX  - dirY * 0.01;
      		dirY = oldDirX * 0.01 + dirY;
      		double oldPlaneX = planeX;
      		planeX = planeX  - planeY * 0.01;
      		planeY = oldPlaneX * 0.01 + planeY;
		

                
	}

	
	if ((packet.keycode[0] == 0xff))
	{
		scenario = 0x00;
		double oldDirX = dirX;
      		dirX = dirX * cos(-0.01) - dirY * sin(-0.01);
      		dirY = oldDirX * sin(-0.01) + dirY * cos(-0.01);
      		double oldPlaneX = planeX;
      		planeX = planeX * cos(-0.01) - planeY * sin(-0.01);
      		planeY = oldPlaneX * sin(-0.01) + planeY * cos(-0.01);
		
		//posX=posX+0.01;
	}
	
	if ((packet.keycode[5] == 0x8f))
	{
		scenario = 0x00;
  		control=4;
		
		wound = 1;

		printf ("would %10d\n",wound);

	
	}
	
	if ((packet.keycode[5] == 0x1f)&&packet.keycode[4]==0x7f)
	{
		scenario = 0x00;
  		control=1;
		
		printf ("would %10d\n",wound);

		usleep(100000);

	}
	
	if ((packet.keycode[5] == 0x1f)&&packet.keycode[4]==0x80)
	{
		 posX = 13.5, posY = 15.5;  
   		dirX = -1.0, dirY = 0.0;
 		  planeX = 0.0, planeY = 0.66; 
  		gamereset = 1;

	}


	if ((packet.keycode[5] == 0x2f))
	{
		scenario = 0x00;
  		control=2;
		
		wound = wound + 1;

		printf ("would %10d\n",wound);

		usleep(100000);

	}

	if ((packet.keycode[5] == 0x4f))
	{
		scenario = 0x00;
  		control=3;
		
		wound = wound + 1;

		printf ("would %10d\n",wound);

		usleep(100000);

	}
			
}

	//else {printf("nothing happened\n");}
 //usleep(100000);

  	//posy=posy+((packet.keycode[0] == 0xff)-(packet.keycode[0] == 0x00)+(packet.keycode[5] == 0x02)-(packet.keycode[5] == 0x06))*0.0025;
	//posX=posX+((packet.keycode[1] == 0xff)-(packet.keycode[1] == 0x00)+(packet.keycode[5] == 0x04)-(packet.keycode[5] == 0x00))*0.01;



  }
  printf("VGA LED Userspace program terminating\n");
  return 0;
}
