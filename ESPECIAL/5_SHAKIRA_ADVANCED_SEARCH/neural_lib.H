//============================================================================
//    Constantes  
//============================================================================
#define N_IN    10000     //Numero de neurona de entrada
#define N_OUT   22         // 11 neuronas por eje, NEURONAS SALIDAS 
#define N_HID   41        // 19      //NEURONAS ESCONlDIDAS MAXIMAS

//-------------------------------------

//=======================================================================================================================
//  DEFINICION DE ESTRUCTURA DE LA Red
//=======================================================================================================================
float  Mid=0.5, Eta=0.25, Gain=0.250;  //Eta anterior 0.15, gain previo 0.25
int  four,five;
float Target[N_OUT];

float Entrada[N_IN];

struct  capa_salida
	  {
	   float  pesos[N_OUT][N_HID];
	   float  error[N_OUT];
	   float  out[N_OUT];
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
  float tmp;
  tmp=random(100);      // original 100
  tmp=(tmp-50)/100;
  return tmp;
}
//----------------------------------
void pump_five(void)             ///                       PUMP !!
{
 five++;  
 if(five>3)five=0;    
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
  for (k=0; k<N_OUT; k++)
    for (i=0; i<N_HID; i++)
    { 
      //c_salida.pesos[k][i]=0;  
      c_salida.pesos[k][i]=peso_aleatorio();
    }  
}
//------------------------------------

//------------------------------------
void corregir_pesos(void)                     //   BACK PROPAGATION
{
  int k,i;
  float sum;

  // Calculo de error para la capa de salida
  for (k=0; k<N_OUT; k++)
	c_salida.error[k]=c_salida.out[k]*(1-c_salida.out[k])*(Target[k]-c_salida.out[k]);

  // Calculo de error para la capa escondida
  for (k=0; k<N_HID; k++)
  {
    sum=0;
    for (i=0; i<N_OUT; i++)
      sum=sum+c_salida.error[i]*c_salida.pesos[i][k];
    c_escondida.error[k]=c_escondida.out[k]*(1-c_escondida.out[k])*sum;
  }

  // Correccion de pesos para la capa de salida
  five=0;
  for (k=0; k<N_OUT; k++)
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
  five=0;
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
  five=0;
  for (k=0; k<N_OUT; k++)
  {
    c_salida.out[k]=0;
    for (i=0; i<N_HID; i++)
        {
            c_salida.out[k]=c_salida.out[k]+c_salida.pesos[k][i]*c_escondida.out[i];
           
        }
    c_salida.out[k]=sigmoide(c_salida.out[k]);
  }
}
//-------------------------------------
void trim_hidden_weights(void)
{
   int k,i;
  // Limpiar de pesos para la capa escondida
    for (k=0; k<N_HID ;k++)
    {
        //xtal=0;    
        for (i=0; i<N_IN; i++)
        {
          c_escondida.pesos[k][i]=peso_aleatorio()*0.001;
          //c_escondida.pesos[k][i]=0;
          //pump_xtal();    
        }    
    }    
}

//-------------------------------------



