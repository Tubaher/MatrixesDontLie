/************************ top_eye_flop.cpp **********************************
Name:         CHAPLIN Shakira.cpp
codigo fuente base:   prof.    oscar  chang
August  2018
/*********************************************************************/
//---------   GLOBALs
float copy[10000][3];  //    matriz de las imagenes de entrada [numero de pixels] [numero de imagenes]
int picture_video;

float cutter;
float energy_band;
float energy_average;
int b_flag;
int blue_column;


int x_roi;     //   coordenadas de la posicion de la camara o roi, peque�a zona donde capturamos pixels 
int y_roi;     //

int x_discharge_image;  //  coordenadas de la zona donde descargamos la imagen a ser analizada
int y_discharge_image;
int hidden_pointer; 

int winner_x, winner_y;

int x_barrido, y_barrido;
//------------------------------------------
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#include "neural_lib.H"              // ----      codigo en C/C++ para manejar redes de neuronas
#include "ROI_helipad.h"
#include "plot_items.h"
#include "load_binary_images.h"
#include "load_save_weights.h"

using namespace std;


/******************* variables locales  ***************************************/
int  winner;           // contadores genericos
int  x,y;
int  xd,yd,alfa;
char color;
char key;
int trace_flag;

int stop;
int picture_video_flag;
int plot_counter;
//-------------------------------------
//    inicia el modo grafico             
//-------------------------------------    
void grafico(void)
{
    int    GraphDriver;  /* The Graphics device driver */
    int    GraphMode;    /* The Graphics mode value */
    int    ErrorCode;    /* Reports any graphics errors */
   
    initgraph(&GraphDriver, &GraphMode, "");

    ErrorCode = graphresult();            /* Read result of initialization*/
    if( ErrorCode != grOk )              /* Error occured during init    */
    {
       printf(" Graphics System Error: %s", grapherrormsg( ErrorCode ) );
       exit( 1 );
    }
}
//----------------------------------------------------
void procesa_imagen(void)
{
   
            capture_roi_imagen();       //  capture_imagen carga las entradas de la red
            plot_net_image_input(); 
            Gain=0.5;    
            calcular_capa_escondida();
            Gain=1.5;    
            calcular_capa_salida(); 
}
//----------------------------------------------------------------
void get_winners(void)              //                                                            CALCULAR BLUE_COLUMN     !!
{
  int i,j;
  float max;
 
 
    j=0;
    max=c_salida.out[0];
    
    for (i=0; i<N_OUT-11; i++)
        {                            
          if (c_salida.out[i]>max)
           {
            max  = c_salida.out[i];
            j= i;
           }        
        }
    winner_x=j;
	
//--------------------------
	j=11;
    max=c_salida.out[11];
    
    for (i=11; i<N_OUT; i++)
        {                            
          if (c_salida.out[i]>max)
           {
            max  = c_salida.out[i];
            j= i;
           }        
        }
    winner_y=j;
	
} 
//----------------------------------------------
void plot_garbage(void)              //                         
{
 int i,j,k,x,y,radio;   
   for(i=0;i<500;i++)
    {
     
      x=random(600);
      y=random(400); 
      radio=random(25); 
      setcolor(RED);      
      circle(x,y,radio);
      //floodfill(x,y,RED);    
    }    
   for(i=0;i<500;i++)
    {
      x=random(600);
      y=random(400); 
      radio=random(25); 
      setcolor(RED);      
      bar(x,y,x+radio,y+radio+random(7)) ;
      //floodfill(x,y,RED);    
    }    
} 
//---------------------------------------------------------------
void move_roi_target(void)
{
 int i,temp;
   for(i=0;i<N_OUT;i++) Target[i]=0.1; 
       
   temp=random(11);
   Target[temp]=0.9;
   temp=temp-5;
   x_roi=320+temp;      /////   320 y 270 son las coordenadas donde se ubica la ROI  
   
   temp=random(11);
   Target[temp+11]=0.9;
   temp=temp-5;
   y_roi=270+temp;    
           
} 

