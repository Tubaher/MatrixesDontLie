//============================================================================
//    RED DE SEGUNDA ETAPA EN SUPERTRACKER  
//============================================================================
#define n_N_IN    220     //Numero de neurona de entrada
#define n_N_SAL   3       // NEURONAS SALIDAS 
#define n_N_HID   37      // NEURONAS ESCONlDIDAS

//-------------------------------------

//=======================================================================================================================
//  DEFINICION DE ESTRUCTURA DE LA Red
//=======================================================================================================================
float  n_Mid=0.5, n_Eta=0.25, n_Gain=0.5;    // gain_old=1.5, eta_old=0.25    
                                        // DE 1.5 PARA RTRAINING A 1.0 PARA TRACKING. APARECIO SUB RECONOCIMIENTO
int  n_four,n_xtal, n_five;
int  jump;

float n_Target[N_SAL];

int n_backpro_zone;            //                     unuihk

float n_Entrada[N_IN];

struct  n_capa_salida
	  {
	   float  pesos[n_N_SAL][n_N_HID];
	   float  error[n_N_SAL];
	   float  out[n_N_SAL];
      } n_c_salida;

struct  n_capa_escondida
	  {
	   float  pesos[n_N_HID][n_N_IN];
	   float  error[n_N_HID];
	   float  out[n_N_HID];
      } n_c_escondida;

//===========================================================================================================================
//  RUTINAS DE CALCULO DE LA RED
//===========================================================================================================================

