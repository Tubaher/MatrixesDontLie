#include <stdlib.h>
#include "C:\opencv\build\include\opencv\highgui.h"
#include "C:\opencv\build\include\opencv\cv.h" 
#include "C:\opencv\build\include\opencv\cxcore.h" 
//#include <tif(greenime.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>

#include "neural_lib.h"
#include "n_neural_lib.h"
#include "cargar_pesos.h" 
#include "n_cargar_pesos.h" 
//----------------------------------------------------------------
CvMemStorage* 	g_storage = NULL;

IplImage* frame= 0;
//IplImage* imgRgbAll=NULL;	    //circulos en pantalla
IplImage* imgAux=NULL;
IplImage* im_gray=NULL;
IplImage* im_bw=NULL;
IplImage* im_ROI=NULL;

// canny   -------   Define the IplImage pointers we're going to use as globals
IplImage* pFrame=0;
IplImage* pProcessedFrame;
IplImage* tempFrame;
//IplImage* frame= 0;


//--------------------------------------------------------
#define INPUT_MAX				(SIZE_ROI*SIZE_ROI)
#define SIZE_FRAME_WIDTH		320 //320,640
#define SIZE_FRAME_HEIGHT		240 //240,480
#define SIZE_ROI				100
#define FPS_CFG			         29 //max 30pfs               // frames per second
//--------------------------------------------------------
#define WHITE       cvScalar(255, 255, 255, 0)
#define BLACK       cvScalar(0,0,0,0)
#define BLUE        cvScalar(255,0,0,0)
#define GREEN       cvScalar(0,255,0,0)
#define RED         cvScalar(0,0,255,0)

#define HOT         cvScalar(100, 100, 255, 0)
#define LIGHT_BLUE  cvScalar(255,100,100,0)
#define YELLOW      cvScalar(0,200,200,0)
#define GRAY        cvScalar(100,100,100,0)
#define CYAN        cvScalar(255,255,100,0)
#define DEEP_BLUE   cvScalar(85,20,0,0)
//---------------------------------------------

int iAux;
//unsigned char cInput[INPUT_MAX]; //[(100x100)/8]=1250
//float		  fInput[INPUT_MAX];

unsigned int iLowThresHold,iHighThresHold;
unsigned int x_roi,y_roi;
int i,j,k;

int rest_x_roi, rest_y_roi;
int roi_x_mov=100, roi_y_mov=50;
int x_increment, y_increment;

int winner_x, winner_y, winner_d;
float blue_column, green_column;

float fifo[10];

int  y_spot=20;
int  fifo_base=10;
int  y_shake;

int x_camera_offset, y_camera_offset, internal_x, internal_y;
int y_shake_inc;
float hit, foul;

int y_average;
int hid_pointer;
float x_average;

#define matrix_row     10
#define matrix_column  22

float matrix_data[matrix_row][22];    //  [fila][columna]
float master_target;
int flig, flog, backpro_flag;
float red_column;

//float  energy_barrier=0.15;
int    backpro_timer;
float  main_column;

float  main_barrier=0.1001;        //  Para llenar matriz espacio-tiempo. con una milesima pararece haber buena informacion 

int happyness, tranquility;
int trimmer_count;
int roi_count;
int coin;

// Slider for the low threshold value of our edge detection
int maxLowThreshold = 1024;
int lowSliderPosition =1001;
 
// Slider for the high threshold value of our edge detection
int maxHighThreshold = 1024;
int highSliderPosition =1000;
int noise_flag;
int m_flag;

//--------------------------------------------------------@@@@@@                    VARIABLES GLOBALES
// Function to find the edges of a given IplImage object
IplImage* findEdges(IplImage* sourceFrame, double thelowThreshold, double theHighThreshold, double theAperture)
{
	// Convert source frame to greyscale version (tempFrame has already been initialised to use greyscale colour settings)
	cvCvtColor(sourceFrame, tempFrame, CV_RGB2GRAY);
 
	// Perform canny edge finding on tempframe, and push the result back into itself!
	cvCanny(tempFrame, tempFrame, thelowThreshold, theHighThreshold, theAperture);
 
	// Pass back our now processed frame!
	return tempFrame;
}


