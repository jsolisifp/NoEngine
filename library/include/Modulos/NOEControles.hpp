#ifndef __NOE_CONTROLES__
#define __NOE_CONTROLES__

namespace NOEControles
{
	

	extern int ratonX;
	extern int ratonY;

	void IniciaControles();
	void ActualizaControles();
	
	// Library functions
	
	bool ObtenTeclaPulsada(NOE_Tecla tecla);
	bool ObtenTeclaAbajo(NOE_Tecla tecla);
	bool ObtenTeclaArriba(NOE_Tecla tecla);
	int ObtenPosicionRatonX();
	int ObtenPosicionRatonY();
	int ObtenDesplazamientoRatonX();
	int ObtenDesplazamientoRatonY();
	bool ObtenBotonRaton(NOE_BotonRaton boton);
	bool ObtenBotonRatonAbajo(NOE_BotonRaton boton);
	bool ObtenBotonRatonArriba(NOE_BotonRaton boton);

}

#endif
