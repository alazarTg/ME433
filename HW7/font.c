#include "font.h"

void drawChar(unsigned int x, unsigned int y, unsigned char character){
    
    unsigned int ascii_value = (int)(character);

    for(int i=0;i<5;i++){
        int arr = (int)ASCII[ascii_value-32][i];
        for (int j=0;j<8;j++){
            if((arr >> j) & 0b1 == 0b1){
                ssd1306_drawPixel(x+i,y+j,1);
            }
            else{
                ssd1306_drawPixel(x+i,y+j,0);
            }
        }
    }
}

void drawString(unsigned int x, unsigned int y, char* message){
    int index = 0;
    while(message[index] != '\0'){
        drawChar(x,y,message[index]);
        if (x < 125){ 
            x += 5;
        }
        else if((x >= 125)){
            y += 9;
            x = 0;
        }
        index++;
    }
}