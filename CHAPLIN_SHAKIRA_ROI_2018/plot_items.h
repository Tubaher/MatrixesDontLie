//-----------------------------
//
//    plot items   ============
//-----------------------------

void plot_loaded_pictures()
{
 int   sx,sy,i,j,x,y,color, counter;
 float timp;
  
   x=200; //       COORDENATES WHERE THE IMAGES STORED IN copy[pixel] [n]  ARE PLOTED                     
   y=330;  
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

void plot_outputs(void)
{
 int y,x,i,temp;
  x=30;
  y=340; //210
    
    
       setcolor(WHITE);  
       setfillstyle(SOLID_FILL,WHITE);     //                                
       bar(x,y,x+50,y-80) ;                 //   ERASE OLD IMAGE
       
       setcolor(DARKGRAY);
       line(x,y-40,x+100,y-40);    
    
      
      for(i=0;i<N_SAL;i++) 
       {
         //setfillstyle(SOLID_FILL,BLUE);     //            PLOT outputs  !!
         setcolor(BLUE);  
         if(b_flag) setcolor(RED); 
         temp= c_salida.out[i]*80;              
         bar(x,y,x+3,y-temp) ;
         if(i==10) x=x+20 ;  
         x=x+10;           
         
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
       bar(x,y,x+50,y-80) ;                 //   ERASE OLD IMAGE
      
      for(i=0;i<N_SAL;i++) 
       {
         setfillstyle(SOLID_FILL,BLUE);     //                                                                                        PLOT outputs  !!
         temp= Target[i]*80;              
         bar(x,y,x+3,y-temp) ;
         if(i==10) x=x+20 ;    
         x=x+10;           
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
        for(i=0;i<N_SAL;i++)
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
