//----------------------------------
//
//    Moving ROI with automatic route trace display
//
//---------------00000--------------
//
//----------------------------------

void capture_roi_imagen(void)                                                               //  ROI
{

 int   i,j,xi,yi,sx,sy,counter;
 //float temp,d_pixel;
 float a,b;
 
   counter=0;
   //xi = x_roi-55;  // 50,50           
   //yi = y_roi-53;  
   
    
   xi = x_roi;  // 50,50           
   yi = y_roi;   
    
   for(j=0;j<100;j++)  // 
     { 
      sy = yi+j;
      for(i=0;i<100;i++)
         {    
          sx = xi+i;
          a=getpixel(sx,sy);
          b=getpixel(sx+1,sy); 
  
          if(a!=b) Entrada[counter]=1.0;    //    medidores de borde   !!
          else  Entrada[counter]=0.1;       //    medidores de borde   !!     
      
          b=getpixel(sx,sy+1); 
          if(a!=b) Entrada[counter]=0.0;
         
          //if(!random(100000)) putpixel(sx,sy, BLUE);   // statistical zone marking
 
          counter++;       
           
         }
                 
     }
     setcolor(BLUE);
     //line(xi,yi-1,xi+100,yi-1); 
     //line(xi+100,yi,xi+100,yi+100); 
     //line(xi+100,yi+100,xi,yi+100);            
     //line(xi-1,yi+100,xi-1,yi-1);                  
} 
//----------------------------------------------------                          

//----------------------------------

