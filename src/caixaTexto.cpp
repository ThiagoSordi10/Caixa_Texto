//==================================================//
//    Multi-Platform Console (MPC) - v4.6.2 @ 2010  //
//     UFSM - Federal University of Santa Maria     //
//==================================================//

//Funções para o funcionamento da caixa de texto
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpc_c.h"
#include "caixaTexto.h"


//variaveis globais para controle da caixa de texto.
bool changed = true;
caixaDeTexto_t caixaTxt;
botao_t botaoEnviar;
botao_t botaoLimpar;
char mensagem[50];


//faz as inicializacoes basicas do MPC
void initMpc(void) {
   //mpc configuration
   mpcSetSize(APP_LINES, APP_COLUMNS);

   //mpc callbacks
   mpcSetMouseFunc(cbMouse);
   mpcSetUpdateFunc(cbUpdate);
   mpcSetKeyboardFunc(cbKeyboard);

   mpcSetCursorColor(RED_3);
   mpcSetCursorPos(caixaTxt.cursorPosLin, caixaTxt.cursorPosCol);
   mpcSetCursorVisible(true);

   strcpy(caixaTxt.texto, "Ola mundo");

   mpcAbout();
}

//funcao que eh chamada a cada frame pela callback cbUpdate() para atualizar a tela
void displayApp(void){
    preencheTela();
    showBorder();
    mpcSetCursorPos(caixaTxt.cursorPosLin, caixaTxt.cursorPosCol);
}

//preenche a tela com cor, textos estaticos, caixa de texto, botões
void preencheTela(){
   for (int l = 1; l < APP_LINES-1; l++)
   {
      for (int c = 1; c < APP_COLUMNS-1; c++)
         mpcSetChar(l, c, 0, F_STD, GREY_1, GREY_1, 1.0);
   }//queria modularizar a parte de preencher as formas, mas a cor é algo que complica

   char texto[] = "Caixa de Texto";
   mostraTitulo(1, 1, texto);
   mostraTexto(LINHA_INICIO_CAIXA-2, 2, "Digite seu texto:");
   mostraTexto(LINHA_INICIO_CAIXA+5, 2, mensagem);
   caixaEditaTexto();

   botaoEnviar = criaBotao(18, 3, 18+2, "Enviar");
   botaoLimpar = criaBotao(18, 15, 18+2, "Limpar");
   exibeBotao(botaoEnviar);
   exibeBotao(botaoLimpar);
}

//cria cada botão passando apenas algumas propriedades
botao_t criaBotao(int linha_inicio, int coluna_inicio, int linha_fim, char *texto){
    //talvez tirar o cluna_fim porque ele define o fim
    botao_t botao;
    strcpy(botao.texto, texto);
    botao.linha_inicio = linha_inicio;
    botao.linha_fim = linha_fim;
    botao.coluna_inicio = coluna_inicio;
    botao.coluna_fim = coluna_inicio + 4 + strlen(texto);
    return botao;
}

//desenha uma janela com moldura
void showBorder(void) {
   for (int x = 0; x < APP_COLUMNS-1; x++) {
      mpcSetChar(0, x, 9, F_STD, GREEN_5, GREEN_5, 1.0);
      mpcSetChar(APP_LINES - 1, x, 9, F_STD, GREEN_5, GREEN_5, 1.0);
   }
   for (int y = 0; y < APP_LINES-1; y++) {
      mpcSetChar(y, 0, 10, F_STD, GREEN_5, GREEN_5, 1.0);
      mpcSetChar(y, APP_COLUMNS - 1, 10, F_STD, GREEN_5, GREEN_5, 1.0);
   }
   mpcSetChar(0, 01,                       11, F_STD, GREEN_5, GREEN_5, 1.0);
   mpcSetChar(0, APP_COLUMNS-1,           12, F_STD, GREEN_5, GREEN_5, 1.0);
   mpcSetChar(APP_LINES-1, 0,             13, F_STD, GREEN_5, GREEN_5, 1.0);
   mpcSetChar(APP_LINES-1, APP_COLUMNS-1, 14, F_STD, GREEN_5, GREEN_5, 1.0);

}

//funcao para exibir titulo da janela
void mostraTitulo(int l, int c, char *msg){
    int cont;
   for (cont = 0; cont < strlen(msg); cont++){
        mpcSetChar(l, c+cont, msg[cont], F_IN, BLACK, GREEN_3, 1);
        mpcSetChar(l+1, c+cont, 0, F_IN, GREEN_3, GREEN_3, 1);
   }
    while(c+cont < APP_COLUMNS-1){
        mpcSetChar(l, c+cont, 0, F_INS, GREEN_3, GREEN_3, 1 );
        mpcSetChar(l+1, c+cont, 0, F_IN, GREEN_3, GREEN_3, 1);
        cont++;
    }
}

//funcao auxiliar para exibir textos na tela.
void mostraTexto(int l, int c, char *msg){
   for (int cont = 0; cont < strlen(msg); cont++)
      mpcSetChar(l, c+cont, msg[cont], F_IN, BLACK, GREY_1, 1 );
}

//desenhar a caixa de texto com o texto já dentro
void caixaEditaTexto(){
    int i = 0;
    for(int linha = LINHA_INICIO_CAIXA; linha < LINHA_INICIO_CAIXA+3; linha++){
        for (int cont = COLUNA_INICIO_CAIXA; cont < APP_COLUMNS-2; cont++){
            if(caixaTxt.texto[i] != '\0' && linha == LINHA_INICIO_CAIXA+1){
                mpcSetChar(linha, cont, caixaTxt.texto[i], F_IN, BLACK, WHITE, 1);
                i++;
            }else{
                mpcSetChar(linha, cont, 0, F_IN, WHITE, WHITE, 1);
            }

       }
    }
}

