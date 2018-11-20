/************************ top_eye_flop.cpp **********************************
Name:         CHAPLIN Shakira.cpp
codigo fuente base:   prof.    oscar  chang
August  2018
/*********************************************************************/
//---------   GLOBALs
float copy[10000][4];  //    matriz de las imagenes de entrada [numero de pixels] [numero de imagenes]
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
//------------------------------------------
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
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
//--------------------------------------------------
void busca_neuron_ganador(void)              //                                                                               BUSCA WINNER  !!
{
  int i;
  float max;
               
    winner_x=0;
    winner_y=0;
    
    max=c_salida.out[0];
    
    for (i=0; i<11; i++)
        {                            
         if (c_salida.out[i]>max)
           {
            max  = c_salida.out[i];
            winner_x= i;
           }        
        } 
      
      max=c_salida.out[11];
      for (i=0; i<11; i++)
        {                            
         if (c_salida.out[i+11]>max)
           {
            max  = c_salida.out[i+11];
            winner_y= i;
           }        
        } 
}
//---------------------------------------------------------------
void move_roi_target(void)
{
 int i,temp;
   for(i=0;i<N_OUT;i++) Target[i]=0.1; 
       
   temp=random(11);
   //temp=saw_x;       
   Target[temp]=0.9;
   temp=temp-5;
   x_roi=320+temp;      /////
   
   temp=random(11);
   //temp=5;   
   Target[temp+11]=0.9;
   temp=temp-5;
   y_roi=270+temp;    
           
} 

//--------------------------------
void move_roi_rand(void)
{
    
 //  x=320  and y=270 are the "centered" position of the ROI
    
 int i,temp;
   for(i=0;i<N_OUT;i++) Target[i]=0.1; // todos los targets en 0
   temp=random(22);    
   Target[temp]=0.9;   //  se sube solo un target aleatorio
   
   
   temp=random(11);   
   temp=temp-5;
   x_roi=320+temp;     //  mueve en el eje x 
   
   temp=random(11);    
   temp=temp-5;
   y_roi=270+temp;     //  mueve en el eje y    
           
} 

//--------------------------------------------------------------

void train(void)  //                                                  TRAIN                          TRAIN          ***************                                                                                                                      TRAIN  !!
{
int slow; 

   while(1)
    {  
     //set_targets();
     if(b_flag) move_roi_target();   
     
     //move_roi_target();   
     procesa_imagen(); 
     
     if(b_flag)corregir_pesos();              //    BACK PROPAGATION
 
     plot_counter++;
     
     
     busca_neuron_ganador();
     
     if((winner_x>0.6)&&(winner_y>0.6)) printf("Shakira ! \n");
     else  printf("unknown image \n");
     if(plot_counter>11)
         {   
          plot_counter=0;   
          //plot_net_image_input(); 
          plot_features();
         }
     slow++;
         
         
     plot_hidden_outputs();  
     plot_outputs();    
     plot_targets();
     
     picture_video++;
     if(picture_video>2) picture_video=0;
      
     if(kbhit()) break;
     if(stop)break;
         
    }
     plot_hidden_outputs();   
     plot_features();
  
}    
//===================================================================================================
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
   
    trace_flag=0;
   
    CargarImagenes();
            
    for(i=0;i<N_OUT;i++) Target[i]=0.1;
        
    //prepare_scene();
   
    trace_flag=1;
    cutter=0.38;                 // 18    unbral para B&W
    plot_loaded_pictures();   
  
    energy_band=0.045        ;
   
    picture_video_flag=1;
    stop=0;  
    b_flag=1; 
    picture_video=0;    //  [picture_video]
    x_roi=320;
    y_roi=270;
    //plot_square();   
    hidden_pointer=3; 
    CargarPesos();
    train();   
    
    do {
       //play_flag=0; 
       key=getch();
       switch (key) {
                       
            
         case '1':              { 
                                   erase_square();
                                   x_roi--; 
                                   plot_loaded_pictures(); 
                                   plot_square();
                                   train(); 
                                } 
           break;                                
  
           case '2':            {
                                   erase_square();
                                   x_roi++; 
                                   plot_loaded_pictures(); 
                                   plot_square();
                                   train(); 
                                }
           break;    

           case 'a':              { 
                                   erase_square();
                                   y_roi++; 
                                   plot_square();
                                   plot_loaded_pictures();
                                   train(); 
                                } 
           break;                                
  
           case 'q':            {
                                   erase_square();
                                   y_roi--; 
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

