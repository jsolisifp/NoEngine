#ifndef __NOE_ENTRADA__
#define __NOE_ENTRADA__

namespace NOEEntrada
{
	extern sfKeyCode tecladoCodigosSFTecla[];

	extern bool tecladoTeclasPulsadas[];

	extern bool tecladoTeclasPulsadasAnteriores[];

	void IniciaEntrada();
	void ActualizaEntrada();
	void FinalizaEntrada();

	// Library functions 
	
	const char *ObtenEntrada();
	void LimpiaEntrada();
	
}

#endif
