//@Thiago Sordi

//Foram implementados todos requisitos básicos, clique de botão, edição na caixa de texto com diversos atalhos
//Foi inserida a funcionalidade de digitar um texto na caixa e inserir esse texto como linha no arquivo txt.
//E limpar a caixa de texto

#include "mpc_c.h"
#include "caixaTexto.h"

int main(void)
{
   initMpc();

   mpcRun(29); //frames por segundo
   return 0;
}
