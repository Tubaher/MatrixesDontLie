#include <stdlib.h>
#include "highgui.h"
#include "cv.h" 
#include "cxcore.h" 
//#include <tif(greenime.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>

#include "neural_lib.h"
#include "cargar_pesos.h" 
//----------------------------------------------------------------
CvMemStorage* 	g_storage = NULL;

IplImage* frame= 0;
IplImage* imgRgbAll=NULL;	    //circulos en pantalla
IplImage* imgAux=NULL;
IplImage* im_gray=NULL;
IplImage* im_bw=NULL;
IplImage* im_ROI=NULL;

//--------------------------------------------------------
#define INPUT_MAX				(SIZE_ROI*SIZE_ROI)
#define SIZE_FRAME_WIDTH		320 //320,640
#define SIZE_FRAME_HEIGHT		240 //240,480
#define SIZE_ROI				100
#define FPS_CFG			         29 //max 30pfs               // frames per second
//--------------------------------------------------------
#define WHITE       cvScalar(255, 255, 255, 0)
#define RED         cvScalar(0,0,255, 0)
#define BLUE        cvScalar(255,0,0,0)
#define LIGHT_BLUE  cvScalar(255,100,100,0)
#define GREEN       cvScalar(0,255,0,0)
//---------------------------------------------

int iAux;
//unsigned char cInput[INPUT_MAX]; //[(100x100)/8]=1250
//float		  fInput[INPUT_MAX];

unsigned int iLowThresHold,iHighThresHold;
unsigned int ixRoi,iyRoi;
int i,j,k;

int rest_x_roi, rest_y_roi;
int roi_x_mov=100, roi_y_mov=50;
int x_increment, y_increment;

int winner_x, winner_y, winner_d;
float blue_column, green_column;

int fifo[10];

int y_spot=20;
int  fifo_base=10;
int y_shake;

int x_camera_offset, y_camera_offset, internal_x, internal_y;
int y_shake_inc;
float hit, foul;

int y_average;
int hid_pointer;

int sep_y,sep_y_avr;
int y_flag;
//float x_average;
//--------------------------------------------------------@@@@@@                    VARIABLES GLOBALES
void redraw_ROI(void)      //  New ROI in town
{
	int i,j,k;
	k=0;
	for(j=0;j<SIZE_ROI;j++)	
	{
		for(i=0;i<SIZE_ROI;i++)	
		{
			
			if(Entrada[k]>=(float)(0.9))		cvSet2D(im_ROI,j,i,cvScalar(255,255,0));//set the pixel
			else								cvSet2D(im_ROI,j,i,cvScalar(0  ,0  ,  0));//clear the pixel
			k++;
		}
	}
}

