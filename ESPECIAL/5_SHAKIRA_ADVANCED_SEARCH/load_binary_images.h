
//================================================================================================================

int CargarImagenes(void)
{
	int i, aux;
	float numero;
	FILE *in_file;

	if ((in_file = fopen ("bin_chaplin.dat", "rt")) == NULL) aux=0;
	
	else
	{
		fseek(in_file, 0, SEEK_SET);
    
			for (i=0; i<10000; i++)
			{
				fscanf(in_file, "%f", &numero);
				copy[i][0]=numero;
			}
	}
	fclose(in_file);
    
    
    //FILE *in_file;
	aux=0;
	if ((in_file = fopen ("bin_shakira.dat", "rt")) == NULL)
		aux=1;
	else
	{
		fseek(in_file, 0, SEEK_SET);
    
			for (i=0; i<10000; i++)
			{
				fscanf(in_file, "%f", &numero);
				copy[i][1]=numero;
			}
	}
	fclose(in_file);
    
 //-------------------------------------------   
    aux=0;
	if ((in_file = fopen ("bin_ironman.dat", "rt")) == NULL)
		aux=1;
	else
	{
		fseek(in_file, 0, SEEK_SET);
    
			for (i=0; i<10000; i++)
			{
				fscanf(in_file, "%f", &numero);
				copy[i][2]=numero;
			}
	}
	fclose(in_file);
	
    /*moveto(400,10);
    setcolor(WHITE);
    settextstyle(3, HORIZ_DIR, 1);
    outtext("Imagen Cargada");   */
    return(aux);
}
//-----------------------------------------------
/*int SalvarPesos(void)
{
	int k, i, aux;
	float numero;
	FILE *out_file;
	aux=0;
	if ((out_file = fopen ("S_GRID_HID.DAT", "wt")) == NULL)
		aux=1;
	else
	{
		fseek(out_file, 0, SEEK_SET);
		for (k=0; k<N_HID; k++)
			for (i=0; i<N_IN; i++)
			{
				numero=c_escondida.pesos[k][i];
				fprintf(out_file, "%f %c", numero,'\n');
			}
		  }
	if ((out_file = fopen ("S_GRID_OUT.DAT", "wt")) == NULL)
		aux=aux+2;
	else
	{
		fseek(out_file, 0, SEEK_SET);
		for (k=0; k<N_SAL; k++)
			for (i=0; i<N_HID; i++)
			{
              numero=c_salida.pesos[k][i];
              fprintf(out_file, "%f %c", numero, '\n');
			}
    }
	fclose(out_file);
    moveto(20,20);
    setcolor(WHITE);
    settextstyle(3, HORIZ_DIR, 1);
    outtext("Pesos Salvados ");
	return(aux);
}
*/
//-----------------------------------------------
