//==================================================//
//    Multi-Platform Console (MPC) - v4.6.1 @ 2010  //
//     UFSM - Federal University of Santa Maria     //
//==================================================//

#ifndef __CAXIA_TEXTO_H__
#define __CAIXA_TEXTO_H__

#include <string>

static const int APP_LINES = 25;
static const int APP_COLUMNS = 56;
static const int COLUNA_INICIO_CAIXA = 3;
static const int LINHA_INICIO_CAIXA = 7;

//total de caracteres aceitos = APP_COLUMNS - coluna - 2
struct caixaDeTexto {
    //colocar o cursor aqui
    int cursorPosCol=COLUNA_INICIO_CAIXA;
    int cursorPosLin = LINHA_INICIO_CAIXA+1;
    char texto[APP_COLUMNS - COLUNA_INICIO_CAIXA - 2];
};

struct botao{
    int linha_inicio;
    int coluna_inicio;
    int linha_fim;
    int coluna_fim;
    char texto[20];
};

typedef struct caixaDeTexto caixaDeTexto_t;
typedef struct botao botao_t;

void initMpc(void);
void initApp(void);

void showBorder(void);
void preencheTela();

void caixaEditaTexto();
void displayApp(void);

void mostraTexto(int l, int c, char *msg);

void mostraTitulo(int l, int c, char *msg);

void moveTextoDireita(int pos, char caract);

void moveTextoEsquerda(int pos);

botao_t criaBotao(int linha_inicio, int coluna_inicio, int linha_fim, char *texto);

void exibeBotao(botao_t botao);

void verificar_clique(int lin, int col, botao_t botao);

void cbMouse(int l, int c, int button, int state);

void cbKeyboard(int key, int modifier, bool special, bool up);

void cbUpdate(void);

#endif // __APP_DEMO_H__
