//-----------------------------
//
//    plot items   ============
//-----------------------------
void plot_matrix_data(void)                                                    // PLOT GIANT CELL
{
 int i,j,k,sx,sy,counter, scale,color,x,y; 
 int x_matrix, y_matrix;   
 float timp; 
    
 x_matrix=20;
 y_matrix=160; 
    
    setcolor(WHITE);  
    setfillstyle(SOLID_FILL,WHITE);     //                                
    bar(x_matrix,y_matrix,x_matrix+150,y_matrix+60) ;                 //   ERASE OLD IMAGE
    
 
   scale=5;
  
     for(j=0;j<matrix_row;j++)  //  22
     { 
      sy = y_matrix+j*scale+j;  
      x_matrix=20;   
      for(i=0;i<matrix_column;i++)   // 10
         {    
          sx = x_matrix+i*scale+i;
          timp= matrix_data[j][i];    
          //timp= Entrada[counter];  
          color=WHITE;   
          if(timp<0.5) color=LIGHTGRAY;        // fROZEN
          //if(timp>0.001) color=RED;         // HOT !     
          else color=DARKGRAY;       
          setcolor(color); 
          //setcolor(GREEN); 
          setfillstyle(SOLID_FILL,color); 
          bar(sx,sy,sx+scale,sy+scale) ;
          //putpixel(sx,sy, color);      
          if(i==10) x_matrix=x_matrix+10;
         }
     } 
         
    
}
//-----------------------------
void discharge_picture_video(void)
{
int   i,j,k,xi,yi,sx,sy,counter,color;
 float temp,d_pixel, strip[100];
   counter=0;
    
   x_discharge_image=70;   // 20
   y_discharge_image=140;           //  150
    
   xi = x_discharge_image;    //         calibrated
   yi = y_discharge_image;    //     
    
   d_pixel=copy[0][picture_video];
   energy_average=0;    
      
   for(k=0;k<100;k++) strip[k]=copy[k][picture_video];  
   for(j=0;j<100;j++)  // 
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
       
              
          temp=copy[counter][picture_video]-strip[i]; 
          if(temp<0) temp=-temp;  if(temp<0) temp=-temp;  
          if(temp>energy_band) color=BLACK;
          putpixel(sx,sy,color);                     //   
          d_pixel=copy[counter][picture_video];
          energy_average=energy_average+copy[counter][picture_video];   
          counter++;               
         }
        energy_average=energy_average/10000;         
     }
} 
//-----------------------------------
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

//----------------------------------

void plot_loaded_pictures()
{
 int   sx,sy,i,j,x,y,color, counter;
 float timp;
  
   x=200; //       COORDENATES WHERE THE IMAGES STORED IN copy[pixel] [n]  ARE PLOTED                     
   y=280;  //
    
    
   setfillstyle(SOLID_FILL,BLACK);     //                          ERASE  background!
   setcolor(BLACK); 
   bar(x-10,y-10,x+340,y+100); 
   
    
   setcolor(DARKGRAY);
   
       
             
   counter=0;
   for(j=0;j<100;j++)  // 
     { 
      sy = y+j;       
      for(i=0;i<100;i++)
         {    
          sx = x+i;
          timp= copy[counter][0];            // PLOT PICTURE 0
          if(timp<cutter) color=BLACK;
          if(timp>cutter) color=WHITE; 
          //if(j>94) color=BLACK;    
          putpixel(sx,sy, color);      
          counter++;               
         }
                 
     }
 //---------------------------------------------------------------    
   x=320;   
   counter=0;
   for(j=0;j<100;j++)  // 
     { 
      sy = y+j;       
      for(i=0;i<100;i++)
         {    
          sx = x+i;
          timp= copy[counter][1];         // PLOT PICTURE 1
          if(timp<cutter) color=BLACK;
          if(timp>cutter) color=WHITE; 
          //if(j>94) color=BLACK;    
          putpixel(sx,sy, color);      
          counter++;               
         }
                 
     } 
//-----------------------------------------------------
   x=440;   
   counter=0;
   for(j=0;j<100;j++)  // 
     { 
      sy = y+j;       
      for(i=0;i<100;i++)
         {    
          sx = x+i;
          timp= copy[counter][2];            // PLOT PICTURE 2
          if(timp<cutter) color=BLACK;
          if(timp>cutter) color=WHITE; 
          //if(j>94) color=BLACK;    
          putpixel(sx,sy, color);      
          counter++;               
         }
                 
     }
} 
       
