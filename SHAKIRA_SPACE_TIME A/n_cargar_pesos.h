//================================================================================================================

int n_CargarPesos(void)
{
	int k, i, aux;
	float numero;
	FILE *in_file;
	aux=0;
	if ((in_file = fopen ("second_net_hid.dat", "rt")) == NULL)
		aux=1;
	else
	{
		fseek(in_file, 0, SEEK_SET);
		for (k=0; k<n_N_HID; k++)
			for (i=0; i<n_N_IN; i++)
			{
				fscanf(in_file, "%f", &numero);
				n_c_escondida.pesos[k][i]=numero;
			}
	}
	fclose(in_file);
	if ((in_file = fopen ("second_net_out.dat", "rt")) == NULL)
		aux=aux+2;
	else
	{
		fseek(in_file, 0, SEEK_SET);
		for (k=0; k<n_N_SAL; k++)
			for (i=0; i<n_N_HID; i++)
			{
				fscanf(in_file, "%f", &numero);
				n_c_salida.pesos[k][i]=numero;
			}
	}
	fclose (in_file);
	
 
    return(aux);
}
//-----------------------------------------------
int n_SalvarPesos(void)
{
	int k, i, aux;
	float numero;
	FILE *out_file;
	aux=0;
	if ((out_file = fopen ("second_net_hid.dat", "wt")) == NULL)
		aux=1;
	else
	{
		fseek(out_file, 0, SEEK_SET);
		for (k=0; k<n_N_HID; k++)
			for (i=0; i<n_N_IN; i++)
			{
				numero=n_c_escondida.pesos[k][i];
				fprintf(out_file, "%f %c", numero,'\n');
			}
		  }
	if ((out_file = fopen ("second_net_out.dat", "wt")) == NULL)
		aux=aux+2;
	else
	{
		fseek(out_file, 0, SEEK_SET);
		for (k=0; k<n_N_SAL; k++)
			for (i=0; i<n_N_HID; i++)
			{
              numero=n_c_salida.pesos[k][i];
              fprintf(out_file, "%f %c", numero, '\n');
			}
    }
	fclose(out_file);
    
	return(aux);
}

//-----------------------------------------------
