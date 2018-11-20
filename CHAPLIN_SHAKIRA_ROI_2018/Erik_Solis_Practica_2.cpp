/************************ top_eye_flop.cpp **********************************

Name:         CHAPLIN Shakira.cpp
codigo fuente base:   prof.    oscar  chang
August  2018
/*********************************************************************/
//---------   GLOBALs
float copy[10000][3];  //    matriz de las imagenes de entrada [numero de pixels] [numero de imagenes]
int picture_video;

// 1) cargar  2) plot image  3) video  4) ejecutar ROI cargar imagen en el vector de entrada

float cutter;
float energy_band;
float energy_average;
int b_flag;
int blue_column;


int x_roi;     //   coordenadas de la posicion de la camara o roi, pequeña zona donde capturamos pixels 
int y_roi;     //

int x_discharge_image;  //  coordenadas de la zona donde descargamos la imagen a ser analizada
int y_discharge_image;

//------------------------------------------
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include "neural_lib.H"              // ----      codigo en C/C++ para manejar redes de neuronas

#include "plot_items.h"
#include "load_binary_images.h"
#include "load_save_weights.h"

/******************* variables locales  ***************************************/
int  winner;           // contadores genericos
int  x,y;
int  xd,yd,alfa;
char color;
char key;
int trace_flag;

int stop;
int  picture_video_flag;




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
//---------------------------------------
     
//-------------------------------------
void discharge_picture_video(void)
{
int   i,j,k,xi,yi,sx,sy,counter,color;
 float temp,d_pixel, strip[100];
   counter=0;
    
   x_discharge_image=70; //20           // dentro del cuadro
   y_discharge_image=140;           //  150
    
    
   xi = x_discharge_image;    //         calibrated
   yi = y_discharge_image;    //     
    
      
   d_pixel=copy[0][picture_video];
   energy_average=0;    
      
   for(k=0;k<100;k++) strip[k]=copy[k][picture_video];  
   for(j=0;j<100;j++)  //  secuencia de imagenes
     {
      if(j>0) {for(k=0;k<100;k++) strip[k]=copy[k+(j-1)*100][picture_video];}   
      sy = yi+j;       
      for(i=0;i<100;i++)
         {
          color=WHITE;    
          sx = xi+i;
          temp=copy[counter][picture_video]-d_pixel;  //   Medidor de Umbral 
          if(temp<0) temp=-temp;    
          if(temp>energy_band) color=GREEN;   //BLACK;  
          
          // Practica 2
          // Agregando ruido blanco a la imagen (Black), la red sigue aprendiendo sin afectar su capacidad de inferir.
          if(temp>energy_band*random(3)) color=BLACK; 
           
          // dibujar imagen    
          temp=copy[counter][picture_video]-strip[i]; 
          if(temp<0) temp=-temp;  if(temp<0) temp=-temp;  
          if(temp>energy_band) color=BLACK;
          putpixel(sx,sy,color); 
          d_pixel=copy[counter][picture_video];
          energy_average=energy_average+copy[counter][picture_video];  
          
          counter++;               
         }
        energy_average=energy_average/10000;         
     }
 
               
} 
//------------------------------------
void plot_net_image_input()             // lo que esta entrando en la red,  Testigo de entrada
{
 int   sx,sy,i,j,x,y,color, counter;
 float timp;
   
   x=10;
   y=10;  
   setcolor(DARKGRAY);
  
    
   counter=0;
   for(j=0;j<100;j++)  // 
     { 
      sy = y+j;       
      for(i=0;i<100;i++)
         {    
          sx = x+i;
          timp= Entrada[counter];  
          if(timp<0.5) color=BLACK;
          if(timp>0.5) color=WHITE;    
          putpixel(sx,sy, color);      
             
          counter++;               
         }
                 
     }
     
     
     line(x,y-1,x+100,y-1); 
     line(x+100,y,x+100,y+100); 
     line(x+100,y+100,x,y+100);            
     line(x-1,y+100,x-1,y-1);

} 
            
//++++++++++++++++++++++++++++++++++++++++++++ 
void erase_square(void)
{
int rx,ry;    
   rx=x_roi;
   ry=y_roi;  

   setcolor(WHITE);
   line(rx,ry-1,rx+100,ry-1); 
   line(rx+100,ry,rx+100,ry+100); 
   line(rx+100,ry+100,rx,ry+100);            
   line(rx-1,ry+100,rx-1,ry-1);
 }