//--------------------------------
void scan_roi(void)       //  Explore environtment with your eye, add periferic vision 
{
  // using namespace std;
 
      x_roi=x_roi+3;  
      if(x_roi>450)  {x_roi=170;y_roi=y_roi+3;plot_features();}   
      if(y_roi>280)  {x_roi=370;y_roi=250;}  
      //printf("x_roi= %i \n",x_roi);
      //printf("y_roi= %i \n",y_roi);
      //cout<<y_roi<<endl;
      //x_roi=320;
      //y_roi=270;
      
}
//--------------------------------
void barrido_roi(void)
//  Explore environtment with your eye, add periferic vision 
{
   using namespace std;
 
      x_barrido=x_barrido+30;  
      if(x_barrido>500)  {x_barrido=20;y_barrido=y_barrido+30;}   
      if(y_barrido>300)  {x_barrido=20;y_barrido=20;}  
     
      printf("x_barrido= %i \n",x_barrido);
      printf("y_barrido= %i \n",y_barrido);
      //cout<<y_roi<<endl;
      //x_roi=320;
      //y_roi=270;
      
}

//--------------------------------------------------------------
void feed_back_roi(void) 
{
 int diff;   
    
        diff=-winner_x+5;//   winner_x entre 0 y 10
        x_roi=x_roi+diff;  
        y_roi=y_roi-(winner_y-11)+5;  //   winner_y entre 11 y 21
		
        //printf("winner_x = %i \n",winner_x);
        //printf("winner_y = %i \n",winner_y);
        //printf("y_roi= %i \n",y_roi);
}
//--------------------------------------------------------------

void train(void)  //                                                  TRAIN                          ***************                                                                                                                      TRAIN  !!
{
int slow, plot_flag; 
int roi_move_count=0;
    
   while(1)
    {  
     //set_targets();
        
     //if(b_flag) move_roi_target(); 
              
    //move_roi_target();   
    procesa_imagen(); 
    get_winners();
      
    if(roi_move_count) feed_back_roi();   
      
     //if(b_flag)corregir_pesos();              //    BACK PROPAGATION
 
    
        
     if(roi_move_count==0) {x_roi=x_barrido;y_roi=y_barrido;} // salto forzado al carril
     
     if(c_salida.out[winner_x]<0.7)  plot_flag=1;   
         
     if((c_salida.out[winner_x]>0.7)&&(c_salida.out[winner_y]>0.7))
           { 
            printf("Shakira ! \n");
            if(plot_flag)
                  {                
                   plot_loaded_pictures();    
                   plot_square();
                   plot_flag=0;    
                   
                   Beep(2345,100);  
                   Sleep(500);     
                  }                     
            //
           }
     //plot_counter++;     
     if(plot_counter>11)
         {   
          plot_counter=0;   
          //plot_net_image_input(); 
          //plot_features();
         }
     slow++;
         
     if(!roi_move_count) barrido_roi();    
     plot_hidden_outputs();  
     plot_outputs();    
     plot_targets();
     
     picture_video++;
     if(picture_video>2) picture_video=0;
         
     roi_move_count++;
     if(roi_move_count>10){roi_move_count=0;} 
     
     if(stop)break;
     if(kbhit()) break;
     
         
    }
     plot_hidden_outputs();   
     plot_features();
  
}    
//===================================================================================================
/* void plot_giant_cell(void)                                                    // PLOT GIANT CELL
{
 int i,j,k,sx,sy,counter, scale; 
float timp; 

//y=y_giant; 
//x=x_giant;
scale=3;
//sx=x_giant;    
   counter=0;
   k=hid_pointer; 
    {   
     for(j=0;j<100;j++)  // 
     { 
      sy = y_giant+j*scale;       
      for(i=0;i<100;i++)
         {    
          sx = x_giant+i*scale;
          timp=c_escondida.pesos[k][counter];   
          //timp= Entrada[counter];  
          color=WHITE;   
          if(timp<-0.001) color=BLUE;        // fROZEN
          if(timp>0.001) color=RED;         // HOT !     
          setcolor(color); 
          setfillstyle(SOLID_FILL,color); 
          bar(sx,sy,sx+scale,sy+scale) ;
          //putpixel(sx,sy, color);      
          counter++;               
         }
     } 
         
    }
} */
//------------------------------------