//-----------------------------------------------
void plot_features()                                                         ////       PLOT FEATURRE
{
 int   sx,sy,i,j,k,x,y,color, counter;
 float timp;
  // radar_flag = 0;
   x=150;
   y=10;  
   setcolor(DARKGRAY);
  
    
   counter=0;
   for(k=0;k<5;k++)     //   plots only first five neurons
    {   
     for(j=0;j<100;j++)  // 
     { 
      sy = y+j;       
      for(i=0;i<100;i++)
         {    
          sx = x+i;
          timp=c_escondida.pesos[k][counter];   
          //timp= Entrada[counter];  
          color=WHITE;   
          if(timp<-0.01) color=BLUE;      // fROZEN     HERE PLOT BARRIER FOR TRIMMED WEIGHTS
          if(timp>0.01) color=RED;         // HOT !        
          putpixel(sx,sy, color);      
          counter++;               
         }
     }
    counter=0;
    x=x+115;
     
    } 
     //line(x,y-1,x+100,y-1); 
     //line(x+100,y,x+100,y+100); 
     //line(x+100,y+100,x,y+100);            
     //line(x-1,y+100,x-1,y-1);

} 
//++++++++++++++++++++++++++++++++++++++++++++ 
void plot_hidden_outputs(void)
{
 int y,x,i,temp;
  x=200;
  y=250; //210
  
      setfillstyle(SOLID_FILL,WHITE);     //                         ERASE  HIDDEN outputs  !!
      bar(x,y-1,x+120,y-90);
   
  setcolor(DARKGRAY);
  line(x,y-40,x+200,y-40);    
      
      for(i=0;i<N_HID;i++) 
       {
         setfillstyle(SOLID_FILL,GREEN);     //                            PLOT  HIDDEN outputs  !!
         temp= c_escondida.out[i]*80;              
         bar(x,y,x+2,y-temp) ;
         //if(i==10) x=x+20 ;  
         x=x+3;           
         
       }   
}
//----------------------------------------------

void plot_outputs(void)
{
 int y,x,i,temp;
  x=30;
  y=340; //210
    
    
       setcolor(WHITE);  
       setfillstyle(SOLID_FILL,WHITE);     //                                
       bar(x,y,x+80,y-80) ;                 //   ERASE OLD IMAGE
       
       setcolor(DARKGRAY);
       line(x,y-40,x+100,y-40);    
    
      
      for(i=0;i<N_OUT;i++) 
       {
         //setfillstyle(SOLID_FILL,BLUE);     //            PLOT outputs  !!
         setcolor(BLUE);  
         if(b_flag) setcolor(RED); 
         temp= c_salida.out[i]*80;              
         bar(x,y,x+2,y-temp) ;
         if(i==10) x=x+5;  
         x=x+3;           
         
       }   

         setfillstyle(SOLID_FILL,CYAN);     //                                                                                        PLOT outputs  !!
         temp= blue_column;              
         bar(x,y,x+3,y-temp) ;   
}
//++++++++++++++++++++++++++++++++++++++++++++ 
//----------------------------------------------------*
void plot_targets(void)
{
 int y,x,i,temp;
  x=30;
  y=460; //430
    
       setcolor(WHITE);  
       setfillstyle(SOLID_FILL,WHITE);     //                                
       bar(x,y,x+80,y-80) ;                 //   ERASE OLD IMAGE
      
      for(i=0;i<N_OUT;i++) 
       {
         setfillstyle(SOLID_FILL,BLUE);     //                                                                                        PLOT outputs  !!
         temp= Target[i]*80;              
         bar(x,y,x+2,y-temp) ;
         if(i==10) x=x+5 ;    
         x=x+3;           
       }    
}
//++++++++++++++++++++++++++++++++++++++++++++ 

//----------------------------------------------------
void borrar(int i,int j)
{
    settextstyle(0, HORIZ_DIR, 1);   
    setcolor(BLACK);     
    moveto(i,j);
    outtext("     ");
    
}

//---------------------------------------------------*
void plot_print_pesos(void)
{
int i,j,x,y;
float  temp;    
char str[10];   
x=380;
y=60;    
settextstyle(0, HORIZ_DIR, 1);
five=0;

for (j=0;j<2;j++)
    {
        for(i=0;i<24;i++)
         { 
          borrar(x,y);
          setcolor(GREEN); 
          if(!five) setcolor(RED);             
          settextstyle(0, HORIZ_DIR, 1);   
          
          temp=c_escondida.pesos[j][i];
          moveto(x, y);  
          outtext(gcvt(temp, 6, str)); 
          y=y+10;
           five++;
          if(five>3) five=0; 
         
         }  
   
    x=x+87;
    y=60;
    }
    
    five=0;
    j=0;  
        for(i=0;i<N_OUT;i++)
        { 
          setcolor(GREEN);  
            if(!five) setcolor(RED);                  
          settextstyle(0, HORIZ_DIR, 1);   
          moveto(x, y);
          temp=c_salida.pesos[j][i];
          outtext(gcvt(temp, 6, str)); 
          y=y+10;
          five++;
          if(five>3) five=0; 
        y=y+10;
        }
        
        setcolor(GREEN);  
        settextstyle(0, HORIZ_DIR, 1);   
        moveto(x, y);
        temp=energy_band;
        outtext(gcvt(temp, 6, str)); 
        
        y=y+50;
        setcolor(RED);  
        settextstyle(0, HORIZ_DIR, 1);   
        moveto(x, y);
        temp=energy_average;
        outtext(gcvt(temp, 6, str)); 
        
}
//----------------------------------------------------------------------
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
//-----------------------------------------------------------