//--------------------------------------------------------
//void clean_input(void)
//{
//	for(int i=0;i<INPUT_MAX;i++)	fInput[i]=0.0;
//}
//-------------------------------------------------
void get_ROI(int iXpos,int iYpos,IplImage* pROI)   //  New ROI in town !
{
	int i,j,iPtrArray;

	//clean_input();

	cvRectangle(pROI,cvPoint(iXpos-1,iYpos-1),cvPoint(iXpos+SIZE_ROI,iYpos+SIZE_ROI),cvScalar(255,0,0),1,8,0);//ROI draw square region -1
	iPtrArray=0;

	//void cvSet2D(CvArr*, int row, int col, CvScalar);

	for(j=iYpos;j<(SIZE_ROI+iYpos);j++)	
	{
		//for(j=iYpos;j<(SIZE_ROI+iYpos);j++)	//orig
		for(i=iXpos;i<(SIZE_ROI+iXpos);i++)	

		{

			CvScalar scal = cvGet2D(pROI,j,i);  
			
			if(scal.val[0])	Entrada[iPtrArray]=(float)(0.9);
			else			Entrada[iPtrArray]=(float)(0.1);
			iPtrArray++;

		}
	}
}
//----------------------------------------------------------
void edge_detector(void)
{
	CvMemStorage* 	g_storage = NULL;

	if(g_storage == NULL)	{	g_storage = cvCreateMemStorage(0);	} 
	else					{	cvClearMemStorage(g_storage);		}

	CvSeq* contours = 0;

	cvFindContours(im_bw, g_storage, &contours );
	cvZero( im_bw);
	if(contours)	    cvDrawContours(im_bw,contours,cvScalarAll(255),cvScalarAll(255),100);

}
//-------------------------------------------
void plot_feature(void)
{
	int i,j,x,y,counter;
	float timp;
	counter=0;
	x=5;
	y=210;
	for(i=0;i<100;i++)  // 
     { 
         
      for(j=0;j<100;j++)
         {    
          
          timp=c_escondida.pesos[hid_pointer][counter];   
          
          if(timp<-0.001)    cvSet2D(frame,x,j+y,cvScalar(255,0,0));   // primero j, luego i
          if(timp>0.001)     cvSet2D(frame,x,j+y,cvScalar(0,0,255));  
		  counter++;
         }
	    x++;
     }     
}
//-------------------------------------------
void plot_outputs(void)
{
int i,j,k,x,y;
float temp;
y=100;
x=10;

k=0;
	for(i=0;i<N_SAL;i++)	
	{
		k=x+8*i;
		cvLine(frame,cvPoint(k,y),cvPoint(k,y-c_salida.out[i]*80),RED,2,8,0);
		if(i==10) x=x+10;
	}

	cvLine(frame,cvPoint(k+15,y),cvPoint(k+15,y-green_column*80),GREEN,4,8,0);
	cvLine(frame,cvPoint(k+15,200),cvPoint(k+15,200-blue_column*80),BLUE,4,8,0);

	x=10;
	for(i=0;i<N_SAL;i++)	
	{
		k=x+8*i;
		cvLine(frame,cvPoint(k,200),cvPoint(k,200-Target[i]*80),LIGHT_BLUE,2,8,0);
		if(i==10) x=x+10;
	}
	
	
}
//---------------------------------------------
void pump_fifo(void)         //              **    PUMP_FIFO_M   **    637
{
  int i,j;
  j=fifo_base-2;             // 10= numero de alementos en fifo , sigue 10-2

  sep_y_avr=0;
 
  for(i=0;i<fifo_base-1;i++)    //   
    {
     fifo[j+1] = fifo[j];
     j--;        
    }
  fifo[0] = sep_y;
  
  for(i=0;i<fifo_base;i++) sep_y_avr = sep_y_avr+fifo[i];
  sep_y_avr= sep_y_avr/10.0;                      // fifo_base;

}

//----------------------------------------------
void set_targets(void)
{
	int lows,i,j,n, dir_flag;
	float max;
	y_flag=0;
	for(i=0;i<N_SAL;i++) Target[i]=0.1;   //  todos los targets a 0.1

	n=winner_x;
	if(n==0) n=1;
	if(n>10) n+10;

	max=c_salida.out[n-1]+c_salida.out[n]+c_salida.out[n+1];
	if(max>0.70)  Target[n]=0.9;

	n=winner_y;
	if(n==11) n=12;
	if(n>20) n=20;

	max=c_salida.out[n-1]+c_salida.out[n]+c_salida.out[n+1];
	if(max>0.70)  {Target[n]=0.9;y_flag=1;}

	//dir_flag=1;
	//f((winner_x>0)&&(winner_x<10)&&(c_salida.out[n]<0.5))  //  nueve posibilidades
	//	{

		//	if((c_salida.out[n-1]>0.2) || (c_salida.out[n+1]>0.2)) dir_flag=1; 
		//	else dir_flag=0;
	//	}
	//lows=0;
    //for (i=0; i<N_SAL-11; i++) if(c_salida.out[i]<0.12  ) lows++;

	//if((c_salida.out[n]>0.7)&&(lows>8)) Target[n]=0.9;
	//n=x_average;
	 
	 
	 
	 //Target[n]=0.9;

    /*
	max=c_salida.out[winner_x];
    for (i=0; i<N_SAL-11; i++) if(i!=winner_x)  max= max-c_salida.out[i]*0.10;       //0.25;   
    if(max<0.1) max=0.1;    
    blue_column=max; 
	winner_y=winner_y-11;


	max=c_salida.out[winner_y];
	for (i=11; i<N_SAL; i++) if(i!=winner_y)  max= max-c_salida.out[i]*0.10;       //0.25;   
    if(max<0.1) max=0.1;    
   // green_column=max;  

	 Target[j]=0.9;
	//j=j-11;
      */	
}
//-------------------------------------------
void calculate_winners(void)              //                                                            CALCULAR BLUE_COLUMN     !!
{
   int i,j,k,second;
  
  float max;
               
    //winner_x=0;
    //winner_y=0;
  for (i=0; i<N_SAL; i++) Target[i]=0.1;

    j=0;
    max=c_salida.out[0];
    
    for (i=0; i<N_SAL-11; i++)
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
    
    for (i=11; i<N_SAL; i++)
        {                            
          if (c_salida.out[i]>max)
           {
            max  = c_salida.out[i];
            j= i;
           }        
        }
   	
    winner_y=j;
	k=j;

     j=11;
	 max=c_salida.out[11];
	 for (i=11; i<N_SAL; i++)
        {                            
          if ((c_salida.out[i]>max)&&(i!=k))
           {
            max  = c_salida.out[i];
            j= i;
           }        
        }

	 second=j;
	 k=winner_y-second;
	 if(k<0) k=-k;
	 sep_y=k;


	
} 
//----------------------------------------------
void calculate_green_column(void)              //                                                            CALCULAR BLUE_COLUMN     !!
{
   int rest,i,j;
   float max;
   //iyRoi=rest_y_roi+j-roi_y_mov/2;     //+j-100;
        hit=0.0;
		rest=winner_d-winner_y;
		if(rest<0) rest=-rest;
		
		if(sep_y_avr<3) hit=0.03;
		else hit=-0.02;
		if(y_flag) hit=0.0;
		//if(winner_d==winner_y+1) hit=0.03;

		if((green_column<0.9)&&(y_flag)) green_column=green_column+hit;
		//if((green_column>0.1)&&(y_flag)) green_column=green_column-foul;
      
}       

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funcion: main																									//
// Description: Core del sistema																					//
// Inputs:																											//
// Output:																											//
///////////////////////////////////////////////////////aaaaaaaa///////////////////////////////////////////////////////