//clique do botão enviar salva texto no arquivo
void enviar(){
    FILE *file = fopen("src/texto.txt", "a+");
    if (file == NULL)
    {
        printf("ERRO! O arquivo não foi aberto!\n");
        return;
    }
    fprintf(file, "%s\n", caixaTxt.texto);
    fclose(file);
    strcpy(mensagem, "Copiado no arquivo texto.txt"); //exibe esse texto na tela
}

//Limpar caixa de texto
void limparCaixaTexto(){
    strcpy(caixaTxt.texto, "");
    caixaTxt.cursorPosCol = COLUNA_INICIO_CAIXA;
    strcpy(mensagem, "Caixa de texto limpa"); //exibe esse texto na tela
}

//verifica se o clique foi dentro de algum dos botões
void verificar_clique(int lin, int col, botao_t botao){
    if(lin >= botao.linha_inicio && lin <= botao.linha_fim && col >= botao.coluna_inicio && col <= botao.coluna_fim){
            printf("botão %s", botao.texto);
        if(!strcmp(botao.texto, "Limpar")){
            limparCaixaTexto();
        }else if(!strcmp(botao.texto,"Enviar")){
            enviar();
        }
    }
}

//desenha o botão na tela
void exibeBotao(botao_t botao){
    for (int l = botao.linha_inicio; l <= botao.linha_fim; l++)
   {
      for (int c = botao.coluna_inicio; c <= botao.coluna_fim; c++)
         mpcSetChar(l, c, 0, F_STD, GREEN_4, GREEN_4, 1.0);
   }
    int j=0;
   for(int i=botao.coluna_inicio+2; i<botao.coluna_fim-2; i++){
        mpcSetChar((botao.linha_inicio+botao.linha_fim)/2, i, botao.texto[j], F_STD, WHITE, GREEN_4, 1.0);
        j++;
   }
}

void moveTextoDireita(int pos, char caract){
    char aux;
    if(pos > strlen(caixaTxt.texto)){ //verfica se o cursor passa da palavra
        for(int cont = strlen(caixaTxt.texto); cont < pos; cont++){ //preenche com espaços até o caracter
            caixaTxt.texto[cont] = ' ';
        }
        caixaTxt.texto[pos] = caract;
    }
    else{
        for(int cont = pos; cont <= strlen(caixaTxt.texto); cont++){//move texto se der espaço no meio
            aux = caixaTxt.texto[cont];
            caixaTxt.texto[cont] = caract;
            caract = aux;
        }
    }
}

void moveTextoEsquerda(int pos){
    for(int cont = pos; cont < strlen(caixaTxt.texto); cont++){ //quando é usado backspace ou delete
        caixaTxt.texto[cont] = caixaTxt.texto[cont+1];

    }
}

//*************************************************************
//callbacks implementation
//*************************************************************
void cbMouse(int lin, int col, int button, int state) {
   //mpcSetCursorPos(l, c);
   //printf("\ncalback de mouse %d %d",lin, col);
   //mouseY = lin;
   if(button == 0 && state == 0){ //todo clique é verificado
    verificar_clique(lin, col, botaoEnviar);
    verificar_clique(lin, col, botaoLimpar);
   }
   changed = true;
}

void cbKeyboard(int key, int modifier, bool special, bool up) {
   if (special) {
      if (!up) { // só no evento clique de botão
         switch (key) {
            case 100: // mover para cursor esquerda
                if(caixaTxt.cursorPosCol > COLUNA_INICIO_CAIXA){ //verifica se está dentro da caixa
                   caixaTxt.cursorPosCol -= 1;
                }
                break;
             case 102: // mover cursor para direita
                 if(caixaTxt.cursorPosCol < APP_COLUMNS-3){ //verifica se está dentro da caixa
                    caixaTxt.cursorPosCol += 1;
                 }
                break;
             case 106: // mover cursor para o inicio
                caixaTxt.cursorPosCol = COLUNA_INICIO_CAIXA;
                break;
            case 107: // mover cursor para o final
                caixaTxt.cursorPosCol = strlen(caixaTxt.texto)+COLUNA_INICIO_CAIXA;
                break;
        }
      }
   } else {
      if (!up) {
           switch(key){
            case 8: // backspace
                if(caixaTxt.cursorPosCol > COLUNA_INICIO_CAIXA){ //verifica se está dentro da caixa
                     moveTextoEsquerda(caixaTxt.cursorPosCol - COLUNA_INICIO_CAIXA-1);
                    caixaTxt.cursorPosCol -= 1;
                }
                break;
            case 127: // delete
                moveTextoEsquerda(caixaTxt.cursorPosCol - COLUNA_INICIO_CAIXA);
                break;
            default: // digitação normal de caracteres
                if(caixaTxt.cursorPosCol < APP_COLUMNS-3){ //verifica se está dentro da caixa
                    moveTextoDireita(caixaTxt.cursorPosCol - COLUNA_INICIO_CAIXA, (char) key);
                    caixaTxt.cursorPosCol += 1;
                    if(caixaTxt.cursorPosCol >= APP_COLUMNS-3){ //na ultima digitação o cursor não sai da caixa
                        caixaTxt.cursorPosCol -= 1;
                    }
                }
                break;
           }
      }
   }
   strcpy(mensagem, "");
   changed = true;
}

void cbUpdate(void) {
   if (changed) {
      displayApp();
      changed = false;
   }
}
