#include "NoEntiendo.hpp"
#include "NoEntiendoDB.hpp"
#include <stdio.h>
#include <math.h>

bool jugando;

int diferenciasGrabadas;
int diferenciasEncontradas;

int main()
{
	NOE_Inicia();
    NOEDB_Inicia();
    
    diferenciasEncontradas = 0;
    jugando = true;
    
    if(NOEDB_ExisteTabla("DIFERENCIAS"))
    {
        NOEDB_ModificaFilas("DIFERENCIAS");
        NOEDB_ModificaEntero("ENCONTRADA",0);
        NOEDB_FinModificaFilas();
        
        NOEDB_ImprimeTabla("DIFERENCIAS");
    }
	
	//NOE_ReproduceMusica(0, 100, 100);

	while(!NOE_ObtenTeclaPulsada(NOE_TECLA_ESCAPE))
	{
		NOE_Actualiza();

        if(jugando)
        {
            // Estado de juego
            
            if(NOE_ObtenBotonRatonAbajo(NOE_BOTONRATON_PRINCIPAL))
            {
                bool encontrado;
                int idEncontrado;
                int ratonX;
                int ratonY;
                
                ratonX = NOE_ObtenPosicionRatonX();
                ratonY = NOE_ObtenPosicionRatonY();
                
                encontrado = false;
                idEncontrado = 0;
                
                NOEDB_BuscaFilas("DIFERENCIAS");
                NOEDB_BuscaSiEntero("ENCONTRADA", NOEDB_OPERADOR_IGUAL, 0);
                NOEDB_FinBuscaFilas();
                
                while(NOEDB_LeeResultado())
                {
                    int coordX;
                    int coordY;
                    
                    coordX = NOEDB_LeeEntero("COORDX");
                    coordY = NOEDB_LeeEntero("COORDY");
                    
                    if(abs(coordX - ratonX) < 5 && abs(coordY - ratonY) < 5)
                    {
                        idEncontrado = NOEDB_LeeClavePrimaria();
                        encontrado = true;
                    }
                    
                    NOEDB_FinLeeResultado();
                }
                
                if(encontrado)
                {
                    NOEDB_ModificaFilas("DIFERENCIAS");
                    NOEDB_ModificaSiClavePrimaria(idEncontrado);
                    NOEDB_ModificaEntero("ENCONTRADA", 1);
                    NOEDB_FinModificaFilas();
                    
                    NOEDB_ImprimeTabla("DIFERENCIAS");
                    
                    diferenciasEncontradas = diferenciasEncontradas + 1;
                    
                    printf("Encontradas %d\n", diferenciasEncontradas);
                }
                
            }            
            
            if(NOE_ObtenTeclaAbajo(NOE_TECLA_G))
            {
                if(NOEDB_ExisteTabla("DIFERENCIAS"))
                {
                    NOEDB_EliminaTabla("DIFERENCIAS");
                }
                
                NOEDB_DefineTabla("DIFERENCIAS", 0);
                NOEDB_DefineEntero("COORDX");
                NOEDB_DefineEntero("COORDY");
                NOEDB_DefineEntero("ENCONTRADA");
                NOEDB_FinDefineTabla();
                
                NOEDB_ImprimeTabla("DIFERENCIAS");
                
                diferenciasGrabadas = 0;
                               
                printf("Pasamos a grabar\n");
                jugando = false;
            }
        }
        else
        {
            // Estado de grabacion
            
            if(NOE_ObtenBotonRatonAbajo(NOE_BOTONRATON_PRINCIPAL) && diferenciasGrabadas < 5)
            {
                
                int x;
                int y;
                
                x = NOE_ObtenPosicionRatonX();
                y = NOE_ObtenPosicionRatonY();
                
                NOEDB_InsertaFila("DIFERENCIAS");
                NOEDB_InsertaEntero("COORDX", x);
                NOEDB_InsertaEntero("COORDY", y);
                NOEDB_InsertaEntero("ENCONTRADA", 0);
                NOEDB_FinInsertaFila();
                
                NOEDB_ImprimeTabla("DIFERENCIAS");
                
                diferenciasGrabadas = diferenciasGrabadas + 1;
                
                printf("Diferencias grabadas %d\n", diferenciasGrabadas);
                
            }

            if(NOE_ObtenTeclaAbajo(NOE_TECLA_J))
            {
                printf("Pasamos a jugar\n");
                diferenciasEncontradas = 0;
                jugando = true;
            }            
            
        }
        
		NOE_LimpiaPantalla(0, 0, 0);
		NOE_DibujaDecorado(0);
		
		
		NOE_MuestraPantalla();
	}

	NOE_ParaMusica();
	
    NOEDB_Finaliza();
	NOE_Finaliza();

    return 0;
	
}