//---------------------------------------------------
void redraw_ROI(void)      //  New ROI in town
{
	int i,j,k;
	k=0;
	for(j=0;j<SIZE_ROI;j++)	
	{
		for(i=0;i<SIZE_ROI;i++)	
		{
			if(Entrada[k]>=(float)(0.9))		cvSet2D(im_ROI,j,i,cvScalar(255,255,0));     //set the pixel
			else								cvSet2D(im_ROI,j,i,cvScalar(0  ,0  ,  0));   //clear the pixel
			k++;
		}
	}
}

//--------------------------------------------------------
void clean_input(void)
{
	//for(int i=0;i<INPUT_MAX;i++)	fInput[i]=0.0;
}
//-------------------------------------------------
void get_ROI(int iXpos,int iYpos,IplImage* pROI)   //  New ROI in town !
{
	int i,j,iPtrArray, flag, width;
	float temp;
	clean_input();

	if(n_c_salida.out[1]>0.5) width=2;
	else width=1;

	cvRectangle(pROI,cvPoint(iXpos-3,iYpos-3),cvPoint(iXpos+SIZE_ROI+3,iYpos+SIZE_ROI+3),cvScalar(255,0,0),width,8,0);//ROI draw square region -1
	
	if(n_c_salida.out[1]>0.5)
	cvRectangle(frame,cvPoint(iXpos-1,iYpos-1),cvPoint(iXpos+SIZE_ROI,iYpos+SIZE_ROI),RED,3,8,0);
	else
	//cvRectangle(frame,cvPoint(iXpos-1,iYpos-1),cvPoint(iXpos+SIZE_ROI,iYpos+SIZE_ROI),cvScalar(15,15,15),1,8,0);	 
	cvRectangle(frame,cvPoint(iXpos-1,iYpos-1),cvPoint(iXpos+SIZE_ROI,iYpos+SIZE_ROI),BLUE,1,8,0);
	iPtrArray=0;
	for(j=iYpos;j<(SIZE_ROI+iYpos);j++)	
	{
		//for(j=iYpos;j<(SIZE_ROI+iYpos);j++)	//orig
		for(i=iXpos;i<(SIZE_ROI+iXpos);i++)	

		{
			CvScalar scal = cvGet2D(pROI,j,i);  
			if(scal.val[0])	Entrada[iPtrArray]=(float)(0.9);
			else			Entrada[iPtrArray]=(float)(0.1);
			if(noise_flag) {coin=rand()%20;if(coin)temp=0.1; else temp=0.9;Entrada[iPtrArray]=temp;} //DENSIDAD DE NIEBLA para ENTRENAMIENTO
			iPtrArray++;

		}
	}
}

