double calcul_entropy(double nbsetosa, double nbversicolor, double nbvirginica, double sample)
{
	double sol=0.0;
	
	if(nbsetosa==0.0 && nbversicolor==0.0 && nbvirginica==0.0)
	{
		sol = 100.0; /*le calcul de l'entropy est impossible. On retourne une valeur improbable positive*/
	}
	/****** On doit tester tous les cas, car le logarithme est défini que dans R+* ******/
	else if(nbsetosa!=0.0 && nbversicolor!=0.0 && nbvirginica!=0.0)
	{
		sol = -((nbsetosa/sample)*log(nbsetosa/sample))/log(2) -((nbversicolor/sample)*log(nbversicolor/sample))/log(2) -((nbvirginica/sample)*log(nbvirginica/sample))/log(2);
	}
	else if(nbsetosa!=0.0 && nbversicolor!=0.0 && nbvirginica==0.0)
	{
		sol = -((nbsetosa/sample)*log(nbsetosa/sample))/log(2) -((nbversicolor/sample)*log(nbversicolor/sample))/log(2);
	}
	else if(nbsetosa!=0.0 && nbversicolor==0.0 && nbvirginica!=0.0)
	{
		sol = -((nbsetosa/sample)*log(nbsetosa/sample))/log(2) -((nbvirginica/sample)*log(nbvirginica/sample))/log(2);
	}
	else if(nbsetosa!=0.0 && nbversicolor==0.0 && nbvirginica==0.0)
	{
		sol = -((nbsetosa/sample)*log(nbsetosa/sample))/log(2);
	}
	else if(nbsetosa==0.0 && nbversicolor!=0.0 && nbvirginica!=0.0)
	{
		sol = -((nbversicolor/sample)*log(nbversicolor/sample))/log(2) -((nbvirginica/sample)*log(nbvirginica/sample))/log(2);
	}
	else if(nbsetosa==0.0 && nbversicolor!=0.0 && nbvirginica==0.0)
	{
		sol = -((nbversicolor/sample)*log(nbversicolor/sample))/log(2);
	}
	else if(nbsetosa==0.0 && nbversicolor==0.0 && nbvirginica!=0.0)
	{
		sol = -((nbvirginica/sample)*log(nbvirginica/sample))/log(2);
	}
	sol = fabs(sol);

	return sol;
}



double initialisation_calcul_entropy(double nbsetosa, double nbversicolor, double nbvirginica, double sample)
{
	double entropy = 0.0;

	entropy = calcul_entropy(nbsetosa, nbversicolor, nbvirginica, sample);
	
	if(entropy >= 0.00)
	{
		return entropy;
	}
	else
	{
		entropy = 100.00;
		return entropy; //si l'entropy calculé est négative, on retourne une valeur improbable positive
	}
	return entropy;
}
