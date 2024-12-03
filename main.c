#include <gb/gb.h>
#include <stdint.h>
//#include "one.h"
#include "Numbers.h"
#include "spr.h"

uint8_t p_x=0,p_y=0;
uint8_t joypadCurrent=0,joypadPrevious=0;
const unsigned char PLUS_OPERATOR = 0, MINUS_OPERATOR = 2;
const unsigned char GRID_X=4,GRID_Y=6;
const unsigned char x_offset=36,y_offset=60;
uint8_t grid_numbers[16];
uint8_t rng_seed = 4;
uint8_t player_number = 0;


const uint8_t rng_table[] =
    {186,87,80,92,101,170,61,213,96,180,2,244,9,122,17,176,156,44,90,116,111,91,158,220,251,20,27,49,18,208,219,47
    ,3,69,173,0,193,182,50,226,142,203,79,128,121,147,233,71,130,57,162,144,73,115,194,210,167,150,255,58,216,238,189,5
    ,93,85,104,231,139,98,23,222,171,151,103,54,83,129,35,41,157,59,212,32,249,132,45,15,163,51,60,209,53,195,215,26
    ,108,105,77,82,202,140,135,29,110,181,100,76,75,243,232,112,236,52,155,240,14,221,201,42,70,66,48,94,97,184,205,187
    ,224,168,21,141,63,119,4,164,86,245,125,62,177,131,10,241,36,191,160,88,248,196,55,161,175,81,218,253,11,43,127,65
    ,84,153,78,72,190,145,143,154,74,137,206,31,199,178,183,89,16,38,134,107,207,24,109,39,95,117,149,172,204,211,165,126
    ,252,123,166,146,118,228,242,133,138,246,8,254,217,250,25,114,198,185,227,113,237,169,148,188,136,99,192,152,40,229,247,200
    ,12,230,235,239,30,67,214,37,34,159,56,106,33,225,120,197,46,1,22,223,19,102,234,28,64,179,68,174,6,13,7};
uint8_t rand(void)
{
    rng_seed++;
    return rng_table[rng_seed];
}

void drawTileNumber(int x,int y,int number)
{
    set_bkg_tile_xy(0+x, 0+y, 0+number*4);
    set_bkg_tile_xy(0+x,1+y,1+number*4);
    set_bkg_tile_xy(1+x, 0+y,2+number*4);
    set_bkg_tile_xy(1+x,1+y,3+number*4);
}

void drawPlayerNumber(void)
{
    drawTileNumber(0,0,player_number/10);
    drawTileNumber(2,0,player_number%10);
}

// operatorData Unused[0000]Highlight[0]OperatorType[00]Horizontal[0]
void drawOperator(int x,int y,uint8_t operatorData)
{
    uint8_t operatorTile = 0;
    if(operatorData & MINUS_OPERATOR)  operatorTile = 44;
    else operatorTile = 45;
    if(operatorData & 8) operatorTile += 64;
    if(operatorData & 1)
    {
        operatorTile += 4;
        set_bkg_tile_xy(x,y,operatorTile);
        set_bkg_tile_xy(x,y+1,operatorTile+2);
    }
    else
    {
        set_bkg_tile_xy(x,y,operatorTile);
        set_bkg_tile_xy(x+1,y,operatorTile+2);
    }
}
void draw_grid(void)
{
    for(uint8_t i = 0; i < 16; i++){
        uint8_t x = GRID_X+3 *(i&3);
        uint8_t y = GRID_Y+3*(i>>2&3);
        drawTileNumber(x,y,grid_numbers[i]&0x1F);
        if (i <12) drawOperator(x,y+2, 0+(grid_numbers[i]>>3&8));
        if ((i&3) !=3) drawOperator(x+2,y,1+(grid_numbers[i]>>2&8));
    }
}
void init_gfx(void) {
    // Load Background tiles and then map
    
    set_bkg_data(0, 120u, number_tileset);

    set_sprite_data(0, 2, sprites);
    set_sprite_tile(0, 0);
    set_sprite_tile(1,1);
    set_sprite_tile(2,1);
    set_sprite_tile(3, 0);
  //  move_sprite(0,80,80);
  //  set_bkg_1bpp_data(0, 36, number_tileset);
    init_bkg(40);
    for(uint8_t i = 0; i < 16; i++){
        grid_numbers[i] = 1+(rand()%9);
    }
    draw_grid();
    
    SHOW_BKG;
    SHOW_SPRITES;
}


void main(void)
{
    init_gfx();
    // Loop forever
    while(1) {
        // Game main loop processing goes here
        joypadPrevious = joypadCurrent;
        joypadCurrent=joypad();

        if(joypadCurrent & J_A)
        {
            uint8_t old_pos = p_x+p_y*4;

            if(joypadCurrent & J_RIGHT && !(joypadPrevious & J_RIGHT) && p_x != 3)
            {
                grid_numbers[old_pos] = grid_numbers[old_pos] | 0x20;
                p_x++;
                player_number += grid_numbers[old_pos+1]& 0x0F;
            }
            if(joypadCurrent & J_LEFT&& !(joypadPrevious & J_LEFT) && p_x != 0)
            {
                grid_numbers[old_pos-1] = grid_numbers[old_pos-1] | 0x20;
                p_x--;
                player_number += grid_numbers[old_pos-1]& 0x0F;
            }
            if(joypadCurrent & J_UP&& !(joypadPrevious & J_UP) && p_y != 0)
            {
                grid_numbers[old_pos-4] = grid_numbers[old_pos-4] | 0x40;
                p_y--;
                player_number += grid_numbers[old_pos-4]& 0x0F;
            }
            if(joypadCurrent & J_DOWN&& !(joypadPrevious & J_DOWN)&& p_y != 3)
            {
                grid_numbers[old_pos] = grid_numbers[old_pos] | 0x40;
                p_y++;
                player_number += grid_numbers[old_pos+4]& 0x0F;
            }

            uint8_t pos = p_x+p_y*4;
            
            if(player_number == 0)
                player_number += grid_numbers[old_pos];
            drawPlayerNumber();
            grid_numbers[pos] = grid_numbers[pos] | 0x10;
            draw_grid();
        }
        else
        {
            if(joypadCurrent & J_RIGHT && !(joypadPrevious & J_RIGHT))p_x++;
            if(joypadCurrent & J_LEFT&& !(joypadPrevious & J_LEFT))p_x--;
            if(joypadCurrent & J_UP&& !(joypadPrevious & J_UP))p_y--;
            if(joypadCurrent & J_DOWN&& !(joypadPrevious & J_DOWN))p_y++;
            // Sets so it don't go out of bound and wrap in grid
            p_x = p_x & 3;
            p_y = p_y & 3;
            if(joypadPrevious & J_A)
            {
                // TODO add check if correct number and remove only the removed number if not selected
                for(uint8_t i = 0; i < 16; i++){
                    grid_numbers[i] = grid_numbers[i]& 0x0F;
                }
                player_number = 0;
            }
        }
    //    uint8_t p_x= pos;
    //    uint8_t p_y= pos>>2&3;

        move_sprite(0,24*p_x+x_offset,24*p_y+y_offset);
        move_sprite(1,24*p_x+16+x_offset,24*p_y+y_offset);
        move_sprite(2,24*p_x+x_offset,24*p_y+16+y_offset);
        move_sprite(3,24*p_x+16+x_offset,24*p_y+16+y_offset);
        //move_sprite(0,playerX*24,playerY*24);

		// Done processing, yield CPU and wait for start of next frame
        vsync();
    }
}