//-------------------------------------------
void plot_feature(void)
{
	int i,j,x,y,counter;
	float timp;
	counter=0;
	y=120;
	x=210;
	for(i=0;i<100;i++)  // 
     { 
         
      for(j=0;j<100;j++)
         {    
          
          timp=c_escondida.pesos[hid_pointer][counter];   
          
          if(timp<-0.001)    cvSet2D(frame,y,j+x,cvScalar(255,0,0));   // primero j, luego i
          if(timp>0.001)     cvSet2D(frame,y,j+x,cvScalar(0,0,255));  
		  counter++;
         }
	    y++;
     }     
}
//--------------------------------------
void n_plot_feature(void)
{
	int i,j,x,y,counter, scale=3;
	float timp;
	counter=0;
	y=240; //210
	x=175; //
	
	for(i=0;i<matrix_row;i++)  // 
     { 
       for(j=0;j<matrix_column;j++)
         {    
           timp=n_c_escondida.pesos[hid_pointer][counter];   
		   if(timp<-0.001)cvLine(frame,cvPoint(y+j*scale,x),cvPoint(y+j*scale,x+2),BLUE,2,8,0);        
	       if(timp>0.001) cvLine(frame,cvPoint(y+j*scale,x),cvPoint(y+j*scale,x+2),RED,2,8,0);  
		   counter++;
         }
	    x=x+5; 
		y=240;
     }     
}
//--------------------------------------
void plot_matrix(void)         //                                                         PLOT MATRIX
{
	int i,j,x,y,counter, scale=5;  //5
	float timp;
	counter=0;
	y=170;
	x=10;
	for(i=0;i<matrix_row;i++)  // matrix_row=10
     { 
      for(j=0;j<matrix_column;j++)  // matrix_column=22
         {    
           timp=matrix_data[i][j];
           if(timp<0.5)cvLine(frame,cvPoint(y+j*scale,x),cvPoint(y+j*scale,x+2),GRAY,2,8,0);        
		   //if(timp>0.15) cvLine(frame,cvPoint(y+j*scale,x),cvPoint(y+j*scale,x+2),RED,2,8,0);     
		   if(timp>=0.5) cvLine(frame,cvPoint(y+j*scale,x),cvPoint(y+j*scale,x+2),WHITE,2,8,0);  
		   if(j==10) y=y+10;
		 }
	    x=x+5; 
		y=170;
	}
}
//-------------------------------------------//                                                   PLOT OUTPUTS
void plot_outputs(void)
{
int i,j,k,x,y;
float temp;
y=70;
x=5;

k=0;
	for(i=0;i<N_SAL;i++)	
	{
		k=x+4*i;
		cvLine(frame,cvPoint(k,y),cvPoint(k,y-c_salida.out[i]*60),RED,2,8,0);
		if(i==10) x=x+7;
	}
	cvLine(frame,cvPoint(k+15,y),cvPoint(k+15,y-green_column*80),GREEN,4,8,0);

y=140;
k=0;
	for(i=0;i<n_N_SAL;i++)	
	{
		k=x+5*i;
		cvLine(frame,cvPoint(k,y),cvPoint(k,y-n_c_salida.out[i]*60),CYAN,2,8,0);
		if(i==10) x=x+7;
	}

y=210;
k=0;
	for(i=0;i<n_N_SAL;i++)	
	{
		k=x+5*i;
		cvLine(frame,cvPoint(k,y),cvPoint(k,y-n_Target[i]*60),BLUE,2,8,0);
		if(i==10) x=x+7;
	}

}
//-------------------------------------------
//----------------------------------------------------------            
void plot_hidden_outputs(void)
{
 int y,x,i,temp;
  x=40;
  y=230; //210
  k=0;
	for(i=0;i<n_N_HID;i++)	
	{
		k=x+3*i;
		cvLine(frame,cvPoint(k,y),cvPoint(k,y-n_c_escondida.out[i]*60),GREEN,1,8,0);
		//if(i==10) x=x+7;
	}
    
}
//-------------------------------------------
void plot_winners(void)
{
int i,j,k,x,y;
float temp;
//color cvScalar;


y=180; //
x=5;

k=0;
	for(i=0;i<N_SAL;i++)	
	{
		temp=Winner[i]*80;
		cvLine(frame,cvPoint(k,y),cvPoint(k,y-temp),YELLOW,2,8,0);
		if(i==10) x=x+7;
	}
	
}
//---------------------------------------------
void move_roi(void)                                                                        //     move ROI
{
   int temp,i;
   
   for(i=0;i<N_SAL;i++) Target[i]=0.1; 

   temp=rand()%11;

   Target[temp]=0.9;
   
   temp=temp-5;
   x_roi= rest_x_roi+temp;
   
   temp=rand()%11;
   Target[temp+11]=0.9;
   temp=temp-5;
   y_roi=rest_y_roi+temp;    
           
} 