void main(void)
{ 
 int i;  
    x=xd=300;
    y=yd= 430;
    alfa=90;                  //     posicion inicial de la camara
    clrscr();                   // limpia la pantalla
    grafico();                   // prepara modo grafico
    cleardevice();               // lkimpia la pantalla windows
    floodfill(x,y,WHITE);
    //randomize();                 // inicia random
    inicializar_pesos();
    setcolor(DARKGRAY);
    plot_garbage();
    trace_flag=0;
   
    CargarImagenes();
            
    for(i=0;i<N_OUT;i++) Target[i]=0.1;
        
    //prepare_scene();
   
    trace_flag=1;
    cutter=0.5;                 // 18    unbral para B&W
    plot_loaded_pictures();   
  
    energy_band=0.045        ;
   
    picture_video_flag=1;
    stop=0;  
    b_flag=0; 
    picture_video=0;    //  [picture_video]d
    x_roi=170;
    y_roi=250;
    
    x_barrido=120;
    y_barrido=230;   
    
    //x_barrido=20;
    //y_barrido=20;   
   
    hidden_pointer=3; 
    CargarPesos();
    train();   
    
    do {
       //play_flag=0; 
       key=getch();
       switch (key) {
                       
            
         case '1':              { 
                                   erase_square();
                                   x_roi=x_roi-10; 
                                   plot_loaded_pictures(); 
                                   plot_square();
                                   train(); 
                                } 
           break;                                
  
           case '2':            {
                                   erase_square();
                                   x_roi=x_roi+10; 
                                   plot_loaded_pictures(); 
                                   plot_square();
                                   train(); 
                                }
           break;    

           case 'a':              { 
                                   erase_square();
                                   y_roi=y_roi+10;  
                                   plot_square();
                                   plot_loaded_pictures();
                                   train(); 
                                } 
           break;                                
  
           case 'q':            {
                                   erase_square();
                                   y_roi=y_roi-10; 
                                   plot_square();
                                   train(); 
                                }
           break;   
                                
                                
                                
                                
                                
           
           case 'r':            { 
                                   inicializar_pesos();
                                   trim_hidden_weights();
                                   train(); 
                                } 
           break;                                
  
           case 'b':            {
                                  b_flag=b_flag^1;
                                  train();
                                }
           break;              
     
           case 's':           {
                                SalvarPesos();
                                 train();
                                }
	       break;                                                  
           case 'c':           {
                                CargarPesos();
                                train();
                               }
	       break;    
          
           case 'p':           {
                                stop=1;
                                train();                              
                               }
	       break;    
           
           case 'g': case 'G': { 
                                picture_video_flag=0;                 
                                train();
                                }   
            break;                    
               
            case 'h': case 'H': { 
                                 picture_video_flag=1; 
                                 //erase_screen();
                                 train();
                                }  
                                
            case ',':          { 
                                cutter=cutter+0.01; 
                                plot_loaded_pictures();                
                                train();
                                }   
            break;                    
               
            case '.':          { 
                                cutter=cutter-0.01; 
                                plot_loaded_pictures();                
                                train();
                               }                      
 
           case ' ':             {                      // ---------    HERE 
                                   stop=stop^1;
                                   train();
                                 }                                     
           break;                      
                        
                               
         }
   } while ((key!='x')&&(key!='X'));

   closegraph();
   clrscr();
}

//---------------------------------------
/*
BLACK        �  0  � Yes � Yes
BLUE         �  1  � Yes � Yes
GREEN        �  2  � Yes � Yes
CYAN         �  3  � Yes � Yes
RED          �  4  � Yes � Yes
MAGENTA      �  5  � Yes � Yes
BROWN        �  6  � Yes � Yes
LIGHTGRAY    �  7  � Yes � Yes
DARKGRAY     �  8  � No  � Yes
LIGHTBLUE    �  9  � No  � Yes
LIGHTGREEN   � 10  � No  � Yes
LIGHTCYAN    � 11  � No  � Yes
LIGHTRED     � 12  � No  � Yes
LIGHTMAGENTA � 13  � No  � Yes
YELLOW       � 14  � No  � Yes
WHITE        � 15  � No  � Yes

----------------------------------------------------------------
     if(x<0)    { x=0; if( heat<100) heat++;}     
     if(y<35)    { y=35; if( heat<100) heat++;}  
     if(x>640) { x=640; if( heat<100) heat++;}  //  x>1200 : full pantalla 
     if(y>470) { y=470; if( heat<100) heat++;}   // y>720 : full pantalla 
         
//--------------------------------------------------

************************ basic_robot.cpp ***************************
desarrollado usando Dev-C++ IDE V4.9.8.4 GNU / MinGW / GNU gcc
                    Sc1 IDE / BCC5.5
                    version orginal por Oscar Chang
*********************************************************************/

