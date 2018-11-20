//============================================================================
//    Constantes  
//============================================================================
#define N_IN    10000     //Numero de neurona de entrada
#define N_SAL   11        // 11 neuronas por eje, NEURONAS SALIDAS 
#define N_HID   19        // 19      //NEURONAS ESCONlDIDAS MAXIMAS

//-------------------------------------

//=======================================================================================================================
//  DEFINICION DE ESTRUCTURA DE LA Red
//=======================================================================================================================
float  Mid=0.5, Eta=0.25, Gain=1.5;     //1.5     OJO EL PROBLEMA DE FRONTERA SE RESOLVIO BAJANDO LA GANANCIA
                                        // DE 1.5 PARA RTRAINING A 1.0 PARA TRACKING. APARECIO SUB RECONOCIMIENTO
int  four,xtal, five;
float Target[N_SAL];

int backpro_zone;            //                     unuihk

float Entrada[N_IN];

struct  capa_salida
	  {
	   float  pesos[N_SAL][N_HID];
	   float  error[N_SAL];
	   float  out[N_SAL];
      } c_salida;

struct  capa_escondida
	  {
	   float  pesos[N_HID][N_IN];
	   float  error[N_HID];
	   float  out[N_HID];
      } c_escondida;

//===========================================================================================================================
//  RUTINAS DE CALCULO DE LA RED
//===========================================================================================================================

float peso_aleatorio(void)
{
  float temp;

  temp=rand()%100;
  //tmp=random(100);      // original 100
  temp=(temp-50)/100;
  return temp;
}
//----------------------------------
void pump_xtal(void)             ///                       PUMP !!
{
 xtal++;  
 if(xtal>2)xtal=0;    
}
//-------------------------------------
void inicializar_pesos(void)
{
   int k,i;
  // Inicializacion de pesos para la capa escondida
    for (k=0; k<N_HID ;k++)
    for (i=0; i<N_IN; i++)
    {
      c_escondida.pesos[k][i]=peso_aleatorio();
    }    
        
  // Inicializacion de pesos para la capa de salida      
  for (k=0; k<N_SAL; k++)
    for (i=0; i<N_HID; i++)
    { 
      //c_salida.pesos[k][i]=0;  
      c_salida.pesos[k][i]=peso_aleatorio();
    }  
}
//------------------------------------
void limpiar_pesos(void)
{
   int k,i;
  // Limpiar de pesos para la capa escondida
    for (k=0; k<N_HID ;k++)
    {
        xtal=0;    
        for (i=0; i<N_IN; i++)
        {
          if(xtal==backpro_zone)c_escondida.pesos[k][i]=peso_aleatorio()*0.001;    //c_escondida.pesos[k][i]=0;
          pump_xtal();    
        }    
    }    
  // Limpiar pesos para la capa de salida      
   /* for (k=0; k<N_SAL; k++)
    { 
        xtal=0;     
        for (i=0; i<N_HID; i++)
        { 
          if(xtal==backpro_zone) c_salida.pesos[k][i]=peso_aleatorio()*0.001;     ///c_salida.pesos[k][i]=0;
          pump_xtal();    
        }
    } */       
}
//------------------------------------
void corregir_pesos_all(void)
{
  int k,i;
  float sum;

  // Calculo de error para la capa de salida
  for (k=0; k<N_SAL; k++)
	c_salida.error[k]=c_salida.out[k]*(1-c_salida.out[k])*(Target[k]-c_salida.out[k]);

  // Calculo de error para la capa escondida
  for (k=0; k<N_HID; k++)
  {
    sum=0;
    for (i=0; i<N_SAL; i++)
      sum=sum+c_salida.error[i]*c_salida.pesos[i][k];
    c_escondida.error[k]=c_escondida.out[k]*(1-c_escondida.out[k])*sum;
  }

  // Correccion de pesos para la capa de salida
  five=0;
  for (k=0; k<N_SAL; k++)
    for (i=0; i<N_HID; i++)
    {   
      c_salida.pesos[k][i]=c_salida.pesos[k][i]+Eta*c_salida.error[k]*c_escondida.out[i];
          
    }
  // Correccion de pesos para la capa escondida */
    five=0;
    for (k=0; k<N_HID; k++)
    for (i=0; i<N_IN; i++)
    { 
       c_escondida.pesos[k][i]= c_escondida.pesos[k][i]+Eta*c_escondida.error[k]*Entrada[i];
      
    } 
    five=0;    
}
//------------------------------------
void corregir_pesos_zone(void)
{
  int k,i;
  float sum;

  // Calculo de error para la capa de salida
  for (k=0; k<N_SAL; k++)
	c_salida.error[k]=c_salida.out[k]*(1-c_salida.out[k])*(Target[k]-c_salida.out[k]);

  // Calculo de error para la capa escondida
  for (k=0; k<N_HID; k++)
  {
    sum=0;
    for (i=0; i<N_SAL; i++)
      sum=sum+c_salida.error[i]*c_salida.pesos[i][k];
    c_escondida.error[k]=c_escondida.out[k]*(1-c_escondida.out[k])*sum;
  }

  // Correccion de pesos para la capa de salida
  //xtal=backpro_zone;
    for (k=0; k<N_SAL; k++)
    {  
     xtal=0;  
     for (i=0; i<N_HID; i++)
      {   
       //if(xtal==backpro_zone) {c_salida.pesos[k][i]=c_salida.pesos[k][i]+Eta*c_salida.error[k]*c_escondida.out[i];}
        c_salida.pesos[k][i]=c_salida.pesos[k][i]+Eta*c_salida.error[k]*c_escondida.out[i];
        pump_xtal();     
      }
    }  
  // Correccion de pesos para la capa escondida */
    //xtal=ring;
    for (k=0; k<N_HID; k++)
    {
     xtal=0;    
     for (i=0; i<N_IN; i++)
      { 
       if(xtal==backpro_zone) {c_escondida.pesos[k][i]= c_escondida.pesos[k][i]+Eta*c_escondida.error[k]*Entrada[i];}
       
       pump_xtal(); 
      }
    }      
}

//-------------------------------------
float sigmoide(float x)
{
  float out;

  if (x>30) x=30;
  if (x<-30) x=-30;
  out=pow((1+exp(-Gain*x)),-1);
  return(out);
}

//-------------------------------------
void calcular_capa_escondida(void)
{
  int k,i;
  xtal=0;
  for (k=0; k<N_HID; k++)
   {
    c_escondida.out[k]=0;
    for (i=0; i<N_IN; i++)
        {
         c_escondida.out[k]=c_escondida.out[k]+c_escondida.pesos[k][i]*Entrada[i];
            
        }
    c_escondida.out[k]=sigmoide(c_escondida.out[k]);
   }
}

//-------------------------------------
void calcular_capa_salida(void)
{
  int k,i;
  xtal=0;
  for (k=0; k<N_SAL; k++)
  {
    c_salida.out[k]=0;
    for (i=0; i<N_HID; i++)
        {
            c_salida.out[k]=c_salida.out[k]+c_salida.pesos[k][i]*c_escondida.out[i];
        }
    c_salida.out[k]=sigmoide(c_salida.out[k]);
    if(c_salida.out[k]<0.1) c_salida.out[k]=0.1;  
  }
}

//-------------------------------------
void Backpropagation(void)
{
  calcular_capa_escondida();
  calcular_capa_salida();
  //corregir_pesos(0);
}

//===========================