//-------------------------------------------------
void pump_fifo(void)         //              **    PUMP_FIFO_M   **    637
{
  int i,j;
  j=fifo_base-2;             // 10= numero de alementos en fifo , sigue 10-2

  for(i=0;i<fifo_base-1;i++)    //   
    {
     fifo[j+1] = fifo[j];
     j--;        
    }
  fifo[0] = red_column;

  main_column=0;
  for(i=0;i<fifo_base;i++) main_column = main_column+fifo[i];
  main_column= main_column/10.0;                      // fifo_base;

}
//---------------------------------------------
void pump_matrix(void)         //              **    PUMP_FIFO_M   **    
{
  int i,j,k;
  float temp;

  j=matrix_row-2;  

   
  for(i=0;i<matrix_row-1;i++)    //   matrix_row=10
    {
		for(k=0;k<matrix_column;k++)   //  matrix_column=22
		{
			matrix_data[j+1][k]= matrix_data[j][k];
		}
	  j--;        
    }

  //for(k=0;k<matrix_column;k++) matrix_data[0][k]= c_salida.out[k];       //  [fila][columna]
		for(k=0;k<matrix_column;k++)  // 22 
		{
			temp=c_salida.out[k];
			//temp=Winner[k];

			if(temp>main_barrier)  matrix_data[0][k]=0.9; //   cargo la primera fila con la salida de la red binarizada OJO!!
			else  matrix_data[0][k]= 0.1;                 //                 SALIDA PENTAVALUE PUEDE SER VENTAJOSA !! PROBAR !   
		}
}

//----------------------------------------------

