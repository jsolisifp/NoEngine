#ifndef __NOE_SISTEMA__
#define __NOE_SISTEMA__

namespace NOESistema
{
	void IniciaSistema();
	void ActualizaSistema();
	void FinalizaSistema();
	
	// Library functions
	
	int ObtenTiempoDesdeActualizacion();
	int ObtenNumeroAleatorio(int minimo, int maximo);
	
}

#endif