int main()
{
//                        INIT SCENE
	i=0;j=0;k=0;
	
	ixRoi=110;

	rest_x_roi=110;
	rest_y_roi=70;

	iyRoi=70;
	x_increment=1;
	fifo_base=10;
	for (i=0; i<N_SAL; i++) Target[i]=0.1;
	y_shake=1;
	y_shake_inc=1;

	foul=0.01;
	hit=0.01;
 
	CargarPesos();

	// Initialize capturing live feed from the camera
    CvCapture* capture = 0;
	capture = cvCaptureFromCAM(0);

	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, SIZE_FRAME_WIDTH );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, SIZE_FRAME_HEIGHT );
	cvSetCaptureProperty(capture,CV_CAP_PROP_FPS,FPS_CFG); 

    // Couldn't get a device? Throw an error and quit
    if(!capture)
    {
        printf("Could not initialize capturing...\n");
        return -1;
    }
	// The two windows we'll be using
    cvNamedWindow("Gray Video");
	cvNamedWindow("Color Video");
	cvNamedWindow("B&W");
	cvNamedWindow("ROI Re-draw");

	iLowThresHold=148;    //  128
	iHighThresHold=255;
  	
	
	while(true)                                //                   LOOPS BEGUIN
    {
        // Will hold a frame captured from the camera

        frame = cvQueryFrame(capture);	// de donde se va a capturar de que camara
		// If we couldn't grab a frame... quit
        if(!frame)
            break;
		// If this is the first frame, we need to initialize it
        if(imgRgbAll == NULL)
        {
        	imgRgbAll= cvCreateImage(cvGetSize(frame),8, 3);//crea una vanetana del tamano frame 1 sola vez
			im_gray  = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
			im_bw = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
			im_ROI=cvCreateImage(cvSize(SIZE_ROI,SIZE_ROI),IPL_DEPTH_8U,1);//ROI
        }

		cvCvtColor(frame,im_gray,CV_RGB2GRAY); //gray
		cvThreshold(im_gray, im_bw, iLowThresHold, iHighThresHold, CV_THRESH_BINARY ); //black and white image
		edge_detector();//black and white edge

		get_ROI(ixRoi,iyRoi,im_bw); //copy ROI to input vector  // <<<<< ROI
		
		redraw_ROI(); //verify copied ROI
//------------------------------------------------     WORK
		calcular_capa_escondida();
		calcular_capa_salida();
        calculate_winners();
        calculate_green_column();
		set_targets();
		
		pump_fifo();

		
		
		i++;
		if(i>1000) i=0;


		//cvLine(im_bw,cvPoint(10,y_spot),cvPoint(100,y_spot),WHITE,1,8,0);

		
		plot_outputs();
		plot_feature();
//-------------------------------------------		
		cvShowImage("Color Video",frame);
		cvShowImage("Gray Video",im_gray);
		cvShowImage("B&W",im_bw);
		cvShowImage("ROI Re-draw",im_ROI);
//-------------------------------------------      //                 ROI  	coordenates	
		
		//ixRoi=ixRoi+x_increment; 

		//if(ixRoi>rest_x_roi+50) x_increment=-1; 					
		//if(ixRoi<rest_x_roi-50) x_increment=1;
			
		
		
		//if(iyRoi>rest_y_roi+30)y_increment=-1;
		//if(iyRoi<rest_y_roi-30)y_increment=1;
		
		//ixRoi=rest_x_roi+rand()%11-5; 
		//y_shake= y_shake^1;

		//ixRoi=rest_x_roi+rand()%9-4;
		//=rest_y_roi+rand()%9-4;


		y_shake=y_shake+y_shake_inc;

		if(y_shake>2) y_shake_inc=-1;   // 3       A= (-1)-(1)=2,    -2-(2)=4
		if(y_shake<2) y_shake_inc=1;    //        parametro critico!  controla la apertura del barrido
		//iyRoi=rest_y_roi+y_shake;   
	
		//else iyRoi=rest_y_roi;  


		//if(green_column>0.7) ixRoi=ixRoi-winner_x+5;

		ixRoi=ixRoi-winner_x+5;
		if(ixRoi<1)	ixRoi=110;   // rest_x_roi
		if(ixRoi>(SIZE_FRAME_WIDTH-(SIZE_ROI+1)))	ixRoi=110;
		
		// rest_x_roi=110;

		iyRoi=iyRoi-(winner_y-11)+5;
		if(iyRoi<1)	iyRoi=70;   // rest_x_roi
		if(iyRoi>(SIZE_FRAME_HEIGHT-(SIZE_ROI+1)))	iyRoi=70;


		//if(green_column>0.7) rest_y_roi=rest_y_roi-y_average+5;
		//else rest_y_roi=70;

		winner_d=winner_y;

		//printf("PESOS HID  w0=%f w1=%f        point=%i \n",c_escondida.pesos[1][i],c_escondida.pesos[1][i+1],i);

		//printf("Salidas  out1=%f  out2=%f \n",c_salida.out[1],c_salida.out[2]);
		printf("sep_y=%i sep_y_avr=%i \n",sep_y, sep_y_avr);
//-------------------------------------------
		int c = cvWaitKey(1);	//1ms
        switch(c)
        {
			case 'x':
			case 'X':
				 cvReleaseCapture(&capture);
			break;

			case 'l':
				if(iLowThresHold>0) iLowThresHold--;
			break;

			case 'L':
				if((iLowThresHold<255)&&(iLowThresHold<iHighThresHold)) iLowThresHold++;
			break;

			
			case 'k':
				if(iHighThresHold>0)	iHighThresHold--;
			break;

			case 'K':
				if(iHighThresHold<255)	iHighThresHold++;
			break;

			case 't':
				redraw_ROI();
			break;

			case 'y':
				if(iyRoi<(SIZE_FRAME_HEIGHT-(SIZE_ROI+1)))	iyRoi++;
			break;

			case 'u':
				if(iyRoi>1)	iyRoi--;
			break;

			case 'i':
				if(ixRoi>1)	ixRoi--;
			break;
			case 'd':
					if(ixRoi<(SIZE_FRAME_WIDTH-(SIZE_ROI+1)))	ixRoi++;
			break;

//------------------------------------------------------------------   NEW SWITCH
			case 'r':
						inicializar_pesos();
						plot_feature();
			break;
			case 'c':
						CargarPesos();
						plot_feature();
			break;

			case ',':
				     hid_pointer--;
					 if(hid_pointer<0) hid_pointer=18;
					 plot_feature();
			break;
			case '.':
				     hid_pointer++;
					 if(hid_pointer>18) hid_pointer=0;
					 plot_feature();
			break;

				   
        }
		if(c!=-1)
		{
		//	system("cls");
		//	printf("Low: %i  High: %i\n ",iLowThresHold,iHighThresHold);
		}
		
		cvReleaseImage(&imgRgbAll);
		cvReleaseImage(&im_gray);
		cvReleaseImage(&im_bw);
		cvReleaseImage(&im_ROI);

	}
}