//-------------------------------------------
void calculate_winners(void)              //                                                            CALCULAR BLUE_COLUMN     !!
{
  int i,j,lows;
  float max;
 
  for (i=0; i<N_SAL; i++) Winner[i]=0.1;

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
	Winner[j]=0.9;
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
	Winner[j]=0.9;
} 
//----------------------------------------------
void calculate_main_column(void)              //                                                    CALCULAR BLUE_COLUMN     !!
{
   int rest,i,j;
   float max;
  
       if((red_column>0.50)&&(main_column<0.9))  main_column=main_column+0.2;
		//if((red_column<0.50)&&(main_column>0.1)) main_column=main_column-0.00001;
} 
//----------------------------------------------------------
void charge_second_net(void)         //             LOAD SECOND NET
{
  int i,j,k;
/*  k=0; 
  for(j=0;j<n_N_HID;j++)    // n_N_HID = 37 !!  
    {
		for(i=0;i<n_N_IN;i++) // n_N_IN = 220
		{
			n_Entrada[k]=matrix_data[j][i];
			k++;
		}
    } */

  k=0; 
  for(j=0;j<matrix_row;j++)    // 10  
    {
		for(i=0;i<matrix_column;i++) // 22
		{
			n_Entrada[k]=matrix_data[j][i];      //  fila-columna
			k++;
		}
    }


  //red_column=n_c_salida.out[0];
}
//--------------------------------------------
void print_pesos(void)                                                   //  PRINT PESOS
{
int i,j,x,y;
float  temph, tempo;  



          //for(i=0;i<3;i++)  temph=
          printf("peso_hidden= %f\n",n_c_escondida.pesos[0][0]);
		  printf("peso_out   = %f\n",n_c_salida.pesos[0][0]);
		  printf("\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funcion: main																									//
// Description: Core del sistema																					//
// Inputs:																											//
// Output:																											//
///////////////////////////////////////////////////////aaaaaaaa///////////////////////////////////////////////////////

int main()
{
	float flotter;
	//                        INIT SCENE
	i=0;j=0;k=0;
	
	x_roi=110;
	y_roi=70;

	rest_x_roi=110;
	rest_y_roi=70;

	
	x_increment=1;
	fifo_base=10;
	for (i=0; i<N_SAL; i++) Target[i]=0.1;

	backpro_zone=2;     //                                    BACKPRO ZONE   zona en la red originaria 
	y_shake=1;
	y_shake_inc=1;

	foul=0.01;
	hit=0.01;
	master_target=0.1;
	coin=0;
	roi_count=0;
	inicializar_pesos();
	//trim_pesos();
	//n_inicializar_pesos();
	noise_flag=0;

	CargarPesos();
	
	n_CargarPesos();

	for(int j=0;j<matrix_row;j++ )
	for(int i=0;i<matrix_column;i++)
         {    
           matrix_data[i][j]=0.0;
         }

	//plot_feature();



	// Initialize capturing live feed from the camera
    CvCapture* capture = 0;
	//capture = cvCaptureFromCAM(1);
	capture = cvCaptureFromCAM(CV_CAP_DSHOW);

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
    //cvNamedWindow("Gray Video");
	cvNamedWindow("Color Video");
	cvNamedWindow("B&W");
	cvNamedWindow("ROI Re-draw");

	iLowThresHold=138;    //  148 home
	iHighThresHold=255;
  	
	while(true)                                                        //                        *****     LOOPS BEGUIN
    {
        // Will hold a frame captured from the camera

		//if(backpro_timer) 	move_roi();
				
        frame = cvQueryFrame(capture);	// de donde se va a capturar de que camara
		// If we couldn't grab a frame... quit
        if(!frame)
            break;
		// If this is the first frame, we need to initialize it
       if(im_gray == NULL)
        {
        	//im_gray   = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
			im_bw     = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
			tempFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
            im_ROI=cvCreateImage(cvSize(SIZE_ROI,SIZE_ROI),IPL_DEPTH_8U,1);//ROI
        }

		//*************************************************

		//cvCvtColor(frame,im_gray,CV_RGB2GRAY); //gray 
		im_bw = findEdges(frame, lowSliderPosition, highSliderPosition, 5); 
        get_ROI(x_roi,y_roi,im_bw); //copy ROI to input vector  // <<<<< ROI
        redraw_ROI(); //verify copied ROI

//------------------------------------------------     WORK

		plot_winners();             
		Gain=0.5;                                                                      //   HERE GAIN SETTING
		calcular_capa_escondida();
		Gain=1.5;
		calcular_capa_salida();

		calculate_winners();         // Heavens!    here was a tough BUG

		pump_matrix();

 		charge_second_net();
		n_calcular_capa_escondida();
		n_calcular_capa_salida();
//-------------------
		//if(backpro_timer) {n_Target[0]=master_target;n_corregir_pesos_all();}    //                HERE NEW STUFF

		if(backpro_timer==150) {n_Target[0]=0.1;n_Target[1]=0.9;n_Target[2]=0.1;}

		if(backpro_timer==100) {n_Target[0]=0.9;n_Target[1]=0.1;n_Target[2]=0.9;noise_flag=1;}
        
		if(backpro_timer==50) {noise_flag=0;backpro_timer--;n_Target[0]=0.1;n_Target[1]=0.1;n_Target[2]=0.1;}

		if(backpro_timer==10)
		                     {	
                               if(n_c_salida.out[1]>0.7) m_flag=0;
		                       if(m_flag) backpro_timer=152;
							   else backpro_timer=1;
		                     } 
		//if((n_c_salida.out[1]>0.5) && (backpro_timer<5)) {m_flag=0; backpro_timer=1;}


		if(backpro_timer>51) n_corregir_pesos_all(); 


		
		print_pesos();		
        calculate_main_column();
				
		//pump_fifo();

	
		plot_outputs();
		n_plot_feature();
		plot_matrix();
		plot_hidden_outputs();
		//plot_feature();

//-------------------------------------------		
		cvShowImage("Color Video",frame);
		//cvShowImage("Gray Video",im_gray);
		cvShowImage("B&W",im_bw);
		cvShowImage("ROI Re-draw",im_ROI);
//-------------------------------------------      //                	

		if(!noise_flag)
		   {
			x_roi=x_roi-winner_x+5;
			if(x_roi<1)	x_roi=110;   // rest_x_roi
			if(x_roi>(SIZE_FRAME_WIDTH-(SIZE_ROI+1)))	x_roi=110;
			
			y_roi=y_roi-(winner_y-11)+5;
			if(y_roi<1)	y_roi=70;   // rest_x_roi
			if(y_roi>(SIZE_FRAME_HEIGHT-(SIZE_ROI+1)))	y_roi=70;
			}
			
		if((!backpro_timer)&&(n_c_salida.out[1]>0.5)) Beep(1000,60);
//-------------------------------------------
		if(backpro_timer) backpro_timer--;

		trimmer_count++;
//-------------------------------------------
		cvReleaseImage(&im_gray);
		cvReleaseImage(&im_bw);
		cvReleaseImage(&im_ROI);

		int c = cvWaitKey(1);	//1ms

        switch(c)
        {
			case 'x':
			case 'X':
				 cvReleaseCapture(&capture);
			break;

			case 'll':
				if(iLowThresHold>0) iLowThresHold--;
			break;

			case 'L':
				if((iLowThresHold<255)&&(iLowThresHold<iHighThresHold)) iLowThresHold++;
			break;
			case 'l':
						n_limpiar_pesos();
						//for(i=0;i<300;i++) {x_curve[i]=0;}
			break;
			
			case 'k':
				if(iHighThresHold>0)	iHighThresHold--;
			break;

			case 'K':
				if(iHighThresHold<255)	iHighThresHold++;
			break;

			case 'q':
				    noise_flag=noise_flag^1;
			break;

			case 'y':
				if(y_roi<(SIZE_FRAME_HEIGHT-(SIZE_ROI+1)))	y_roi++;
			break;

			case 'u':
				if(y_roi>1)	y_roi--;
			break;

			case 'i':
				//if(x_roi>1)	x_roi--;
			break;
			case 'd':
					//if(x_roi<(SIZE_FRAME_WIDTH-(SIZE_ROI+1)))	x_roi++;
			break;

//------------------------------------------------------------------   NEW SWITCH
			case 'a':
						n_backpro_zone++;

						if(n_backpro_zone>2) n_backpro_zone=0;
								
			break;
			
			
			case 'b':
						if(!backpro_timer) backpro_timer=50;
						else backpro_timer=0;
								
			break;
			case 'g':
						n_limpiar_pesos();
			break;

			case 'h':
						//trim_pesos();
			break;


			case 'n':
				   //if(!backpro_timer) {backpro_timer=10;master_target=0.1;}  //n_backpro_zone=1;}     //0.0015
								
			break;

			case 'm':
				      //if(!backpro_timer) {backpro_timer=100;} 
				        m_flag = m_flag^1;
						if(m_flag) backpro_timer=152;
						else backpro_timer=1;
			break;
			
			case 'r':
						n_inicializar_pesos();
						//inicializar_pesos();
						n_plot_feature();
			break;
			case 'c':
						n_CargarPesos();
						n_plot_feature();
			break;

			case 's':
						n_SalvarPesos();
						printf("n_pesos salvados \n",winner_y, n_backpro_zone);
						plot_feature();
			break;
			case 't':
						flig=flig^1;
						if(flig) master_target=0.9;
						else master_target=0.1;				
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
		
		//cvReleaseImage(&imgRgbAll);


	}
}

// La retina se mueve libremente por la pantalla
// dispara 50 ciclos de backpro y al terminarlos la red sale de l aprendizaje

/*
Octubre 10 2013

PUnto previo importante.
Hay que definir un metodo de aprendizaje rapido
que haga que el GBT (generic biased tracking) 
se aferre a una area selecionada y minimize
su deriva media = promedio de los desplazamientos del roi.
Esto podria facilitar el entrenamiento de la red concepto   YES!


Estrategia a probar en Red Concepto

1.- Captura roi as usual
2.- Genera nueva roi con la correcion dada por la red (winers x,y)
3.- Esta nueva imagen deberia ser estable o quasi estable
4.- Elimina los bits que cambien demasiado
5.- Forma sinapsis de la red "objeto" solo con los bits que permanecen estables
    durante el seguimiento dinamico estructurado (Structured Dinamic Tracking or SDT
6.- Entrena a una nueva red que capture los bits que permanecen estable en la nueva roi

Tema importante
Si se utiizan solo los bits que permanecen exitados entonces sera facil enganar a la red
con figuras ajenas al proceso. Deben participar otros bits. Opciones: 
		a) los que permanecen oscuros (pudieran ser demasiados)
		b) el nivel DC de la red
		c) usar solo los bits que cambien entre frame y frame !! => Esto parece interesanta
																porque usa pocos bits y ellos
																son muy particulares a la imagen !!!
																EUREKA ??

En principio como la imagen es simple estable se puede aprender rapido (alto Eta)

Finalmente la red memoriza el patron de desplazamiento de una
imagen cualquiera sometida a seguimiento dinamico estructurado









*/