//______________________________ 
void plot_square(void) 
{ 
 int rx,ry;    
   rx=x_roi;
   ry=y_roi;     
    
   setcolor(DARKGRAY);
   line(rx,ry-1,rx+100,ry-1); 
   line(rx+100,ry,rx+100,ry+100); 
   line(rx+100,ry+100,rx,ry+100);            
   line(rx-1,ry+100,rx-1,ry-1);  
}
//------------------------------------------------------- PUMP!
void capture_roi_imagen(void)                                                                              //  capture_imagen
{
 int   i,j,xi,yi,sx,sy,counter;
 //float temp;
   counter=0;
   xi = x_roi;  // 50,50           
   yi = y_roi;  
   //sx = xi+i;
    
   for(j=0;j<100;j++)  //  10 000 vueltas
     { 
      sy = yi+j;        
      for(i=0;i<100;i++)
         {    
          sx = xi+i;
          color= getpixel(sx,sy);      
          if(color==WHITE) Entrada[counter]=0.1;    // 0.1 color        Entrada[N_IN] 
          else  Entrada[counter]=0.9;               // color
           
          //if(!random(2))
            //{
            //    if (random(2))Entrada[counter] = 0.1;
            //    else Entrada[counter] = 0.9;
          //  }
          
          counter++;                              // aumento contador
         }                                     //  Capture image MARKER
                 
     }
                      
} 
//----------------------------------------------------
void procesa_imagen(void)
{
   
            capture_roi_imagen();       //  capture_imagen carga las entradas de la red
            if(trace_flag) plot_net_image_input(); 
            
            calcular_capa_escondida();  // en neural lib, coger entrada de datos (19 hidden) a cada neuron escondida 10 000 pesos pesos = 19*10 000
            calcular_capa_salida(); 
            
            plot_targets();
            plot_outputs();
                    
}
//----------------------------------------------------------------
                       
//------------------------------------                                                    //////     MOVE  CAMERA       
void set_targets(void)
{
 int i;  
   x=250;       
   y=200;    // coordenadas donde se ubica nuestra "camara"     
    
    
   for(i=0;i<N_SAL;i++) Target[i]=0.1; 
    
   if(picture_video==0) Target[0]=0.9;
      
   if(picture_video==1) Target[1]=0.9;      // dependiendo de la imagen, ajusto el target 
       
   if(picture_video==2) Target[2]=0.9;
           
}           
//--------------------------------------------------------------

void train(void)  //                                                  TRAIN                          TRAIN          ***************                                                                                                                      TRAIN  !!
{
int slow; 

   while(1)
    {  
     set_targets();             // objectivos
        
       // plot_dots(); 
     
     //if(picture_video_flag) {plot_picture_video(); }
   
     discharge_picture_video();                 // descargamos imagen
       
     procesa_imagen(); 
     
     if(b_flag)corregir_pesos();              //    BACK PROPAGATION     corregir pesos
     plot_print_pesos(); 
     slow++;
        
     picture_video++;
     if(picture_video>2) picture_video=0;
      
     if(kbhit()) break;
     if(stop)break;
         
    }
    //SalvarPesos();
                        
    plot_print_pesos();    
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
            
    for(i=0;i<N_SAL;i++) Target[i]=0.1;
        
    //prepare_scene();
   
    trace_flag=1;
    cutter=0.18;                  //   unbral para B&W
    plot_loaded_pictures();   
  
    energy_band=0.045        ;
   
    picture_video_flag=1;
    stop=0;  
    b_flag=1; 
    picture_video=0;    //  [picture_video]
    x_roi=77;
    y_roi=139;
    plot_square();   
    
    train();   
    
    do {
       //play_flag=0; 
       key=getch();
       switch (key) {
                       
            
         case '1':              { 
                                   erase_square();
                                   x_roi--; 
                                   plot_square();
                                   train(); 
                                } 
           break;                                
  
           case '2':            {
                                   erase_square();
                                   x_roi++; 
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
                                   x_roi--; 
                                   plot_square();
                                   train(); 
                                }
           break;   
                                
                                
                                
                                
                                
           
           case 'r':            { 
                                   inicializar_pesos();
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
                                plot_print_pesos();
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
BLACK        ³  0  ³ Yes ³ Yes
BLUE         ³  1  ³ Yes ³ Yes
GREEN        ³  2  ³ Yes ³ Yes
CYAN         ³  3  ³ Yes ³ Yes
RED          ³  4  ³ Yes ³ Yes
MAGENTA      ³  5  ³ Yes ³ Yes
BROWN        ³  6  ³ Yes ³ Yes
LIGHTGRAY    ³  7  ³ Yes ³ Yes
DARKGRAY     ³  8  ³ No  ³ Yes
LIGHTBLUE    ³  9  ³ No  ³ Yes
LIGHTGREEN   ³ 10  ³ No  ³ Yes
LIGHTCYAN    ³ 11  ³ No  ³ Yes
LIGHTRED     ³ 12  ³ No  ³ Yes
LIGHTMAGENTA ³ 13  ³ No  ³ Yes
YELLOW       ³ 14  ³ No  ³ Yes
WHITE        ³ 15  ³ No  ³ Yes

----------------------------------------------------------------
     if(x<0)    { x=0; if( heat<100) heat++;}     
     if(y<35)    { y=35; if( heat<100) heat++;}  
     if(x>640) { x=640; if( heat<100) heat++;}  //  x>1200 : full pantalla 
     if(y>470) { y=470; if( heat<100) heat++;}   // y>720 : full pantalla 
         
//--------------------------------------------------
// ROI: region of interest -> cuadrado
// spars: few neurons, lot neurons
************************ basic_robot.cpp ***************************
desarrollado usando Dev-C++ IDE V4.9.8.4 GNU / MinGW / GNU gcc
                    Sc1 IDE / BCC5.5
                    version orginal por Oscar Chang
*********************************************************************/