float n_peso_aleatorio(void)
{
  float temp;

  temp=rand()%100;
  //tmp=random(100);      // original 100
  temp=(temp-50)/100;     // /100
  return temp;
}
//----------------------------------
void n_pump_xtal(void)             ///                       PUMP !!
{
 n_xtal++;  
 if(n_xtal>2)n_xtal=0;        // OJOJ   OOJOJ  !!!!   Habia un 2
}
//-------------------------------------                 //    hasta aqui va n_.....
void n_inicializar_pesos(void)
{
   int k,i;
  // Inicializacion de pesos para la capa escondida
    for (k=0; k<n_N_HID ;k++)
    for (i=0; i<n_N_IN; i++)
    {
      n_c_escondida.pesos[k][i]=n_peso_aleatorio();
    }    
        
  // Inicializacion de pesos para la capa de salida      
  for (k=0; k<n_N_SAL; k++)
    for (i=0; i<n_N_HID; i++)
    { 
      //c_salida.pesos[k][i]=0;  
      n_c_salida.pesos[k][i]=n_peso_aleatorio();
    }  
}
//------------------------------------
void n_limpiar_pesos(void)
{
   int k,i;
  // Limpiar de pesos para la capa escondida
    jump=0;
    for (k=0; k<n_N_HID ;k++)
    {
        //n_xtal=0;    
        for (i=0; i<n_N_IN; i++)
        {
          n_c_escondida.pesos[k][i]=n_peso_aleatorio()*0.001;    //c_escondida.pesos[k][i]=0;
          //n_pump_xtal();    
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
void n_corregir_pesos_all(void)
{
  int k,i;
  float sum;

  // Calculo de error para la capa de salida
  for (k=0; k<n_N_SAL; k++)
	n_c_salida.error[k]=n_c_salida.out[k]*(1-n_c_salida.out[k])*(n_Target[k]-n_c_salida.out[k]);

  // Calculo de error para la capa escondida
  for (k=0; k<n_N_HID; k++)
  {
    sum=0;
    for (i=0; i<n_N_SAL; i++)
      sum=sum+n_c_salida.error[i]*n_c_salida.pesos[i][k];
    n_c_escondida.error[k]=n_c_escondida.out[k]*(1-n_c_escondida.out[k])*sum;
  }

  // Correccion de pesos para la capa de salida
  n_five=0;
  for (k=0; k<n_N_SAL; k++)
    for (i=0; i<n_N_HID; i++)
    {   
      n_c_salida.pesos[k][i]=n_c_salida.pesos[k][i]+n_Eta*n_c_salida.error[k]*n_c_escondida.out[i];
          
    }
  // Correccion de pesos para la capa escondida */
    five=0;
    for (k=0; k<n_N_HID; k++)
    for (i=0; i<n_N_IN; i++)
    { 
       n_c_escondida.pesos[k][i]= n_c_escondida.pesos[k][i]+n_Eta*n_c_escondida.error[k]*n_Entrada[i];
      
    } 
    n_five=0;    
}
//------------------------------------
void n_corregir_pesos_zone(void)
{
  int k,i;
  float sum;

  // Calculo de error para la capa de salida
  for (k=0; k<n_N_SAL; k++)
	n_c_salida.error[k]=n_c_salida.out[k]*(1-n_c_salida.out[k])*(n_Target[k]-n_c_salida.out[k]);

  // Calculo de error para la capa escondida
  for (k=0; k<n_N_HID; k++)
  {
    sum=0;
    for (i=0; i<n_N_SAL; i++)
      sum=sum+n_c_salida.error[i]*n_c_salida.pesos[i][k];
    n_c_escondida.error[k]=n_c_escondida.out[k]*(1-n_c_escondida.out[k])*sum;
  }

  // Correccion de pesos para la capa de salida
  //xtal=backpro_zone;
/*
//$$$$$$$$$$$$$$$$$$$          OJO ESTA VERSION NO CORRIGE LOS PESOS DE SALIDA, SOLO LOS ESCONDIDOS

  jump=0;
  if(n_backpro_zone==1)    ///   OJO  solo corrige pesos de salida si esta HELIPAD
  {
    for (k=0; k<n_N_SAL; k++)
    {  
     //n_xtal=0;  
     for (i=0; i<N_HID; i++)
      {   
       //if(xtal==backpro_zone) {c_salida.pesos[k][i]=c_salida.pesos[k][i]+Eta*c_salida.error[k]*c_escondida.out[i];}
        n_c_salida.pesos[k][i]=n_c_salida.pesos[k][i]+n_Eta*n_c_salida.error[k]*n_c_escondida.out[i];
        //n_pump_xtal();     
      }
    }  
  }
	
*/	
	
	// Correccion de pesos para la capa escondida */
    //xtal=ring;
   jump=0;
    for (k=0; k<n_N_HID; k++)
    {
     n_xtal=0;    
     for (i=0; i<n_N_IN; i++)
      { 
       if(n_xtal==n_backpro_zone) {n_c_escondida.pesos[k][i]=n_c_escondida.pesos[k][i]+n_Eta*n_c_escondida.error[k]*n_Entrada[i];}
       n_pump_xtal(); 
      }
    }      
}

//-------------------------------------
float n_sigmoide(float x)
{
  float out;

  if (x>30) x=30;
  if (x<-30) x=-30;
  out=pow((1+exp(-Gain*x)),-1);
  return(out);
}

//-------------------------------------
void n_calcular_capa_escondida(void)
{
  int k,i;
  xtal=0;
  for (k=0; k<n_N_HID; k++)
   {
    n_c_escondida.out[k]=0;
    for (i=0; i<n_N_IN; i++)
        {
         n_c_escondida.out[k]=n_c_escondida.out[k]+n_c_escondida.pesos[k][i]*n_Entrada[i];
            
        }
    n_c_escondida.out[k]=n_sigmoide(n_c_escondida.out[k]);
   }
}

//-------------------------------------
void n_calcular_capa_salida(void)
{
  int k,i;
  n_xtal=0;
  for (k=0; k<n_N_SAL; k++)
  {
    n_c_salida.out[k]=0;
    for (i=0; i<n_N_HID; i++)
        {
            n_c_salida.out[k]=n_c_salida.out[k]+n_c_salida.pesos[k][i]*n_c_escondida.out[i];
        }
    n_c_salida.out[k]=n_sigmoide(n_c_salida.out[k]);
    if(n_c_salida.out[k]<0.1) n_c_salida.out[k]=0.1; 
	//if(k<10) c_salida.out[k]=0.05; 
  }
}

//-------------------------------------
void n_backpropagation(void)
{
  n_calcular_capa_escondida();
  n_calcular_capa_salida();
  //corregir_pesos(0);
}

//===========================
void n_trim_pesos(void)
{
   int k,i;
   float temp;
     
     
  // trim de pesos para la capa escondida
    for (k=0; k<n_N_HID ;k++)
	{
		n_xtal=0;
		for (i=0; i<n_N_IN; i++)
		{
		  if(n_xtal==n_backpro_zone)
			{
				temp= n_c_escondida.pesos[k][i];
				if(temp<0) temp=-temp;  
				if(temp<0.025) n_c_escondida.pesos[k][i]=n_peso_aleatorio()*0.0;        //0.0001*;  ///0.01
			}
			n_pump_xtal();	
		} 
	}
}