#include "Modulos/NOEComun.hpp"

namespace NOEPrincipal
{
	bool Inicia()
	{
		printf("----------------------------------------------\n");
		printf("  %s Version %d.%d\n", noeNombreLibreria, noeVersionMajor, noeVersionMinor);
		printf("----------------------------------------------\n");
		printf("\n");
        
		IniciaConfiguracion();
		printf("Configuracion iniciada\n");
		IniciaRecursos();
		printf("Recursos iniciados\n");
		IniciaPantalla();
		printf("Pantalla iniciada\n");
		IniciaEntrada();
		printf("Entrada iniciada\n");
		IniciaControles();
		printf("Controles iniciados\n");
		IniciaGraficos();
		printf("Graficos iniciados\n");
		IniciaSonido();
		printf("Sonidos iniciados\n");
		IniciaDatos();
		printf("Datos iniciados\n");        
		IniciaColisiones();
		printf("Colisiones iniciadas\n");
		IniciaSistema();
		printf("Sistema iniciado\n");
        printf("\n");
		
		return true;
		
	}

	void Actualiza()
	{

		// Procesar eventos de ventana
		
		sfRenderWindow *ventana = ObtenVentana();
		
		sfEvent event;
		
		while (sfRenderWindow_pollEvent(ventana, &event))
		{
			/* Close ventana : exit */
			//if (event.type == sfEvtClosed)
			//	sfRenderWindow_close(ventana);
		}
		
		ActualizaControles();
		ActualizaEntrada();
		ActualizaSistema();

		
	}


	void Finaliza()
	{

		// Liberar entrada
		
		FinalizaEntrada();

		// Liberar recursos

        FinalizaDatos();
		FinalizaSonido();
		FinalizaPantalla();
		FinalizaSistema();	
			
		
	}

	void Espera(int tiempo)
	{
		sfTime t = sfMilliseconds(tiempo);
		sfSleep(t);
		
	}








}