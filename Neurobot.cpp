////////////////////////////////////////////////////////////////// 
//                                                              //
//             * * * NEURAL LABYRINTH ROBOT * * *               //
//                                                              //
//         Copyright (c) 2002 Marvin Oliver Schneider           //
//                                                              //
//    Projeto a ser apresentado no Xo. Encontro Nacional de     //
//   Inteligencia Artificial (ENIA) durante o XXo. Congresso    //
//         da Sociedade Brasileira de Computacao (SBC)          //
//                                                              //
//////////////////////////////////////////////////////////////////
//                             //                               //
// ---------- VERSAO --------- // ---- DATA DE ATUALIZACAO ---- //
//                             //                               //
//             1.0b            //           01/01/2003          //
//                             //                               //
//////////////////////////////////////////////////////////////////
//                                                              //
// -------------------- TAREFAS EM ABERTO --------------------- //
//                                                              //
// 1. Modificacao de labirintos mais comfortavel - OK           //
// 2. Testes com labirintos diferentes           - OK           //
// 3. Implementacao da entrada adicional         - OK           //
// 4. Testes com labirintos parecidos            - OK           //
// 5. Completar os comentarios no codigo fonte                  //
// ------------------------------------------------------------ //   
// 6. Criacao de leitura e gravacao de labirintos               //
// 7. Criacao de logs de aprendizado                            //
// 8. Desenvolvimento de diferentes arquivos de labirintos      //
// 9. Testes finais e avaliacao dos resultados                  //
//                                                              //
//////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// Arquivos incluidos                                //
///////////////////////////////////////////////////////

#include <stdio.h>   // standard
#include <stdlib.h>  // standard
#include <conio.h>   // Para funcao "GETCH()"
#include <math.h>    // Para funcao "ABS()"
#include <console.h> // Biblioteca para uso com a console

                     // Comandos disponiveis:

                     // clrscr( escrita | fundo );
                     // setcolor( escrita | fundo );
                     // gotoxy(x,y);

                     // Cores disponiveis:

                     // BLACK, BLUE, GREEN, CYAN, RED, PURPLE, BROWN, LIGHTGRAY, DARKGRAY,
                     // LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTPURPLE, YELLOW, WHITE

                     // Uso de cores:

                     // XFOREGROUND_[cor] (ex: XFOREGROUND_WHITE) - cor da escrita
                     // XBACKGROUND_[cor] (ex: XBACKGROUND_BLACK) - cor do fundo


///////////////////////////////////////////////////////
// Constantes globais                                //
///////////////////////////////////////////////////////

const char versao[5] = "1.0b";

const int x_max = 8; // numero maximo de elementos
const int y_max = 8;

//const int x_start = 33; // ponto de comeco do tabuleiro para saida na tela
                          // x_start foi redefinido como variavel global no programa
                          // principal (veja abaixo)
const int y_start = 11;

const int caminhos_max = 20; // numero maximo de caminhos a ser achado;

const int max_itens_menu = 15; // numero maximo de itens em um menu;

const int max_tabuleiros = 21; // numero maximo de tabuleiros tratados

const int obstaculo = 1; // codificacao interna do tabuleiro
const int vazio     = 0;

const int robo      = 2; // apenas para visualizacao de visao com cor certa
const int destino   = 3;

const char obstaculo_ch = char(177); // characteres para visualizacao na tela
const char destino_ch[3]     = "[]";
const char robo_ch[3]        = "oo";
const char vazio_ch[3]       = "  ";

const int sem_giro      = 0; // para administracao dos giros possiveis
const int para_cima     = 1;
const int para_baixo    = 2;
const int para_esquerda = 3;
const int para_direita  = 4;

const int dummy  = -1;  // valores dummy para inicializacao de arrays

const int nro_codigos           = 2;

const int elemento_visibilidade = 2;  // para processamento da rede
const int elemento_obstaculo    = 0;
const int elemento_adicional    = 1;

const int True  = 1;  // na verdade nao necessario, mas para versoes antigas de Visual C
const int False = 0;

const int simples  = 0; // para as caixas de mensagem
const int alerta   = 1;
const int pergunta = 2;
const int aviso    = 3;
const int chave    = 4;

const int menu1 = 0; // para menu "modificar labirintos"
const int menu2 = 1;

// caracteres para poder fazer um tabuleiro "limpo" (codigo Ascii standard)

const char ces = char(201); //canto direito superior;
const char cdi = char(188); //canto esquerdo inferior;
const char ho  = char(205); //horizontal
const char cr  = char(206); //cruz 
const char ve  = char(186); //vertical 
const char vhe = char(185); //vertical com linha horizontal na esquerda
const char vhd = char(204); //vertical com linha horizontal na direita
const char cei = char(200); //canto direito inferior
const char cds = char(187); //canto esquerdo superior
const char hva = char(203); //horizontal com linha vertical em cima
const char hve = char(202); //horizontal com linha vertical em baixo 

const char cds1 = char(218); //canto direito superior;
const char cei1 = char(217); //canto esquerdo inferior;
const char ho1  = char(196); //horizontal
const char cr1  = char(197); //cruz
const char ve1  = char(179); //vertical
const char vhe1 = char(180); //vertical com linha horizontal na esquerda
const char vhd1 = char(195); //vertical com linha horizontal na direita
const char cdi1 = char(192); //canto direito inferior
const char ces1 = char(191); //canto esquerdo superior
const char hva1 = char(194); //horizontal com linha vertical em cima
const char hve1 = char(193); //horizontal com linha vertical em baixo 


const char h1 = char(196);      // linha para o titulo

const char s1 = char(176); // sombra  1  - a mais fraca
const char s2 = char(177); // sombra  2  - um pouco mais forte
const char s3 = char(178); // sombra  3  - a sombra mais forte
const char s4 = char(219); // "sombra 4" - um campo solido

const char right_ch = char(16); // para visualizacao de giros
const char left_ch  = char(17);
const char up_ch    = char(30);
const char down_ch  = char(31);

const int nr_entradas   = x_max*y_max*nro_codigos; // forma generica de definicao das camadas
const int nr_escondidos = x_max*y_max;  // por causa da topologia da rede livre ajuste possivel!!
const int nr_saidas     = x_max*y_max;

const BOOL debug = True; // liga/desliga do modo debug

///////////////////////////////////////////////////////
// Tipos globais                                     //
///////////////////////////////////////////////////////

typedef int BOOL;       // variaveis booleanas

typedef struct _xy {	// tipo para coordenadas no tabuleiro
	int x, y;
} XY;

typedef struct _caminho {	// tipo para guardar o caminho do robo do inicio ate o final
	XY pos[x_max*y_max];
} CAMINHO;

typedef struct _caminhos {		// para processamento "cache" dos caminhos
	CAMINHO cam[max_tabuleiros];
} CAMINHOS;

typedef struct _tabuleiro {    // tabuleiro (mundo virtual 2D) com todos os dados necessarios p/ processamento
	int numero, giros_pos;
	int elemento[x_max][y_max];  // tabuleiro "real" de elementos
	BOOL visao[x_max][y_max];  // tabuleiro de elementos visiveis
	XY giro[x_max*y_max]; // valor maximo nominal de giros possiveis
	CAMINHO cam_mem;
	XY destino;  // destino e robo estao fora do tabuleiro para facilitar os calculos
	XY robo;
} TABULEIRO;

typedef struct _mapa { // saida da rede em forma de matriz
	float preferencia[x_max][y_max];
} MAPA;

typedef struct _saida { // para definicao da saida certa
	float valor[nr_saidas];
} SAIDA;

typedef struct _rede_neural { // rede neural => Perceptron Multicamadas (1 camada escondida)
	float entrada[nr_entradas];
	float sinapse1[nr_entradas][nr_escondidos];
	float escondido[nr_escondidos];
	float sinapse2[nr_escondidos][nr_saidas];
	float saida[nr_saidas];
} REDE_NEURAL;

typedef struct _menu {  // estrutura para a visualizacao de menus
	char* titulo;
	char* item[max_itens_menu];
} MENU;


///////////////////////////////////////////////////////
// Variaveis globais                                 //
///////////////////////////////////////////////////////

TABULEIRO data[max_tabuleiros]; // tabuleiros em memoria

TABULEIRO central; // tabuleiro central

CAMINHO resultado; // resultado do calculo

CAMINHOS cache; // cache para o aprendizado de caminhos

REDE_NEURAL rede; // estrutura de rede neural

SAIDA saida_central; // saidas da rede separadas (apenas para simplificar)

MAPA mapa_central; // o mapa resultante

MENU principal, labirinto, labirinto2, vision; // os menus disponiveis

BOOL todos_ok; // monitoramento de aprendizado

int total_acertos, total_giros, x_start; // monitoramento

float taxa_erro; // monitoramento

///////////////////////////////////////////////////////
// Prototipos de funcoes                             //
///////////////////////////////////////////////////////

void init_menus(void); // inicializacao das variaveis de menu

void janela(int,int,int,int); // IO: visualizacao de uma janela na tela

BOOL caixa_mensagem(int,int,int,char*); // IO: visualizacao de uma caixa de mensagem

void tecla(void); // IO: dialogo "continuar com qualquer tecla"

int escolha(int,int,MENU); // IO: visualizacao de um menu

void cor_padrao(void); // IO: reajuste para a cor padrao

void cor_visao(int); // IO: reajuste para a cor do padrao de visao

void tab(void); // IO: tabulacao

void titulo(void); // IO: visualizacao de titulo

void informacoes_gerais(void); // Funcao: Visualizacao de Informacoes Gerais

void init_todos_tabuleiros(void); // Inicializacao dos tabuleiros dentro do array "data"

TABULEIRO init_tabuleiro(int); // Inicializacao de um tabuleiro especifico

CAMINHO init_caminho(void); // Inicializacao de um caminho especifico

void mostra_tabuleiro(TABULEIRO,BOOL,BOOL); // Visualizacao de uma tabuleiro na tela

TABULEIRO estabelece_visao(TABULEIRO); // Estabelecimento da visao a partir de um certo tabuleiro

TABULEIRO estabelece_giros(TABULEIRO); // Estabelecimento dos giros a partir de um certo tabuleiro com uma situacao de visao

int conta_elementos(CAMINHO); // Contagem de elementos de um caminho (para processamento simbolico) 

int calcula_distancia(CAMINHO); // Calculo de distancia total dentro de caminho (para processamento simbolico)

CAMINHO algoritmo_simbolico(XY,TABULEIRO,CAMINHO); // Rotina principal do algoritmo simbolico

MAPA init_mapa(); // Inicializacao de um mapa de saidas

void execute_caminho(TABULEIRO,CAMINHO); // Execucao de um caminho em cima de um tabuleiro

void aprende_caminho(TABULEIRO,CAMINHO); // Aprendizado de um caminho

void use_algoritmo_simbolico(void); // Funcao: Uso do algoritmo simbolico

void use_rede_neural(void); // Funcao: Uso da rede neural

void aprende_caminhos(void); // Funcao: Aprendizado de caminhos

float sigmoide(float); // Funcao de propagacao da rede

REDE_NEURAL init_rede(void); // Inicializacao da rede neural

void dump_rede(REDE_NEURAL,BOOL); // Visualizacao simples dos valores da rede na tela

REDE_NEURAL feedforward(TABULEIRO,REDE_NEURAL); // Processamento de dados pela rede

REDE_NEURAL backpropagation(REDE_NEURAL,SAIDA); // Funcao de aprendizado da rede

SAIDA extrai_saidas(REDE_NEURAL); // Extracao das saidas da rede

MAPA cria_mapa_das_saidas(SAIDA); // Criacao do mapa atraves das saidas

SAIDA cria_saidas_do_mapa(MAPA); // Criacao das saidas atraves do mapa

void modificar_tabuleiro(void); // Funcao: Modificacao de tabuleiros em memoria

void mostra_tabuleiros(void); // Funcao: Visualizacao dos tabuleiros em memoria

void mostra_mapa(MAPA); // Visualizacao de mapa (debug)

void mostra_saida(SAIDA); // Visualizacao de saidas (debug)

SAIDA gera_saida_para_teste(int,int); // Geracao de saidas apenas para teste (debug)

void testa_teclado(void); // Teste de teclado fornecendo o scan-code (debug)

int gera_codigo_visao(TABULEIRO); // Formula para geracao do codigo de visao

int gera_codigo_obstaculos(TABULEIRO); // Formula para geracao do codigo de obstaculos

///////////////////////////////////////////////////////
// Programa principal                                //
///////////////////////////////////////////////////////

void main()
{
	int opcao;

	BOOL final_do_programa;

//	testa_teclado();

	x_start=39-x_max; // definicao automatica da posicao no eixo x do tabuleiro

	init_menus();

	init_todos_tabuleiros();

	final_do_programa=False;

	rede=init_rede();

	while(final_do_programa==False)
	{
		clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

		cor_padrao();

		titulo();

		opcao=escolha(0,8,principal);

		switch(opcao)
		{
		case 0: informacoes_gerais(); break;
		case 1: mostra_tabuleiros(); break;
		case 2: modificar_tabuleiro(); break;
		case 3: dump_rede(rede,False); break;
		case 4: use_algoritmo_simbolico(); break;
		case 5: if(caixa_mensagem(pergunta,0,20," Zerar toda a Rede? ")==True)
				{
					rede=init_rede();
					caixa_mensagem(aviso,0,20," Rede inicializada com sucesso. ");
				}
			break;
		case 6: aprende_caminhos(); break;
		case 7: use_rede_neural(); break;
		case 8: if(caixa_mensagem(pergunta,0,20," Sair do Programa? ")==True)
					final_do_programa=True;
			break;
		}
	}

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLACK );

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLACK );
	
}

///////////////////////////////////////////////////////
// Rotinas                                           //
///////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Funcao:      init_menus                                         //
// Descricao:   Rotina de IO para inicializacao dos dialogos menu  //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: Se for desejado espacos tambem precisam ser        //
//              inicializados                                      //
/////////////////////////////////////////////////////////////////////

void init_menus(void)
{
	principal.titulo=" MENU PRINCIPAL ";
	principal.item[0]=" Sobre o Sistema          ";
	principal.item[1]=" Mostrar Labirintos       ";
	principal.item[2]=" Modificar Labirintos     ";
	principal.item[3]=" Dump da Rede Neural      ";
	principal.item[4]=" Usar Algoritmo Simbolico ";
	principal.item[5]=" Inicializar Rede Neural  ";
	principal.item[6]=" Aprender Caminhos        ";
	principal.item[7]=" Usar Rede Neural         ";
	principal.item[8]=" Sair do Programa         ";
	principal.item[9]=NULL;

	labirinto.titulo=" ESCOLHA LABIRINTO (01-10) ";
	labirinto.item[0]=" Modificar Labirinto 01       ";
	labirinto.item[1]=" Modificar Labirinto 02       ";
	labirinto.item[2]=" Modificar Labirinto 03       ";
	labirinto.item[3]=" Modificar Labirinto 04       ";
	labirinto.item[4]=" Modificar Labirinto 05       ";
	labirinto.item[5]=" Modificar Labirinto 06       ";
	labirinto.item[6]=" Modificar Labirinto 07       ";
	labirinto.item[7]=" Modificar Labirinto 08       ";
	labirinto.item[8]=" Modificar Labirinto 09       ";
	labirinto.item[9]=" Modificar Labirinto 10       ";
	labirinto.item[10]=" Modificar Labirintos 11 a 21 ";
	labirinto.item[11]=NULL;

	labirinto2.titulo=" ESCOLHA LABIRINTO (11-21) ";
	labirinto2.item[0]=" Modificar Labirinto 11       ";
	labirinto2.item[1]=" Modificar Labirinto 12       ";
	labirinto2.item[2]=" Modificar Labirinto 13       ";
	labirinto2.item[3]=" Modificar Labirinto 14       ";
	labirinto2.item[4]=" Modificar Labirinto 15       ";
	labirinto2.item[5]=" Modificar Labirinto 16       ";
	labirinto2.item[6]=" Modificar Labirinto 17       ";
	labirinto2.item[7]=" Modificar Labirinto 18       ";
	labirinto2.item[8]=" Modificar Labirinto 19       ";
	labirinto2.item[9]=" Modificar Labirinto 20       ";
	labirinto2.item[10]=" Modificar Labirinto 21       ";
	labirinto2.item[11]=" Modificar Labirintos 01 a 10 ";
	labirinto2.item[12]=NULL;

	vision.titulo=" ESCOLHA VISAO ";
	vision.item[0]=" Sem Elementos do Processamento ";
	vision.item[1]=" Mostrar Lugares com Visao      ";
	vision.item[2]=" Mostrar Pontos de Giro         ";
	vision.item[3]=" Mostrar Giros e Visao          ";
	vision.item[4]=NULL;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      janela                                             //
// Descricao:   Rotina de IO para criacao de contorno de janelas   //
// Parametros:  Canto esquerdo superior (x,y) e comprimento das    //
//              linhas (x,y)                                       //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void janela(int canto_x, int canto_y, int ext_x, int ext_y)
{
	int i, j;
	gotoxy(canto_x,canto_y);

	printf("%c",ces);
	for(i=0;i<ext_x;i++) printf("%c",ho);
	printf("%c",cds);

	for(i=0;i<ext_y;i++)
	{
		gotoxy(canto_x,canto_y+i+1);
		printf("%c",ve);
		for(j=0;j<ext_x;j++)
			printf(" ");
		printf("%c",ve);
	}

	gotoxy(canto_x,canto_y+ext_y+1);
	printf("%c",cei);
	for(i=0;i<ext_x;i++) printf("%c",ho);
	printf("%c",cdi);

}

/////////////////////////////////////////////////////////////////////
// Funcao:      caixa_mensagem                                     //
// Descricao:   Rotina para mostrar uma caixa de mensagem na tela  //
// Parametros:  Tipo da caixa, comeco x, comeco y, texto           //
// Retorno:     Confirmacao (se for do tipo pergunta)              //
// Versao:      0.1 - Versao inicial                               //
// Observacoes:                                                    //
//              Tipos disponiveis                                  //
//              -----------------                                  //
//              Aviso                                              //
//              Pergunta                                           //
//              Alerta                                             //
//              Simples                                            //
//              Tecla                                              //
//                                                                 //
/////////////////////////////////////////////////////////////////////

BOOL caixa_mensagem(int tipo, int x, int y, char* dialogo)
{
	int comprimento_topo, comprimento_dialogo, comprimento_x, comprimento_y;

	char ch;

	BOOL confirmacao;

	char* topo;

	topo=NULL;

	confirmacao=False;

	switch(tipo)
	{
	case alerta:   topo=" ALERTA "; 
		setcolor( XFOREGROUND_WHITE | XBACKGROUND_RED );
		break;
	case pergunta: topo=" PERGUNTA "; 
		setcolor( XFOREGROUND_WHITE | XBACKGROUND_CYAN );
		break;
	case aviso:    topo=" AVISO "; 
		setcolor( XFOREGROUND_WHITE | XBACKGROUND_PURPLE );
		break;
	case simples: topo=NULL;
		setcolor( XFOREGROUND_WHITE | XBACKGROUND_LIGHTBLUE );
		break;
	case chave: topo=NULL;
		setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN );
		break;
	}

	comprimento_dialogo=strlen(dialogo);

	if(topo!=NULL)
		comprimento_topo=strlen(topo); else comprimento_topo=0;

	comprimento_y=2;

	if(tipo==simples || tipo==chave) comprimento_y=1;

	if(comprimento_topo>comprimento_dialogo) 
		comprimento_x=comprimento_topo; else comprimento_x=comprimento_dialogo;

	if(x==0) x=(80-comprimento_x-2)/2; // se x=0 entao assume que o dialogo deve ser posto no meio da tela

	janela(x,y,comprimento_x,comprimento_y);

	gotoxy(x+1+(comprimento_x-comprimento_dialogo)/2,y+1);

	printf("%s",dialogo);

	gotoxy(x+1+(comprimento_x-comprimento_topo)/2,y);

	if(topo!=NULL)
		printf("%s",topo);

	setcolor( XFOREGROUND_BLUE | XBACKGROUND_WHITE );

	if(tipo==alerta || tipo==aviso)
	{
		gotoxy(x+1+(comprimento_x-4)/2,y+2);
		printf(" OK ");
		gotoxy(x+2+(comprimento_x-4)/2,y+2);
		getch();
	}

	if(tipo==pergunta)
	{
		gotoxy(x+1+(comprimento_x-5)/2,y+2);
		printf(" S/N ");
		gotoxy(x+3+(comprimento_x-5)/2,y+2);
		ch=' ';
		while(ch!='N' && ch!='S' && ch!='J' && ch!='Y')
		{
			ch=getch();
			ch=toupper(ch);
		}
		if(ch=='S') confirmacao=True;
	}

	if(tipo==simples || tipo==chave)
	{
		gotoxy(x+comprimento_x,y+1);
	}

	cor_padrao();

	return confirmacao;
}

void tecla(void)
{
	caixa_mensagem(chave,0,22," Pressione qualquer tecla para continuar.... ");

	getch();
}

/////////////////////////////////////////////////////////////////////
// Funcao:      escolha                                            //
// Descricao:   Funcao de menu                                     //
// Parametros:  Comeco x e comeco y como o menu inicializado       //
// Retorno:     Item escolhido pelo usuario                        //
// Versao:      0.1 - Versao inicial                               //
// Observacoes:                                                    //
//              Uso                                                //
//              ---                                                //
//              [Cursor Up] ou [8] para cima no menu               //
//              [Cursor Down] ou [2] para baixo no menu            //
//              [Enter] ou [5] para confirmar                      //
//                                                                 //
/////////////////////////////////////////////////////////////////////

int escolha(int x, int y, MENU menu_local)
{
	int escolha_local, m, itens, comprimento, comprimento_titulo, aux, ajuste_titulo;

	char ch;

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN );

	comprimento=strlen(menu_local.titulo);

	itens=0;

	while(menu_local.item[itens]!=NULL) itens++;

	for(m=0;m<itens;m++)
	{
		aux=strlen(menu_local.item[m]);
		if(aux>comprimento)
			comprimento=aux;
	}

	if(x==0) x=(80-comprimento-2)/2; // se x=0 entao assume que o dialogo deve ser posto no meio da tela	

	janela(x,y,comprimento,itens);

	escolha_local=0;

	comprimento_titulo=strlen(menu_local.titulo);

	if(comprimento_titulo<comprimento)
		ajuste_titulo=(comprimento-comprimento_titulo)/2; else
		ajuste_titulo=0;

	gotoxy(x+1+ajuste_titulo,y);
	printf("%s",menu_local.titulo);

	ch=' ';

	while(ch!=char(13) && ch!='5')
	{
		for(m=0;m<itens;m++)
		{
			gotoxy(x+1,y+1+m);
			if(m==escolha_local)
				setcolor( XFOREGROUND_BLUE | XBACKGROUND_WHITE );
			printf("%s",menu_local.item[m]);
			setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN);
		}

		gotoxy(x+1,y+1+escolha_local);

		ch=getch();

		for(m=0;m<itens;m++)
		{
			gotoxy(x+1,y+1+m);
			printf("%s",menu_local.item[m]);
		}


		switch(ch)
		{
		case char(72): escolha_local--;
			if(escolha_local<0) escolha_local=itens-1;
			break;
		case char(80): escolha_local++;
			if(escolha_local>=itens) escolha_local=0;
			break;
		case '8': escolha_local--;
			if(escolha_local<0) escolha_local=itens-1;
			break;
		case '2': escolha_local++;
			if(escolha_local>=itens) escolha_local=0;
			break;
		}

	}

	return escolha_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      cor_padrao                                         //
// Descricao:   Rotina que estabelece o equema de cor padrao       //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void cor_padrao()
{
	setcolor(XFOREGROUND_WHITE | XBACKGROUND_BLUE);
}

/////////////////////////////////////////////////////////////////////
// Funcao:      cor_visao                                          //
// Descricao:   Estabelece a cor na visualizacao da visao          //
// Parametros:  Codigo do elemento a ser visualizado               //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void cor_visao(int codigo)
{
	switch(codigo)
	{
	case robo : setcolor(XFOREGROUND_LIGHTBLUE | XBACKGROUND_GREEN); break;
	case destino : setcolor(XFOREGROUND_LIGHTGREEN | XBACKGROUND_GREEN); break;
	case obstaculo : setcolor(XFOREGROUND_LIGHTRED | XBACKGROUND_GREEN); break;
	case vazio : setcolor(XFOREGROUND_WHITE | XBACKGROUND_GREEN); break;
	}
}

/////////////////////////////////////////////////////////////////////
// Funcao:      tab                                                //
// Descricao:   Rotina para simplificar os espacos antes de uma    //
//              linha                                              //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void tab(void)
{
	printf("   ");
}

/////////////////////////////////////////////////////////////////////
// Funcao:      titulo                                             //
// Descricao:   Mostra o titulo do sistema na tela                 //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void titulo()
{
	setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY );

	janela(5,1,69,3);

	setcolor( XFOREGROUND_YELLOW | XBACKGROUND_DARKGRAY );

	gotoxy(19,2);

	printf("N E U R A L   L A B Y R I N T H   R O B O T");

    setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY );

	gotoxy(10,4);

	printf("Copyright (c) 2002-2003 by Marvin Oliver Schneider - Freeware");

	setcolor( XFOREGROUND_LIGHTGREEN | XBACKGROUND_DARKGRAY );

	gotoxy(63,2);

	printf(" %s ",versao);

	gotoxy(69,2);

	printf("t%i",max_tabuleiros);

	cor_padrao();
}

/////////////////////////////////////////////////////////////////////
// Funcao:      informacoes_gerais                                 //
// Descricao:   Rotina para mostrar informacoes gerais referentes  //
//              ao sistema                                         //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void informacoes_gerais(void)
{
	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();

	caixa_mensagem(simples,0,6," SOBRE O SISTEMA ");

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_CYAN );

	janela(2,10,74,10);

	gotoxy(5,11);

	printf("O presente sistema foi criado para demonstrar o uso de uma rede neural");

	gotoxy(5,12);

	printf("em um robo que acha o caminho para a saida dentro de um labirinto");

	gotoxy(5,13);

	printf("virtual.");

	gotoxy(5,15);

	printf("Ele foi desenvolvido para apresentacao no ENIA'03 (parte do congresso");

	gotoxy(5,16);

	printf("da SBC em Campinas em 2003).");

	gotoxy(5,18);

	printf("Sao analisados %i labirintos por um algoritmo simbolico, que fornece",max_tabuleiros);

	gotoxy(5,19);

	printf("o resultado de seus calculos a rede neural. A rede neural e realizada");

	gotoxy(5,20);

	printf("atraves de um Perceptron de duas camadas e aprendizado Backpropagation.");

	tecla();
}

/////////////////////////////////////////////////////////////////////
// Funcao:      init_todos_tabuleiros                              //
// Descricao:   Inicializacao dos 11 tabuleiros                    //
// Parametros:  Identificacao do tabuleiro a ser retornado         //
// Retorno:     Tabuleiro inicializado                             //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void init_todos_tabuleiros(void)
{
	int i, j, k;

	// Zerar os tabuleiros a serem trabalhados inicialmente

	for(k=0;k<max_tabuleiros;k++)
	{

		for(i=0;i<x_max;i++)
			for(j=0;j<y_max;j++)
			{
				data[k].elemento[i][j]=vazio;
				data[k].visao[i][j]=False;
			}

		// Inicializacao do array de giros
	
		for(i=0;i<x_max*y_max;i++)
		{
			data[k].giro[i].x=dummy;
			data[k].giro[i].x=dummy;

			data[k].cam_mem.pos[i].x=dummy;
			data[k].cam_mem.pos[i].y=dummy;
		}

		data[k].giros_pos=0;
	}

		
/*

     Tabuleiro Nro.: 1

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . . . . . . . |
  1 | . o . . . . . . |
  2 | . . . . . . . . |
  3 | . . . . . . . . |
  4 | . . . . . . . . |
  5 | . . . . . . . . |
  6 | . . . . . . P . |
  7 | . . . . . . . . |
    +-----------------+

*/
			data[0].numero=1;

			data[0].robo.x=1;
			data[0].robo.y=1;
					
			data[0].destino.x=6;
			data[0].destino.y=6;
		
/*

     Tabuleiro Nro.: 1

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | P . . . . . . . |
  1 | . # . # . # . # |
  2 | . . . . . . . . |
  3 | . # . # . # . # |
  4 | . . . . . . . . |
  5 | # . # . # . # . |
  6 | . . . . . . . . |
  7 | # . # . # . # o |
    +-----------------+

*/
			data[1].numero=2;

			data[1].robo.x=7;
			data[1].robo.y=7;
			
			data[1].destino.x=0;
			data[1].destino.y=0;

			data[1].elemento[1][1]=obstaculo;
			data[1].elemento[3][1]=obstaculo;
			data[1].elemento[5][1]=obstaculo;
			data[1].elemento[7][1]=obstaculo;
			
			data[1].elemento[1][3]=obstaculo;
			data[1].elemento[3][3]=obstaculo;
			data[1].elemento[5][3]=obstaculo;
			data[1].elemento[7][3]=obstaculo;

			data[1].elemento[0][5]=obstaculo;
			data[1].elemento[2][5]=obstaculo;
			data[1].elemento[4][5]=obstaculo;
			data[1].elemento[6][5]=obstaculo;

			data[1].elemento[0][7]=obstaculo;
			data[1].elemento[2][7]=obstaculo;
			data[1].elemento[4][7]=obstaculo;
			data[1].elemento[6][7]=obstaculo;
			
/*

     Tabuleiro Nro.: 3

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . . # P . . . |
  1 | . . . # . . . . |
  2 | . . . # . . . . |
  3 | . . . # . . . . |
  4 | . . . . . . . . |
  5 | . . . . # . . . |
  6 | . o . . # . . . |
  7 | . . . . # . . . |
    +-----------------+

*/

			data[2].numero=3;

			data[2].robo.x=1;
			data[2].robo.y=6;
						
			data[2].destino.x=4;
			data[2].destino.y=0;
			
			data[2].elemento[3][0]=obstaculo;
			data[2].elemento[3][1]=obstaculo;
			data[2].elemento[3][2]=obstaculo;
			data[2].elemento[3][3]=obstaculo;
			data[2].elemento[4][5]=obstaculo;
			data[2].elemento[4][6]=obstaculo;
			data[2].elemento[4][7]=obstaculo;
/*

     Tabuleiro Nro.: 4

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . # . . . . . |
  1 | . . # . # . . . |
  2 | . . # . . . . . |
  3 | . . # P # . . . |
  4 | . . # # # # . . |
  5 | . . . # . . . . |
  6 | . # . # . . # # |
  7 | o # . . . . . . |
    +-----------------+

*/

			data[3].numero=4;

			data[3].robo.x=0;
			data[3].robo.y=7;
			
			data[3].destino.x=3;
			data[3].destino.y=3;
			
			data[3].elemento[2][0]=obstaculo;
			data[3].elemento[2][1]=obstaculo;
			data[3].elemento[2][2]=obstaculo;
			data[3].elemento[2][3]=obstaculo;
			data[3].elemento[2][4]=obstaculo;

			data[3].elemento[3][4]=obstaculo;

			data[3].elemento[4][1]=obstaculo;
	
			data[3].elemento[4][3]=obstaculo;
			data[3].elemento[4][4]=obstaculo;

			data[3].elemento[3][5]=obstaculo;
			data[3].elemento[3][6]=obstaculo;
			data[3].elemento[1][6]=obstaculo;
			data[3].elemento[1][7]=obstaculo;
			data[3].elemento[6][6]=obstaculo;
			data[3].elemento[7][6]=obstaculo;
			data[3].elemento[5][4]=obstaculo;			
		
/*

     Tabuleiro Nro.: 5

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | o # . # . # . . |
  1 | . # . . . . . . |
  2 | . # . # # . # # |
  3 | . # . # . . . . |
  4 | . . . # . # # . |
  5 | . # . # . # . . |
  6 | . # . . . # . # |
  7 | . . . # . # . P |
    +-----------------+

*/

			data[4].numero=5;

			data[4].robo.x=0;
			data[4].robo.y=0;
			
			data[4].destino.x=7;
			data[4].destino.y=7;
			
			data[4].elemento[1][0]=obstaculo;
			data[4].elemento[1][1]=obstaculo;
			data[4].elemento[1][2]=obstaculo;
			data[4].elemento[1][3]=obstaculo;
			data[4].elemento[1][5]=obstaculo;

			data[4].elemento[3][0]=obstaculo;
			data[4].elemento[3][2]=obstaculo;
			data[4].elemento[3][3]=obstaculo;
			data[4].elemento[3][4]=obstaculo;
			data[4].elemento[3][5]=obstaculo;

			data[4].elemento[4][2]=obstaculo;

			data[4].elemento[5][4]=obstaculo;

			data[4].elemento[1][6]=obstaculo;
			data[4].elemento[3][7]=obstaculo;
			data[4].elemento[5][0]=obstaculo;
			data[4].elemento[5][5]=obstaculo;
			data[4].elemento[5][6]=obstaculo;
			data[4].elemento[5][7]=obstaculo;
			data[4].elemento[6][2]=obstaculo;
			data[4].elemento[6][4]=obstaculo;
			data[4].elemento[7][2]=obstaculo;
			data[4].elemento[7][6]=obstaculo;

			
		
/*

     Tabuleiro Nro.: 6

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . # . # . . . . |
  1 | . . . o # . . . |
  2 | . # . # . . . . |
  3 | . . # . . . . . |
  4 | . # . # . . . . |
  5 | . . . . # . . . |
  6 | . . . . . . # . |
  7 | . . . . # . . P |
    +-----------------+

*/

			data[5].numero=6;

			data[5].robo.x=3;
			data[5].robo.y=1;
			
			data[5].destino.x=7;
			data[5].destino.y=7;
			
			data[5].elemento[1][0]=obstaculo;

			data[5].elemento[1][2]=obstaculo;

			data[5].elemento[1][4]=obstaculo;

			data[5].elemento[2][3]=obstaculo;

			data[5].elemento[3][0]=obstaculo;

			data[5].elemento[3][2]=obstaculo;

			data[5].elemento[3][4]=obstaculo;

			data[5].elemento[4][1]=obstaculo;

			data[5].elemento[4][5]=obstaculo;
			data[5].elemento[4][7]=obstaculo;
			data[5].elemento[6][6]=obstaculo;
			
		

/*

     Tabuleiro Nro.: 7 

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . . . . . . . |
  1 | . . . . . . . . |
  2 | # # . # . # . . |
  3 | o . . . . . . . |
  4 | # # # # # . # # |
  5 | . . . P # . . . |
  6 | . . . . . . # . |
  7 | . . . . # . . . |
    +-----------------+

*/

			data[6].numero=7;

			data[6].robo.x=0;
			data[6].robo.y=3;
			
			data[6].destino.x=3;
			data[6].destino.y=5;
			
			data[6].elemento[0][2]=obstaculo;
			data[6].elemento[1][2]=obstaculo;
	
			data[6].elemento[3][2]=obstaculo;
	
            data[6].elemento[5][2]=obstaculo;

			data[6].elemento[0][4]=obstaculo;
			data[6].elemento[1][4]=obstaculo;
			data[6].elemento[2][4]=obstaculo;
			data[6].elemento[3][4]=obstaculo;
			data[6].elemento[4][4]=obstaculo;

			data[6].elemento[4][5]=obstaculo;

			data[6].elemento[6][4]=obstaculo;
			data[6].elemento[7][4]=obstaculo;
			data[6].elemento[4][7]=obstaculo;
			data[6].elemento[6][6]=obstaculo;
			
		
/*

     Tabuleiro Nro.: 8

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | # . # . # . # . |
  1 | # . # . # . # . |
  2 | # . . . . . . . |
  3 | . . . # . # . . |
  4 | . # . # . # . . |
  5 | . # . # . # # # |
  6 | . # . . . . . . |
  7 | o # . # # # P . |
    +-----------------+

*/

			data[7].numero=8;

			data[7].robo.x=0;
			data[7].robo.y=7;
			
			data[7].destino.x=6;
			data[7].destino.y=7;
			
			data[7].elemento[0][0]=obstaculo;
			data[7].elemento[0][1]=obstaculo;
			data[7].elemento[0][2]=obstaculo;

			data[7].elemento[1][4]=obstaculo;
			data[7].elemento[1][5]=obstaculo;

            data[7].elemento[2][0]=obstaculo;
			data[7].elemento[2][1]=obstaculo;

			data[7].elemento[3][3]=obstaculo;
			data[7].elemento[3][4]=obstaculo;
			data[7].elemento[3][5]=obstaculo;

			data[7].elemento[4][0]=obstaculo;
			data[7].elemento[4][1]=obstaculo;
			
			data[7].elemento[5][3]=obstaculo;
			data[7].elemento[5][4]=obstaculo;
			data[7].elemento[5][5]=obstaculo;

			data[7].elemento[1][6]=obstaculo;
			data[7].elemento[1][7]=obstaculo;
			data[7].elemento[3][7]=obstaculo;
			data[7].elemento[4][7]=obstaculo;
			data[7].elemento[5][7]=obstaculo;
			data[7].elemento[6][0]=obstaculo;
			data[7].elemento[6][1]=obstaculo;
			data[7].elemento[6][5]=obstaculo;
			data[7].elemento[7][5]=obstaculo;
			
				
/*

     Tabuleiro Nro.: 9

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | x x x x x x . . |
  1 | x o . . . x . . |
  2 | x . x x . x . . |
  3 | x . x x . x . # |
  4 | x . . . . . . # |
  5 | x x x x x x . # |
  6 | . . . . . . . # |
  7 | P # . . . . . # |
    +-----------------+

*/

			data[8].numero=9;

			data[8].robo.x=1;
			data[8].robo.y=1;
			
			data[8].destino.x=0;
			data[8].destino.y=7;
			
			data[8].elemento[0][0]=obstaculo;
			data[8].elemento[0][1]=obstaculo;
			data[8].elemento[0][2]=obstaculo;
			data[8].elemento[0][3]=obstaculo;
			data[8].elemento[0][4]=obstaculo;
            data[8].elemento[0][5]=obstaculo;

			data[8].elemento[5][0]=obstaculo;
			data[8].elemento[5][1]=obstaculo;
			data[8].elemento[5][2]=obstaculo;
			data[8].elemento[5][3]=obstaculo;

			data[8].elemento[5][5]=obstaculo;
			
			data[8].elemento[1][0]=obstaculo;
			data[8].elemento[2][0]=obstaculo;
			data[8].elemento[3][0]=obstaculo;
			data[8].elemento[4][0]=obstaculo;
			
			data[8].elemento[1][5]=obstaculo;
			data[8].elemento[2][5]=obstaculo;
			data[8].elemento[3][5]=obstaculo;
			data[8].elemento[4][5]=obstaculo;
			
			data[8].elemento[2][2]=obstaculo;
			data[8].elemento[2][3]=obstaculo;
			data[8].elemento[3][2]=obstaculo;
			data[8].elemento[3][3]=obstaculo;

			data[8].elemento[7][3]=obstaculo;
			data[8].elemento[7][4]=obstaculo;
			data[8].elemento[7][5]=obstaculo;
			data[8].elemento[7][6]=obstaculo;
			data[8].elemento[7][7]=obstaculo;

			data[8].elemento[1][7]=obstaculo;
			
/*

     Tabuleiro Nro.: 10

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | o # # # # # # P |
  1 | . # . . # . # . |
  2 | . . . # . . . . |
  3 | . # . # # . . . |
  4 | . # . # # . . . |
  5 | . . . . # . . . |
  6 | . . . . # . . . |
  7 | . . . . . . . . |
    +-----------------+

*/

			data[9].numero=10;

			data[9].robo.x=0;
			data[9].robo.y=0;
			
			data[9].destino.x=7;
			data[9].destino.y=0;
			
			data[9].elemento[1][0]=obstaculo;
			data[9].elemento[2][0]=obstaculo;
			data[9].elemento[3][0]=obstaculo;
			data[9].elemento[4][0]=obstaculo;

			data[9].elemento[1][1]=obstaculo;
            data[9].elemento[1][3]=obstaculo;
			data[9].elemento[1][4]=obstaculo;

			data[9].elemento[3][2]=obstaculo;
			data[9].elemento[3][3]=obstaculo;
			data[9].elemento[3][4]=obstaculo;

			data[9].elemento[4][1]=obstaculo;
			data[9].elemento[4][3]=obstaculo;
			data[9].elemento[4][4]=obstaculo;
			
			data[9].elemento[5][0]=obstaculo;
			data[9].elemento[6][0]=obstaculo;
			data[9].elemento[6][1]=obstaculo;
			data[9].elemento[4][5]=obstaculo;
			data[9].elemento[4][6]=obstaculo;

/*

     Tabuleiro Nro.: 11

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | o # # # # # # P |
  1 | . # . . # . # . |
  2 | . . . # . . . . |
  3 | . # . # # . . . |
  4 | . # . # # . . . |
  5 | . . . . # . . . |
  6 | . . . . # . . # |
  7 | . . . . . . . . |
    +-----------------+

*/

			
			data[10].numero=11;

			data[10].robo.x=0;
			data[10].robo.y=0;
			
			data[10].destino.x=7;
			data[10].destino.y=0;
			
			data[10].elemento[1][0]=obstaculo;
			data[10].elemento[2][0]=obstaculo;
			data[10].elemento[3][0]=obstaculo;
			data[10].elemento[4][0]=obstaculo;

			data[10].elemento[1][1]=obstaculo;
            data[10].elemento[1][3]=obstaculo;
			data[10].elemento[1][4]=obstaculo;

			data[10].elemento[3][2]=obstaculo;
			data[10].elemento[3][3]=obstaculo;
			data[10].elemento[3][4]=obstaculo;

			data[10].elemento[4][1]=obstaculo;
			data[10].elemento[4][3]=obstaculo;
			data[10].elemento[4][4]=obstaculo;
			
			data[10].elemento[5][0]=obstaculo;
			data[10].elemento[6][0]=obstaculo;
			data[10].elemento[6][1]=obstaculo;
			data[10].elemento[4][5]=obstaculo;
			data[10].elemento[4][6]=obstaculo;

			data[10].elemento[7][6]=obstaculo;

/*

     Tabuleiro Nro.: 12

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | o # # # # # # P |
  1 | . # . . # . # . |
  2 | . . . # . . . . |
  3 | . # . # # . . . |
  4 | . # . # # . . . |
  5 | . . . . # . . . |
  6 | . . . . # . . . |
  7 | . . . . . . . # |
    +-----------------+

*/

			
			data[11].numero=12;

			data[11].robo.x=0;
			data[11].robo.y=0;
			
			data[11].destino.x=7;
			data[11].destino.y=0;
			
			data[11].elemento[1][0]=obstaculo;
			data[11].elemento[2][0]=obstaculo;
			data[11].elemento[3][0]=obstaculo;
			data[11].elemento[4][0]=obstaculo;

			data[11].elemento[1][1]=obstaculo;
            data[11].elemento[1][3]=obstaculo;
			data[11].elemento[1][4]=obstaculo;

			data[11].elemento[3][2]=obstaculo;
			data[11].elemento[3][3]=obstaculo;
			data[11].elemento[3][4]=obstaculo;

			data[11].elemento[4][1]=obstaculo;
			data[11].elemento[4][3]=obstaculo;
			data[11].elemento[4][4]=obstaculo;
			
			data[11].elemento[5][0]=obstaculo;
			data[11].elemento[6][0]=obstaculo;
			data[11].elemento[6][1]=obstaculo;
			data[11].elemento[4][5]=obstaculo;
			data[11].elemento[4][6]=obstaculo;

			data[11].elemento[7][7]=obstaculo;

/*

     Tabuleiro Nro.: 13

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . . . . # . P |
  1 | . # . . . . . . |
  2 | . . # . . . . # |
  3 | . . . # . . . . |
  4 | . . . . # . . . |
  5 | # . . . . # . . |
  6 | . . . . . . # . |
  7 | o . # . . . . . |
    +-----------------+

*/

			data[12].numero=13;

			data[12].robo.x=0;
			data[12].robo.y=7;
			
			data[12].destino.x=7;
			data[12].destino.y=0;
			
			data[12].elemento[0][5]=obstaculo;
			data[12].elemento[2][7]=obstaculo;

			data[12].elemento[1][1]=obstaculo;
			data[12].elemento[2][2]=obstaculo;
			data[12].elemento[3][3]=obstaculo;
			data[12].elemento[4][4]=obstaculo;
			data[12].elemento[5][5]=obstaculo;
			data[12].elemento[6][6]=obstaculo;

			data[12].elemento[5][0]=obstaculo;
			data[12].elemento[7][2]=obstaculo;


/*

     Tabuleiro Nro.: 14

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | o . . . . . . . |
  1 | # . . . # . . . |
  2 | # # . # # . . . |
  3 | # . # . # # # . |
  4 | # . . . . . . . |
  5 | # . . . # # # # |
  6 | . . . . . . P # |
  7 | . . . . . # # # |
    +-----------------+

*/

			data[13].numero=14;

			data[13].robo.x=0;
			data[13].robo.y=0;
			
			data[13].destino.x=6;
			data[13].destino.y=6;
			
			data[13].elemento[0][1]=obstaculo;
			data[13].elemento[0][2]=obstaculo;
			data[13].elemento[0][3]=obstaculo;
			data[13].elemento[0][4]=obstaculo;
			data[13].elemento[0][5]=obstaculo;

			data[13].elemento[1][2]=obstaculo;
			data[13].elemento[2][3]=obstaculo;
			data[13].elemento[3][2]=obstaculo;

			data[13].elemento[4][1]=obstaculo;
			data[13].elemento[4][2]=obstaculo;
			data[13].elemento[4][3]=obstaculo;
			data[13].elemento[4][5]=obstaculo;

			data[13].elemento[5][3]=obstaculo;
			data[13].elemento[6][3]=obstaculo;
			data[13].elemento[5][5]=obstaculo;
			data[13].elemento[6][5]=obstaculo;
			data[13].elemento[7][5]=obstaculo;
			data[13].elemento[7][6]=obstaculo;
			data[13].elemento[7][7]=obstaculo;
			data[13].elemento[6][7]=obstaculo;
			data[13].elemento[5][7]=obstaculo;


/*

     Tabuleiro Nro.: 15 

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . . . . . . . |
  1 | . # P # . # . # |
  2 | . . # . . . # . |
  3 | . . . . . . . . |
  4 | . . . # . # . . |
  5 | . . . . # . . . |
  6 | # . # . . . . . |
  7 | . # . . . . . o |
    +-----------------+

*/

			data[14].numero=15;

			data[14].robo.x=7;
			data[14].robo.y=7;
			
			data[14].destino.x=2;
			data[14].destino.y=1;
			
			data[14].elemento[1][1]=obstaculo;
			data[14].elemento[2][2]=obstaculo;
			data[14].elemento[3][1]=obstaculo;
			
			data[14].elemento[5][1]=obstaculo;
			data[14].elemento[6][2]=obstaculo;
			data[14].elemento[7][1]=obstaculo;
			
			data[14].elemento[3][4]=obstaculo;
			data[14].elemento[4][5]=obstaculo;
			data[14].elemento[5][4]=obstaculo;
			
			data[14].elemento[0][6]=obstaculo;
			data[14].elemento[1][7]=obstaculo;
			data[14].elemento[2][6]=obstaculo;
			


/*

     Tabuleiro Nro.: 16

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . . # # . . . |
  1 | . . . . . . . . |
  2 | . . . # # . . o |
  3 | . # # # # # # # |
  4 | . # # # # # # P |
  5 | . . . . . . # . |
  6 | . . . # # . . . |
  7 | . . . # # . . . |
    +-----------------+

*/

			data[15].numero=16;

			data[15].robo.x=7;
			data[15].robo.y=2;
			
			data[15].destino.x=7;
			data[15].destino.y=4;
		
			data[15].elemento[1][3]=obstaculo;
			data[15].elemento[2][3]=obstaculo;
			data[15].elemento[3][3]=obstaculo;
			data[15].elemento[4][3]=obstaculo;
			data[15].elemento[5][3]=obstaculo;
			data[15].elemento[6][3]=obstaculo;
			data[15].elemento[7][3]=obstaculo;
			data[15].elemento[1][4]=obstaculo;
			data[15].elemento[2][4]=obstaculo;
			data[15].elemento[3][4]=obstaculo;
			data[15].elemento[4][4]=obstaculo;
			data[15].elemento[5][4]=obstaculo;
			data[15].elemento[6][4]=obstaculo;

			data[15].elemento[3][0]=obstaculo;
			data[15].elemento[4][0]=obstaculo;
			data[15].elemento[3][2]=obstaculo;
			data[15].elemento[4][2]=obstaculo;

			data[15].elemento[3][6]=obstaculo;
			data[15].elemento[4][6]=obstaculo;
			data[15].elemento[3][7]=obstaculo;
			data[15].elemento[4][7]=obstaculo;

			data[15].elemento[6][5]=obstaculo;

/*

     Tabuleiro Nro.: 17

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . . . . . . . |
  1 | . # # # # # . . |
  2 | . . . . . # . . |
  3 | # # P # . # . . |
  4 | . . # # # # # . |
  5 | # . . . . # . . |
  6 | # # # # . # . # |
  7 | o . . . . . . . |
    +-----------------+

*/

			data[16].numero=17;

			data[16].robo.x=0;
			data[16].robo.y=7;
			
			data[16].destino.x=2;
			data[16].destino.y=3;
			
			data[16].elemento[1][1]=obstaculo;
			data[16].elemento[2][1]=obstaculo;
			data[16].elemento[3][1]=obstaculo;
			data[16].elemento[4][1]=obstaculo;
			data[16].elemento[5][1]=obstaculo;

			data[16].elemento[5][2]=obstaculo;
			data[16].elemento[5][3]=obstaculo;
			data[16].elemento[5][4]=obstaculo;
			data[16].elemento[5][5]=obstaculo;
			data[16].elemento[5][6]=obstaculo;

			data[16].elemento[0][6]=obstaculo;
			data[16].elemento[1][6]=obstaculo;
			data[16].elemento[2][6]=obstaculo;
			data[16].elemento[3][6]=obstaculo;

			data[16].elemento[0][3]=obstaculo;
			data[16].elemento[1][3]=obstaculo;
			data[16].elemento[3][3]=obstaculo;
			data[16].elemento[2][4]=obstaculo;
			data[16].elemento[3][4]=obstaculo;
			data[16].elemento[4][4]=obstaculo;

			data[16].elemento[0][5]=obstaculo;
			
			data[16].elemento[7][6]=obstaculo;

			data[16].elemento[6][4]=obstaculo;
			
/*

     Tabuleiro Nro.: 18

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . # . . . # . o |
  1 | . # . # . # . . |
  2 | . . . # . . . . |
  3 | . # # # # # # # |
  4 | . # . # . # . . |
  5 | . # . . . # . . |
  6 | . . . # . . . . |
  7 | . # . # . # . P |
    +-----------------+

*/

			data[17].numero=18;

			data[17].robo.x=7;
			data[17].robo.y=0;
			
			data[17].destino.x=7;
			data[17].destino.y=7;
			
			data[17].elemento[1][3]=obstaculo;
			data[17].elemento[2][3]=obstaculo;
			data[17].elemento[3][3]=obstaculo;
			data[17].elemento[4][3]=obstaculo;
			data[17].elemento[5][3]=obstaculo;
			data[17].elemento[6][3]=obstaculo;
			data[17].elemento[7][3]=obstaculo;
			
			data[17].elemento[1][0]=obstaculo;
			data[17].elemento[1][1]=obstaculo;
			data[17].elemento[1][4]=obstaculo;
			data[17].elemento[1][5]=obstaculo;
			data[17].elemento[1][7]=obstaculo;
			
			data[17].elemento[3][1]=obstaculo;
			data[17].elemento[3][2]=obstaculo;
			data[17].elemento[3][4]=obstaculo;
			data[17].elemento[3][6]=obstaculo;
			data[17].elemento[3][7]=obstaculo;
			
			data[17].elemento[5][0]=obstaculo;
			data[17].elemento[5][1]=obstaculo;
			data[17].elemento[5][4]=obstaculo;
			data[17].elemento[5][5]=obstaculo;
			data[17].elemento[5][7]=obstaculo;
			
/*

     Tabuleiro Nro.: 19

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . # . . . # . |
  1 | . . # . # . . . |
  2 | . . . . # # # . |
  3 | # # # . # P # . |
  4 | . . . . # . # . |
  5 | . # # # # . # . |
  6 | . # . . . . # . |
  7 | o # . . . . . . |
    +-----------------+

*/

			data[18].numero=19;

			data[18].robo.x=0;
			data[18].robo.y=7;
			
			data[18].destino.x=5;
			data[18].destino.y=3;
		
			data[18].elemento[0][3]=obstaculo;
			data[18].elemento[1][3]=obstaculo;
			data[18].elemento[2][3]=obstaculo;

			data[18].elemento[2][0]=obstaculo;
			data[18].elemento[2][1]=obstaculo;

			data[18].elemento[1][5]=obstaculo;
			data[18].elemento[1][6]=obstaculo;
			data[18].elemento[1][7]=obstaculo;

			data[18].elemento[4][1]=obstaculo;
			data[18].elemento[4][2]=obstaculo;
			data[18].elemento[4][3]=obstaculo;
			data[18].elemento[4][4]=obstaculo;
			data[18].elemento[4][5]=obstaculo;
			
			data[18].elemento[2][5]=obstaculo;
			data[18].elemento[3][5]=obstaculo;

			data[18].elemento[6][0]=obstaculo;
			data[18].elemento[6][2]=obstaculo;
			data[18].elemento[6][3]=obstaculo;
			data[18].elemento[6][4]=obstaculo;
			data[18].elemento[6][5]=obstaculo;
			data[18].elemento[6][6]=obstaculo;

			data[18].elemento[5][2]=obstaculo;
/*

     Tabuleiro Nro.: 20

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . . . . . P . |
  1 | . # . . . . # . |
  2 | . . . # # . . . |
  3 | . . # # # # . . |
  4 | . # # # # # # . |
  5 | . # # o . # # . |
  6 | . # . . . . # . |
  7 | . . . . . . . . |
    +-----------------+

*/

			data[19].numero=20;

			data[19].robo.x=3;
			data[19].robo.y=5;
			
			data[19].destino.x=6;
			data[19].destino.y=0;
			
			data[19].elemento[1][4]=obstaculo;
			data[19].elemento[1][5]=obstaculo;
			data[19].elemento[1][6]=obstaculo;

			data[19].elemento[1][1]=obstaculo;

			data[19].elemento[2][3]=obstaculo;
			data[19].elemento[2][4]=obstaculo;
			data[19].elemento[2][5]=obstaculo;

			data[19].elemento[3][2]=obstaculo;
			data[19].elemento[3][3]=obstaculo;
			data[19].elemento[3][4]=obstaculo;

			data[19].elemento[4][2]=obstaculo;
			data[19].elemento[4][3]=obstaculo;
			data[19].elemento[4][4]=obstaculo;

			data[19].elemento[5][3]=obstaculo;
			data[19].elemento[5][4]=obstaculo;
			data[19].elemento[5][5]=obstaculo;

			data[19].elemento[6][4]=obstaculo;
			data[19].elemento[6][5]=obstaculo;
			data[19].elemento[6][6]=obstaculo;

			data[19].elemento[6][1]=obstaculo;


/*

     Tabuleiro Nro.: 21

      0 1 2 3 4 5 6 7
    +-----------------+
  0 | . . . . . . . . |
  1 | . . . . . . . . |
  2 | . . . # # # . . |
  3 | . . . # P # . . |
  4 | . . . # # # . . |
  5 | . . . . . . . . |
  6 | . . . . . . . . |
  7 | o . . . . . . . |
    +-----------------+

*/

			data[20].numero=21;

			data[20].robo.x=0;
			data[20].robo.y=7;
			
			data[20].destino.x=4;
			data[20].destino.y=3;
			
			data[20].elemento[3][2]=obstaculo;
			data[20].elemento[3][3]=obstaculo;
			data[20].elemento[3][4]=obstaculo;

			data[20].elemento[5][2]=obstaculo;
			data[20].elemento[5][3]=obstaculo;
			data[20].elemento[5][4]=obstaculo;

			data[20].elemento[4][2]=obstaculo;

			data[20].elemento[4][4]=obstaculo;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      init_tabuleiro                                     //
// Descricao:   Rotina para devolucao de um certo tabuleiro dos    //
//              tabuleiros padrao                                  //
// Parametros:  Numero do tabuleiro escolhido                      //
// Retorno:     Tabuleiro escolhido                                //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: Funcao para compatibilidade com a versao anterior  //
//              do sistema                                         //
/////////////////////////////////////////////////////////////////////

TABULEIRO init_tabuleiro(int id)
{
	TABULEIRO tabuleiro_local;

	tabuleiro_local=data[id];

	return tabuleiro_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      init_caminho                                       //
// Descricao:   Inicializa um caminho com valores "dummy"          //
// Parametros:  -                                                  //
// Retorno:     Caminho inicializado                               //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

CAMINHO init_caminho(void)
{
	int i;
	
	CAMINHO caminho_local;

	for(i=0;i<caminhos_max;i++)
	{
		caminho_local.pos[i].x=dummy;  
		caminho_local.pos[i].y=dummy;  
	}

	return caminho_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      mostra_tabuleiro                                   //
// Descricao:   Mostra o tabuleiro na tela                         //
// Parametros:  Tabuleiro a ser mostrado, Definicao se visao atual //
//              do robo e os giros achados devem ser mostrados     //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void mostra_tabuleiro(TABULEIRO tabuleiro_local, BOOL visao, BOOL giros)
{
	int i, j;

	setcolor(XFOREGROUND_WHITE | XBACKGROUND_BLACK);
	janela(x_start,y_start,x_max*2,y_max);

	gotoxy(x_start+2,y_start);
	printf(" %i ",tabuleiro_local.numero);

	for(i=0;i<x_max;i++)
		for(j=0;j<y_max;j++)
		{
			gotoxy(x_start+(i*2)+1,y_start+j+1);
			if(tabuleiro_local.elemento[i][j]==obstaculo)
			{
				if(visao && tabuleiro_local.visao[i][j]) cor_visao(obstaculo); else
					setcolor(XFOREGROUND_LIGHTRED | XBACKGROUND_BLACK);
				printf("%c%c",obstaculo_ch,obstaculo_ch);
			} else
				if(visao && tabuleiro_local.visao[i][j])
				{
					cor_visao(vazio);
					printf("  ");
				}
		}

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLUE );

	gotoxy(6,11);

	printf("( DEBUG INFO");

	gotoxy(6,13);

	printf("Visao......: %i   ",gera_codigo_visao(tabuleiro_local));

	gotoxy(6,14);

	printf("Obstaculos.: %i )   ",gera_codigo_obstaculos(tabuleiro_local));

	if(giros)
	{
		setcolor(XFOREGROUND_YELLOW | XBACKGROUND_BLACK);

		for(i=0;i<x_max*y_max;i++)
		{
			if(tabuleiro_local.giro[i].x!=dummy)
			{
				gotoxy(x_start+tabuleiro_local.giro[i].x*2+1,y_start+tabuleiro_local.giro[i].y+1);
				printf("%i%c",i,down_ch);
			}		
		}

//		gotoxy(1,16);

//		for(i=0;i<x_max*y_max;i++)
//		{
//			if(tabuleiro_local.giro[i].x!=dummy)
//			{
//				printf("%i:%i,%i; ",i,tabuleiro_local.giro[i].x,tabuleiro_local.giro[i].y);
//			}
//		}
		

	}

	if(visao) 
		cor_visao(robo); else
		setcolor(XFOREGROUND_LIGHTBLUE | XBACKGROUND_BLACK);
	gotoxy(x_start+(tabuleiro_local.robo.x)*2+1,y_start+tabuleiro_local.robo.y+1);
	printf("%s",robo_ch);

	if(visao && tabuleiro_local.visao[tabuleiro_local.destino.x][tabuleiro_local.destino.y]) 
		cor_visao(destino); 
		else setcolor(XFOREGROUND_LIGHTGREEN | XBACKGROUND_BLACK);
	gotoxy(x_start+(tabuleiro_local.destino.x)*2+1,y_start+tabuleiro_local.destino.y+1);
	if(tabuleiro_local.destino.x==tabuleiro_local.robo.x && 
		tabuleiro_local.destino.y==tabuleiro_local.robo.y) printf("OK"); else
		printf("%s",destino_ch);

	cor_padrao();
	
	gotoxy(x_start+1,y_start-1);
	for(i=0;i<x_max;i++)
		printf("%i ",i+1);

	for(i=0;i<y_max;i++)
	{
		gotoxy(x_start-2,y_start+1+i);
		printf("%i",i+1);
	}

}

/////////////////////////////////////////////////////////////////////
// Funcao:      estabelece_visao                                   //
// Descricao:   Rotina para definir as possibilidades de visao do  //
//              robo em uma determinada situacao no tabuleiro      //
// Parametros:  Tabuleiro para estabelecer a visao                 //
// Retorno:     Tabuleiro com visualizacao da visao                //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

TABULEIRO estabelece_visao(TABULEIRO tabuleiro_local)
{
	int i, j, limite_x_cima_dir, limite_x_cima_esq, limite_x_baixo_dir, limite_x_baixo_esq;

	tabuleiro_local.visao[tabuleiro_local.robo.x][tabuleiro_local.robo.y]=True;

	// visao para cima

	if(tabuleiro_local.robo.y>-1)
	{
		i=tabuleiro_local.robo.y;

		limite_x_cima_dir=x_max;
		limite_x_cima_esq=-1;

		do
		{

			if(tabuleiro_local.elemento[tabuleiro_local.robo.x][i]!=obstaculo)
			{
				tabuleiro_local.visao[tabuleiro_local.robo.x][i]=True;

				j=tabuleiro_local.robo.x;

				if(j>limite_x_cima_esq)
				{
					while(j>limite_x_cima_esq && tabuleiro_local.elemento[j][i]!=obstaculo)
					{
						tabuleiro_local.visao[j][i]=True;
						j--;
					} 
				}
				limite_x_cima_esq=j;

				j=tabuleiro_local.robo.x;
				if(j<limite_x_cima_dir)
				{
					while(j<limite_x_cima_dir && tabuleiro_local.elemento[j][i]!=obstaculo)
					{
						tabuleiro_local.visao[j][i]=True;
						j++;
					} 
				}
				limite_x_cima_dir=j;
			}

			i--;

		} while(i>-1 && tabuleiro_local.elemento[tabuleiro_local.robo.x][i]!=obstaculo);
	}

	// visao para baixo

	if(tabuleiro_local.robo.y<y_max)
	{
		i=tabuleiro_local.robo.y;

		limite_x_baixo_dir=x_max;
		limite_x_baixo_esq=-1;

		do
		{

			if(tabuleiro_local.elemento[tabuleiro_local.robo.x][i]!=obstaculo)
			{
				tabuleiro_local.visao[tabuleiro_local.robo.x][i]=True;

				j=tabuleiro_local.robo.x;

				if(j>limite_x_baixo_esq)
				{
					while(j>limite_x_baixo_esq && tabuleiro_local.elemento[j][i]!=obstaculo)
					{
						tabuleiro_local.visao[j][i]=True;
						j--;
					} 
				}
				limite_x_baixo_esq=j;

				j=tabuleiro_local.robo.x;
				if(j<limite_x_baixo_dir)
				{
					while(j<limite_x_baixo_dir && tabuleiro_local.elemento[j][i]!=obstaculo)
					{
						tabuleiro_local.visao[j][i]=True;
						j++;
					} 
				}
				limite_x_baixo_dir=j;
			}

			i++;

		} while(i<y_max && tabuleiro_local.elemento[tabuleiro_local.robo.x][i]!=obstaculo);
	}


	return tabuleiro_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      estabelece_giros                                   //
// Descricao:   Rotina que determina local e numero de giros a     //
//              partir de uma determinada situacao                 //
// Parametros:  Tabuleiro para definicao dos giros                 //
// Retorno:     Tabuleiro com pontos de giro definidos             //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

TABULEIRO estabelece_giros(TABULEIRO tabuleiro_local)
{
	int i, j;

	BOOL ja_marcado;

	for(i=0;i<x_max*y_max;i++)
	{
		tabuleiro_local.giro[i].x=dummy;
		tabuleiro_local.giro[i].x=dummy;
	}

	tabuleiro_local.giros_pos=0;

	for (i=0;i<x_max;i++)
		for (j=0;j<y_max;j++)
		{
			if(tabuleiro_local.visao[i][j])
			{
				ja_marcado=False;

				if(i>0)
					if(tabuleiro_local.elemento[i-1][j]==vazio && tabuleiro_local.visao[i-1][j]==False)
					{
						tabuleiro_local.giro[tabuleiro_local.giros_pos].x=i;
						tabuleiro_local.giro[tabuleiro_local.giros_pos].y=j;
						tabuleiro_local.giros_pos++;
						ja_marcado=True;
					}
				if(i<x_max-1 && ja_marcado==False)
					if(tabuleiro_local.elemento[i+1][j]==vazio && tabuleiro_local.visao[i+1][j]==False)
					{
						tabuleiro_local.giro[tabuleiro_local.giros_pos].x=i;
						tabuleiro_local.giro[tabuleiro_local.giros_pos].y=j;
						tabuleiro_local.giros_pos++;
						ja_marcado=True;
					}
				if(j>0 && ja_marcado==False)
					if(tabuleiro_local.elemento[i][j-1]== vazio && tabuleiro_local.visao[i][j-1]==False)
					{
						tabuleiro_local.giro[tabuleiro_local.giros_pos].x=i;
						tabuleiro_local.giro[tabuleiro_local.giros_pos].y=j;
						tabuleiro_local.giros_pos++;
						ja_marcado=True;
					}
				if(j<y_max-1 && ja_marcado==False)
					if(tabuleiro_local.elemento[i][j+1]==vazio && tabuleiro_local.visao[i][j+1]==False)
					{
						tabuleiro_local.giro[tabuleiro_local.giros_pos].x=i;
						tabuleiro_local.giro[tabuleiro_local.giros_pos].y=j;
						tabuleiro_local.giros_pos++;
					}
			}
		}


	return tabuleiro_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      gera_codigo_visao                                  //
// Descricao:   Rotina que determina um codigo especifico a partir //
//              da visao que o robo estabeleceu dentro de um       //
//              labirinto                                          //
// Parametros:  Tabuleiro para definicao do codigo                 //
// Retorno:     Codigo estabelecido                                //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

int gera_codigo_visao(TABULEIRO tabuleiro_local)
{

	int i, j, k, codigo;

	codigo=0;

	k=1;

	for(i=0;i<x_max;i++)
		for(j=0;j<y_max;j++)
		{
			if(tabuleiro_local.visao[i][j])//tabuleiro_local.elemento[i][j]==obstaculo)
				codigo=codigo+k;

			k++;
		}

	codigo=(int((k*tan(codigo))));

	if(codigo>0) while(codigo>64) codigo=codigo-64;

	if(codigo<0) while(codigo<-64) codigo=codigo+64;

	return codigo;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      gera_codigo_obstaculos                             //
// Descricao:   Rotina que determina um codigo numerico a partir   //
//              dos obstaculos vistos pelo robo                    //
// Parametros:  Tabuleiro para definicao do codigo                 //
// Retorno:     Codigo estabelecido                                //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

int gera_codigo_obstaculos(TABULEIRO tabuleiro_local)
{

	int i, j, k, codigo;

	BOOL adicionar_obstaculo;

	codigo=0;

	k=1;

	for(i=0;i<x_max;i++)
		for(j=0;j<y_max;j++)
		{
			adicionar_obstaculo=False;

			if(tabuleiro_local.elemento[i][j]==obstaculo)
			{
				if(i>0)
					if(tabuleiro_local.visao[i-1][j]==True)
						adicionar_obstaculo=True;

				if(i<x_max-1)
					if(tabuleiro_local.visao[i+1][j]==True)
						adicionar_obstaculo=True;

				if(j>0)
					if(tabuleiro_local.visao[i][j-1]==True)
						adicionar_obstaculo=True;

				if(j<y_max-1)
					if(tabuleiro_local.visao[i][j+1]==True)
						adicionar_obstaculo=True;
			}

			if(adicionar_obstaculo==True) codigo++;

			k++;
		}

	//gotoxy(1,2);

	//printf("obstaculos: %i",codigo);

	codigo=(int((k*tan(codigo))));

	if(codigo>0) while(codigo>64) codigo=codigo-64;

	if(codigo<0) while(codigo<-64) codigo=codigo+64;

	return codigo;
}


/////////////////////////////////////////////////////////////////////
// Funcao:      anti_loop                                          //
// Descricao:   Rotina para eliminacao de "loops" nos caminhos     //
//              achados que fatalmente resultariam em "stack       //
//              overflow", demora e caminhos ineficientes          //
// Parametros:  Posicao a ser analisada e caminho a ser comparado  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

BOOL anti_loop(XY test_pos, CAMINHO cam_local)
{
	int c;

	BOOL flag;

	flag=False;

	for(c=0;c<(x_max*y_max);c++)
	{
		if(test_pos.x==cam_local.pos[c].x && test_pos.y==cam_local.pos[c].y) flag=True;
	}

	return flag;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      conta_elementos                                    //
// Descricao:   Conta os pontos dentro de um caminho               //
// Parametros:  Caminho a ser analisado                            //
// Retorno:     Numero de elementos                                //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

int conta_elementos(CAMINHO cam_local)
{
	int flag;

	flag=0;

	if(cam_local.pos[0].x==dummy) return 999; // para nao retornar 0 e provocar que o algoritmo pense
	                                          // que seja o menor caminho!!!

	while(cam_local.pos[flag].x!=dummy) flag++;

	return flag;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      calcula distancia                                  //
// Descricao:   Calcula a distancia total percorrida em um         //
//              determinado caminho (levando em consideracao que   //
//              o robo nao se movimentara diagonalmente)           //
// Parametros:  Caminho a ser analisado                            //
// Retorno:     Distancia em numero de casas percorridas           //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

int calcula_distancia(CAMINHO cam_local)
{
	int flag, m;

	flag=0;

	if(cam_local.pos[0].x==dummy  ||
		cam_local.pos[1].x==dummy) 
		return 0;

	m=1;

	while(cam_local.pos[m].x!=dummy && m<x_max*y_max) 
	{
		flag=flag+abs(cam_local.pos[m].x-cam_local.pos[m-1].x);
		flag=flag+abs(cam_local.pos[m].y-cam_local.pos[m-1].y);
		m++;
	}

	return flag;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      algoritmo_simbolico                                //
// Descricao:   Gerenciador recursivo do algoritmo simbolico       //
//              (abre um diagrama de arvore)                       //
// Parametros:  Proxima posicao do caminho, tabuleiro e caminho    //
//              atual                                              //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

CAMINHO algoritmo_simbolico(XY nova_pos, TABULEIRO tab_local, CAMINHO rota)
{
	int a, b;

	tab_local.robo.x=nova_pos.x;
	tab_local.robo.y=nova_pos.y;

	b=0;

	while(tab_local.cam_mem.pos[b].x!=dummy)
		b++;

	tab_local.cam_mem.pos[b].x=nova_pos.x;
	tab_local.cam_mem.pos[b].y=nova_pos.y;

    tab_local=estabelece_visao(tab_local);

	tab_local=estabelece_giros(tab_local);

//	if(debug)
//		mostra_tabuleiro(tab_local,True,True); // debug

	if(tab_local.visao[tab_local.destino.x][tab_local.destino.y]==False
		&& conta_elementos(tab_local.cam_mem)<conta_elementos(rota)) // destino nao foi achado ainda!!
	{

		a=0;

		while(tab_local.giro[a].x!=dummy)
		{
			if(anti_loop(tab_local.giro[a],tab_local.cam_mem)==False)
				rota=algoritmo_simbolico(tab_local.giro[a],tab_local,rota);
			a++;
		}
	} else
	{
		b++;

		tab_local.cam_mem.pos[b].x=tab_local.destino.x;
		tab_local.cam_mem.pos[b].y=tab_local.destino.y;

		if((conta_elementos(tab_local.cam_mem)<conta_elementos(rota))
			||(conta_elementos(tab_local.cam_mem)==conta_elementos(rota) && calcula_distancia(tab_local.cam_mem)<calcula_distancia(rota)))
		{
			rota=tab_local.cam_mem;
			
//			if(debug)	// output debug
//			{
//				gotoxy(1,21);
//				d=0;
//				printf("Caminho: ");
//				while(rota.pos[d].x!=dummy) 
//				{
//					printf(" %i,%i -",rota.pos[d].x,rota.pos[d].y);
//					d++;
//				}
//				printf(" .. pos:%i dist:%i",conta_elementos(rota),calcula_distancia(rota));
//				getch();
//			}
		}
	}

	return rota;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      execute_caminho                                    //
// Descricao:   Rotina que executa um caminho estabelecido pelo    //
//              processador simbolico                              //
// Parametros:  Tabuleiro atual, caminho, e elementos de processa- //
//              mento a serem mostrados                            //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void execute_caminho(TABULEIRO tabuleiro_local2, CAMINHO caminho_exec, BOOL esc_vis, BOOL esc_gir)
{
	int p;

	p=0;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();

	caixa_mensagem(simples,0,6," ALGORITMO SIMBOLICO ");

	caixa_mensagem(simples,0,11,"Processando....");

	while(caminho_exec.pos[p].x!=dummy)
	{
		tabuleiro_local2.robo.x=caminho_exec.pos[p].x;
		tabuleiro_local2.robo.y=caminho_exec.pos[p].y;

		tabuleiro_local2=estabelece_visao(tabuleiro_local2);

		tabuleiro_local2=estabelece_giros(tabuleiro_local2);

		mostra_tabuleiro(tabuleiro_local2,esc_vis,esc_gir);

		tecla();

		clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

		titulo();

		caixa_mensagem(simples,0,6," ALGORITMO SIMBOLICO ");

		caixa_mensagem(chave,0,11," Processando.. ");

		p++;
	}
}

/////////////////////////////////////////////////////////////////////
// Funcao:      init_mapa                                          //
// Descricao:   Inicializacao de um mapa                           //
// Parametros:  -                                                  //
// Retorno:     Mapa inicializado                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

MAPA init_mapa(void)
{
	MAPA mapa_local2;

	int i2, j2;

	for(i2=0;i2<x_max;i2++)
		for(j2=0;j2<y_max;j2++)
			mapa_local2.preferencia[i2][j2]=0.0;

	return mapa_local2;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      aprende_caminho                                    //
// Descricao:   Aprendizado de um caminho estabelecido para um     //
//              certo tabuleiro                                    //
// Parametros:  Tabuleiro correspondente, caminho aplicavel        //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void aprende_caminho(TABULEIRO tabuleiro_local2, CAMINHO caminho_exec)
{
	int p, p2, acertos;

	MAPA mapa_local;

	SAIDA saida_local;

	float maior_valor;

	int maior_ponto;

	XY maior_pos;

	p=0;

	acertos=0;

	while(caminho_exec.pos[p+1].x!=dummy && 
		(caminho_exec.pos[p+1].x!=tabuleiro_local2.destino.x || caminho_exec.pos[p+1].y!=tabuleiro_local2.destino.y))
	{
		tabuleiro_local2.robo.x=caminho_exec.pos[p].x;
		
		tabuleiro_local2.robo.y=caminho_exec.pos[p].y;

		tabuleiro_local2=estabelece_visao(tabuleiro_local2);

		tabuleiro_local2=estabelece_giros(tabuleiro_local2);

		//mostra_tabuleiro(tabuleiro_local2,True,True);

		//getch();

		rede=feedforward(tabuleiro_local2,rede);

		saida_local=extrai_saidas(rede);

		mapa_local=cria_mapa_das_saidas(saida_local);

		p2=1;

		maior_pos.x=tabuleiro_local2.giro[0].x;
		maior_pos.y=tabuleiro_local2.giro[0].y;

		maior_valor=mapa_local.preferencia[tabuleiro_local2.giro[0].x][tabuleiro_local2.giro[0].y];

		//printf("\n\nPONTOS DE GIROS\n");

		//printf("pos:%i\n\n",tabuleiro_local2.giros_pos);

		//printf("%i,%i - %f\n",tabuleiro_local2.giro[0].x,maior_pos.y=tabuleiro_local2.giro[0].y,
//				mapa_local.preferencia[tabuleiro_local2.giro[0].x][tabuleiro_local2.giro[0].y]);

		while(p2<tabuleiro_local2.giros_pos)
		{
			if(mapa_local.preferencia[tabuleiro_local2.giro[p2].x][tabuleiro_local2.giro[p2].y]>maior_valor)
			{
				maior_pos.x=tabuleiro_local2.giro[p2].x;
				maior_pos.y=tabuleiro_local2.giro[p2].y;

				maior_valor=mapa_local.preferencia[tabuleiro_local2.giro[p2].x][tabuleiro_local2.giro[p2].y];
				maior_ponto=p2;

				
		//		printf("%i,%i - %f\n",tabuleiro_local2.giro[p2].x,maior_pos.y=tabuleiro_local2.giro[p2].y,
		//			mapa_local.preferencia[tabuleiro_local2.giro[p2].x][tabuleiro_local2.giro[p2].y]);

			}
			p2++;
		}

		//printf("\n\nESCOLHIDO\n");

		//printf("[%i %i %f - %i %i]\n",maior_pos.x,maior_pos.y,maior_valor,caminho_exec.pos[p+1].x,caminho_exec.pos[p+1].y);

		//getch();

		if(maior_pos.x!=caminho_exec.pos[p+1].x || maior_pos.y!=caminho_exec.pos[p+1].y)
		{

			p2=0;

			while(p2<tabuleiro_local2.giros_pos)
			{
				if(p2!=maior_ponto) taxa_erro=taxa_erro+mapa_local.preferencia[tabuleiro_local2.giro[p2].x][tabuleiro_local2.giro[p2].y];
				mapa_local.preferencia[tabuleiro_local2.giro[p2].x][tabuleiro_local2.giro[p2].y]=0.0;
				p2++;
			}

			taxa_erro=float(taxa_erro+(1.0-mapa_local.preferencia[caminho_exec.pos[p+1].x][caminho_exec.pos[p+1].y]));
			mapa_local.preferencia[caminho_exec.pos[p+1].x][caminho_exec.pos[p+1].y]=1.0;

//			mostra_mapa(mapa_local);

//			mostra_tabuleiro(tabuleiro_local2,True,True);

			saida_local=cria_saidas_do_mapa(mapa_local);

		//	printf("learn!\n");

			rede=backpropagation(rede,saida_local);

		} else acertos++;
		
		p++;
	}

	//printf("[ ");

	if(acertos<p) setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_BLUE ); else
		setcolor( XFOREGROUND_LIGHTGREEN | XBACKGROUND_BLUE );

	printf("%c%c",s4,s4);

	cor_padrao();

	printf(" ");

	printf("(%i / %i)",acertos,p);

	if(acertos<p) todos_ok=False;

	total_acertos=total_acertos+acertos;

	total_giros=total_giros+p;

//	getch();

//	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );
}

/////////////////////////////////////////////////////////////////////
// Funcao:      use_algoritmo_simbolico                            //
// Descricao:   Rotina "frontend" para demonstracao do processa-   //
//              mento simbolico                                    //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void use_algoritmo_simbolico(void)
{
	TABULEIRO tabuleiro_local;

	CAMINHO caminho_certo;

	int i, escolha_processamento;
		
	BOOL escolha_visao, escolha_giros;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();
	
	caixa_mensagem(simples,0,6," ALGORITMO SIMBOLICO ");

	escolha_processamento=escolha(0,10,vision);

	switch(escolha_processamento)
	{
	case 0: escolha_visao=False;
		escolha_giros=False;
		break;
	case 1: escolha_visao=True;
		escolha_giros=False;
		break;
	case 2: escolha_visao=False;
		escolha_giros=True;
		break;
	case 3: escolha_visao=True;
		escolha_giros=True;
	}

	for(i=0;i<max_tabuleiros;i++)
	{
		caminho_certo=init_caminho();
		tabuleiro_local=init_tabuleiro(i);
		caminho_certo=algoritmo_simbolico(tabuleiro_local.robo,tabuleiro_local,caminho_certo);

		if(caminho_certo.pos[0].x!=dummy)
			execute_caminho(tabuleiro_local,caminho_certo,escolha_visao,escolha_giros); else
		{
			clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );
			titulo();
			caixa_mensagem(simples,0,6," ALGORITMO SIMBOLICO ");
			mostra_tabuleiro(tabuleiro_local,escolha_visao,escolha_giros);
			caixa_mensagem(alerta,0,21," Nao ha caminho nesta posicao! ");
		}
	}
}

/////////////////////////////////////////////////////////////////////
// Funcao:      use_rede_nerual                                    //
// Descricao:   Rotina "frontend" para uso da rede neural a partir //
//              do menu principal                                  //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void use_rede_neural(void)
{
	TABULEIRO tabuleiro_local;

	CAMINHO caminho_local;

	SAIDA saida_local;

	MAPA mapa_local;

	BOOL erro_no_processamento, escolha_visao, escolha_giros;

	int i, j, maior_pos, caminho_pos, escolha_processamento;

	float maior_valor;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();
	
	caixa_mensagem(simples,0,6," REDE NEURAL ");

	escolha_processamento=escolha(0,10,vision);

	switch(escolha_processamento)
	{
	case 0: escolha_visao=False;
		escolha_giros=False;
		break;
	case 1: escolha_visao=True;
		escolha_giros=False;
		break;
	case 2: escolha_visao=False;
		escolha_giros=True;
		break;
	case 3: escolha_visao=True;
		escolha_giros=True;
	}

	for(i=0;i<max_tabuleiros;i++)
	{
		clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

		titulo();

		caixa_mensagem(simples,0,6," REDE NEURAL ");

		caminho_local=init_caminho();
		
		tabuleiro_local=init_tabuleiro(i);

		caminho_pos=1;

		caminho_local.pos[0].x=tabuleiro_local.robo.x;
		caminho_local.pos[0].y=tabuleiro_local.robo.y;

		erro_no_processamento=False;
		
		while(tabuleiro_local.visao[tabuleiro_local.destino.x][tabuleiro_local.destino.y]==False 
			&& erro_no_processamento==False)
		{
			tabuleiro_local=estabelece_visao(tabuleiro_local);

			tabuleiro_local=estabelece_giros(tabuleiro_local);

			if(tabuleiro_local.giros_pos==0 && tabuleiro_local.visao[tabuleiro_local.destino.x][tabuleiro_local.destino.y]==False)
				erro_no_processamento=True;

			if(erro_no_processamento==False)
			{

				mostra_tabuleiro(tabuleiro_local,escolha_visao,escolha_giros);

				tecla();

				rede=feedforward(tabuleiro_local,rede);

				saida_local=extrai_saidas(rede);

				mapa_local=cria_mapa_das_saidas(saida_local);

				maior_pos=0;

				maior_valor=mapa_local.preferencia[tabuleiro_local.giro[0].x][tabuleiro_local.giro[0].y];

				if(tabuleiro_local.giros_pos>1)
					for(j=0;j<tabuleiro_local.giros_pos;j++)
					{
						if(mapa_local.preferencia[tabuleiro_local.giro[j].x][tabuleiro_local.giro[j].y]>maior_valor)
						{
							maior_valor=mapa_local.preferencia[tabuleiro_local.giro[j].x][tabuleiro_local.giro[j].y];
							maior_pos=j;
						}

					}

				tabuleiro_local.robo.x=tabuleiro_local.giro[maior_pos].x;
				tabuleiro_local.robo.y=tabuleiro_local.giro[maior_pos].y;
			}

		}

		if(erro_no_processamento==False)
		{
		
			tabuleiro_local.robo.x=tabuleiro_local.destino.x;
			tabuleiro_local.robo.y=tabuleiro_local.destino.y;

			mostra_tabuleiro(tabuleiro_local,escolha_visao,escolha_giros);

			caixa_mensagem(aviso,0,21," O robo chegou ao ponto final. Labirinto resolvido com sucesso. ");

		} else
		{
			mostra_tabuleiro(tabuleiro_local,escolha_visao,escolha_giros);

			caixa_mensagem(alerta,0,21," Sem caminho ate o destino. A rede nao pode resolver esta situacao. ");
		}
	}
}

/////////////////////////////////////////////////////////////////////
// Funcao:      aprende_caminhos                                   //
// Descricao:   Rotina mestre para o aprendizado de todos os       //
//              caminhos utilizando rotino escravo aprende_caminho //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void aprende_caminhos(void)
{
	TABULEIRO tabuleiro_local;

	int i, iteracao, progresso_pos, j, k;

	float percentagem;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	cor_padrao();

	titulo();

	caixa_mensagem(simples,0,6," APRENDIZADO DOS CAMINHOS ");

	gotoxy(2,10);

	caixa_mensagem(chave,0,10," Elaborando caminhos com algoritmo simbolico... ");

	progresso_pos=(80-(max_tabuleiros*2))/2;

	gotoxy(progresso_pos,14);

	for(i=0;i<max_tabuleiros;i++) printf("%c%c",s1,s1);

	gotoxy(progresso_pos,14);
	
	for(i=0;i<max_tabuleiros;i++)
	{
		cache.cam[i]=init_caminho();
		tabuleiro_local=init_tabuleiro(i);
		cache.cam[i]=algoritmo_simbolico(tabuleiro_local.robo,tabuleiro_local,cache.cam[i]);

		printf("%c%c",s4,s4);
	}

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	cor_padrao();

	titulo();

	caixa_mensagem(simples,0,6," APRENDIZADO DOS CAMINHOS - MONITORAMENTO ");

	iteracao=0;

	todos_ok=False;

	//gotoxy(28,7);

	//printf("------------------------");

	cor_padrao();

	gotoxy(46,10);

	printf("Iteracao atual : _");
	
	gotoxy(46,12);

	printf("Total acertos  : ___");

	percentagem=float(total_acertos*100.0/total_giros*1.0);

	gotoxy(46,14);

	printf("Percentagem    : ");

	for(j=0;j<10;j++) printf("%c",s1);

	gotoxy(46,16);

	printf("Erro total     : _______");

	gotoxy(46,18);

	printf("Erro relativo  : _______");

	for(i=1;i<11;i++)
	{
		gotoxy(10,9+i);
		if(i<10) printf("0");
			
		printf("%i : __ _______",i);
	}

	for(i=11;i<22;i++)
	{
		gotoxy(27,-1+i);
				
		printf("%i : __ _______",i);
	}

	caixa_mensagem(chave,0,22," Processando. Interromper com qualquer tecla. ");

	while(kbhit()) getch();

	while(!kbhit() && todos_ok==False)
	{
		gotoxy(63,10);
		printf("%i",iteracao+1);
		todos_ok=True;
		total_giros=0;
		total_acertos=0;
		taxa_erro=0.0;

		for(i=0;i<max_tabuleiros;i++)
		{
		//	caminho_certo=init_caminho();
			tabuleiro_local=init_tabuleiro(i);
		//	caminho_certo=algoritmo_simbolico(tabuleiro_local.robo,tabuleiro_local,caminho_certo);

			if(i<10)
			gotoxy(15,10+i); else 
			  gotoxy(32,i);
			
			if(cache.cam[i].pos[0].x!=dummy)
				aprende_caminho(tabuleiro_local,cache.cam[i]); else
			{
				setcolor( XFOREGROUND_LIGHTBLUE | XBACKGROUND_BLUE );

				printf("%c%c",s4,s4);

				printf(" (Erro!)");

				cor_padrao();
				
			}
		}

		gotoxy(63,12);

		printf("%i / %i",total_acertos,total_giros);

		percentagem=float(total_acertos*100.0/total_giros*1.0);

		gotoxy(63,14);

		k=int(percentagem/10);

		for(j=0;j<k;j++) printf("%c",s4);

		for(j=k;j<10;j++) printf("%c",s1);

		printf(" ");

		//if(percentagem<70.0) setcolor( XFOREGROUND_WHITE | XBACKGROUND_RED ); else
		//	if(percentagem<90.0) setcolor( XFOREGROUND_WHITE | XBACKGROUND_LIGHTBLUE ); else
		//			setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN );
		
		printf("%i",int(percentagem));

		cor_padrao();

		gotoxy(63,16);

		//setcolor( XFOREGROUND_LIGHTPURPLE | XBACKGROUND_BLUE );

		printf("%f ",taxa_erro);

		cor_padrao();

		//setcolor( XFOREGROUND_LIGHTPURPLE | XBACKGROUND_BLUE );

		gotoxy(63,18);

		printf("%f ",taxa_erro/total_giros*1.0);

		cor_padrao();

		iteracao++;
	}

	while(kbhit()) getch();

	if(todos_ok==True)
		caixa_mensagem(aviso,0,21," Finalizado (todos os caminhos aprendidos com sucesso)!! ");
	 else
		caixa_mensagem(alerta,0,21," Aprendizado interrompido (os dados atuais serao mantidos). ");
	
}

/////////////////////////////////////////////////////////////////////
// Funcao:      sigmoide                                           //
// Descricao:   Funcao de ativacao dos neuronios da rede           //
// Parametros:  Valor base para qual devera ser calculado o        //
//              sigmoide                                           //
// Retorno:     Sigmoide do valor base                             //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

float sigmoide(float valor)
{
	valor=float(1.0/(1.0+exp(-valor))); // sigmoide:=1.0/(1.0+EXP(-soma));

	return valor;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      init_rede                                          //
// Descricao:   Inicializacao da rede neural                       //
// Parametros:  -                                                  //
// Retorno:     Rede inicializada                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

REDE_NEURAL init_rede(void)
{
	REDE_NEURAL rede_local;

	int i, j;

	for(i=0;i<nr_entradas;i++)	// inicializa entradas com 0
		rede_local.entrada[i]=0;

	for(i=0;i<nr_escondidos;i++)	// inicializa neuronios escondidos com 0
		rede_local.escondido[i]=0;

	for(i=0;i<nr_saidas;i++)	// inicializa saidas com 0
		rede_local.saida[i]=0;

	for(i=0;i<nr_entradas;i++)  // inicializa sinapses 1: valores de -0.1 a 0.1
		for(j=0;j<nr_escondidos;j++)
			rede_local.sinapse1[i][j]=float(rand()/(RAND_MAX*5.0)-0.1);

	for(i=0;i<nr_escondidos;i++)  // sinapses 2, dto. sinapses 1
		for(j=0;j<nr_saidas;j++)
			rede_local.sinapse2[i][j]=float(rand()/(RAND_MAX*5.0)-0.1);

	return rede_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      dump_rede                                          //
// Descricao:   Simples dump dos valores da rede para debug        //
// Parametros:  Rede neural                                        //
// Retorno:     -                                                  //
// Versao:      0.2 (interface melhorada)                          //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void dump_rede(REDE_NEURAL rede_local, BOOL apenas_entradas)
{
	int i, j, k;

	cor_padrao();

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();

	caixa_mensagem(simples,0,6," DUMP DA REDE - ENTRADAS ");
	
	gotoxy(16,10);

	for(i=0;i<nr_entradas;i++) 
	{
		if(i%2==0) setcolor( XFOREGROUND_LIGHTGREEN | XBACKGROUND_BLUE); else
			setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_BLUE);
		printf("%i  ",int(rede_local.entrada[i]));
		if((i+1)%16==0) gotoxy(16,10+(i+1)/16);
	}

	cor_padrao();

	printf("\n\n\n");

	tecla();

	if(apenas_entradas==False)
	{

    clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();

	caixa_mensagem(simples,0,6," DUMP DA REDE - SINAPSES 1A. CAMADA ");	
	
	gotoxy(2,10);

	setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_BLUE );

	printf("[POSICAO: %i / %i] ",0,nr_entradas*nr_escondidos);

	cor_padrao();

	k=1;

	for(i=0;i<nr_entradas;i++)
	{
		for(j=0;j<nr_escondidos;j++)
		{
			printf("%f  ",rede_local.sinapse1[i][j]);
			if(k%80==0)
			{
				printf("\n\n\n");

				tecla();

				clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

				titulo();

				caixa_mensagem(simples,0,6," DUMP DA REDE - SINAPSES 1A. CAMADA ");	
	
				gotoxy(2,10);

				setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_BLUE );

				printf("[POSICAO: %i / %i] ",k,nr_entradas*nr_escondidos);

				cor_padrao();
			}
			k++;
		}
	
	}

	printf("\n\n\n");

	tecla();

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();

	caixa_mensagem(simples,0,6," DUMP DA REDE - NEURONIOS ESCONDIDOS ");

	gotoxy(2,10);

	for(i=0;i<nr_escondidos;i++) 
		printf("%f  ",rede_local.escondido[i]);
		
	printf("\n\n\n");

	tecla();

    clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();

	caixa_mensagem(simples,0,6," DUMP DA REDE - SINAPSES 2A. CAMADA ");
	
	gotoxy(2,10);

	setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_BLUE );

	printf("[POSICAO: %i / %i] ",0,nr_escondidos*nr_saidas);

	cor_padrao();

	k=1;

	for(i=0;i<nr_escondidos;i++)
	{
		for(j=0;j<nr_saidas;j++)
		{
			printf("%f  ",rede_local.sinapse2[i][j]);

			if(k%80==0)
			{
				printf("\n\n\n");

				tecla();

				clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

				titulo();

				caixa_mensagem(simples,0,6," DUMP DA REDE - SINAPSES 2A. CAMADA ");
	
				gotoxy(2,10);

				setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_BLUE );

				printf("[POSICAO: %i / %i] ",k,nr_escondidos*nr_saidas);

				cor_padrao();

			}

			k++;
		}

	}

	printf("\n\n\n");

	tecla();

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();

	caixa_mensagem(simples,0,6," DUMP DA REDE - SAIDAS ");

	gotoxy(2,10);

	for(i=0;i<nr_saidas;i++) 
	{
		printf("%f  ",rede_local.saida[i]);
		if((i+1)%150==0) getch();
	}

	tecla();
	}

}

/////////////////////////////////////////////////////////////////////
// Funcao:      feedforward                                        //
// Descricao:   Processamento da rede neural                       //
// Parametros:  Tabuleiro inicial a ser analisado e a rede neural  //
//              em "estado cru"                                    //
// Retorno:     Rede neural alterada pelo processamento            //
// Versao:      0.1 - Versao inicial                               //
// Observacoes:                                                    //
//             -Topologia escolhida: Perceptron de 2 Camadas       //
//                                                                 //
//             -Aprendizado escolhido: Backpropagation             //
//                                                                 //
//             -O tabuleiro e mapeado para as entradas             //
//             -Cada elemento do tabuleiro e codificado com 3      //
//              entradas para observar a caracteristica binaria    //
//             -Todas as entradas estao conectadas com todos os    //
//              neuronios da camada escondida                      //
//             -Todos os neuronios da camada escondida estao       //
//              conectados com todas as saidas                     //
//             -Cada elemento da camada se saida corresponde a     //
//              uma pontuacao de preferencia de um certo ponto     //
//              de giro                                            //
//                                                                 //
//             -Nao toda rede sera usada para avaliacao de uma     //
//              situacao, ela serve apenas para decidir entre      //
//              os possiveis pontos de giro a partir de uma        //
//              situacao no tabuleiro                              //
//                                                                 //
/////////////////////////////////////////////////////////////////////

REDE_NEURAL feedforward(TABULEIRO tabuleiro_local, REDE_NEURAL rede_local)
{

	int i, j, k, x_mem, y_mem, codigo, codigo2;

	k=0;

	// E N T R A D A

	codigo=gera_codigo_visao(tabuleiro_local);

	codigo2=gera_codigo_obstaculos(tabuleiro_local);

//	mostra_tabuleiro(tabuleiro_local,True,True);

//	gotoxy(1,2);
//	printf("%i",codigo2);

//	tecla();

	for(i=0;i<nr_entradas;i++)	// inicializacao para nao ficar com restos de situacoes antigas na entrada
		rede_local.entrada[i]=False;

	for(j=0;j<y_max;j++)		// situacao do tabuleiro para entradas da rede
		for(i=0;i<x_max;i++)
		{
			if(tabuleiro_local.visao[i][j]==True) 
			{
				//rede_local.entrada[k*nro_codigos+elemento_visibilidade]=True;
				if(tabuleiro_local.elemento[i][j]!=vazio)
					rede_local.entrada[k*nro_codigos+elemento_obstaculo]=True;
			}

			if(codigo>0 && k<codigo) rede_local.entrada[k*nro_codigos+elemento_adicional]=True;

			if(codigo<0 && (63-k)<abs(codigo)) rede_local.entrada[k*nro_codigos+elemento_adicional]=True;

			if(i==tabuleiro_local.robo.x || j==tabuleiro_local.robo.y)
			{
				if(rede_local.entrada[k*nro_codigos+elemento_adicional]==True) 
					rede_local.entrada[k*nro_codigos+elemento_adicional]=False; else
					rede_local.entrada[k*nro_codigos+elemento_adicional]=True;
			}

			if(codigo2>0 && k<codigo2) rede_local.entrada[k*nro_codigos+elemento_obstaculo]=True;

			if(codigo2<0 && (63-k)<abs(codigo2)) rede_local.entrada[k*nro_codigos+elemento_obstaculo]=True;

			k++;
		}

//	dump_rede(rede_local,True);

	x_mem=tabuleiro_local.robo.x;
	y_mem=tabuleiro_local.robo.y;


	// C A M A D A   E S C O N D I D A

	for(i=0;i<nr_escondidos;i++) // inicializacao dos neuronios escondidos - necessaria por causa da soma a seguir
		rede_local.escondido[i]=0.0;

	for(i=0;i<nr_escondidos;i++)	// entrada para dendritos neuronios escondidos
		for(j=0;j<nr_entradas;j++)
			rede_local.escondido[i]=rede_local.escondido[i]+rede_local.entrada[j]*rede_local.sinapse1[j][i];

	for(i=0;i<nr_escondidos;i++)	// dendritos escondidos para axonios (usando mesmo campo...)
		rede_local.escondido[i]=sigmoide(rede_local.escondido[i]);



	// S A I D A

	for(i=0;i<nr_saidas;i++) // inicializacao das saidas - necessaria por causa da soma a seguir
		rede_local.saida[i]=0.0;

	for(i=0;i<nr_saidas;i++)		// escondidos para dendritos das saidas
		for(j=0;j<nr_escondidos;j++)
			rede_local.saida[i]=rede_local.saida[i]+rede_local.escondido[j]*rede_local.sinapse2[j][i];

	for(i=0;i<nr_saidas;i++)	// dendritos saidas para axonios (tambem usando mesmo campo...)
		rede_local.saida[i]=sigmoide(rede_local.saida[i]);




	return rede_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      backpropagation                                    //
// Descricao:   Rotina para aprendizado da rede                    //
// Parametros:  Rede neural apos processamento e a saida desejada  //
// Retorno:     Rede neural alterada pelo aprendizado              //
// Versao:      0.1 - Versao inicial                               //
// Observacoes:                                                    //
//              Formulas aplicadas                                 //
//              ------------------                                 //
//                                                                 //
//             1.Erros na camada de saida                          //
//                                                                 //
//                   o2[j]=o[j](1-o[j])(y[j]-o[j]) p/ j=0,...,C    //
//                                                                 //
//             2.Erros na camada escondida                         //
//                                                                 //
//                   o1[j]=h[j](1-h[j]).soma3                      //
//                                                                 //
//                      onde soma3 = soma(i=1 a c)o2[i].w2[i][j]   //
//                                                                 //
//             3.Ajuste entre camada escondida e saida             //
//                                                                 //
//                   dw2[i][j]=n.o2[j].h[i] p/ i=0,...,B e         //
//                                             j=0,...,C           //
//                                                                 //
//             4.Ajuste entre camada de entrada e camada escondida //
//                                                                 //
//                   dw1[i][j]=n.o1[j].x[i] p/ i=0,...,A e         //
//                                             j=0,...,B           //
//                                                                 //
//              Legenda                                            //
//              -------                                            //
//                                                                 //
//                   A   : nro de entradas                         //
//                   B   : nro de neuronios na camada escondida    //
//                   C   : nro de saidas                           //
//                                                                 //
//                   o   : saida real da rede                      //
//                   y   : saida desejada                          //
//                   h   : axonios da camada escondida             //
//                   w2  : valor de sinapses entre camada          //
//                        escondida e saida                        //
//                   n   : taxa de aprendizado                     //
//                   x   : valor da entrada                        //
//                                                                 //
//                   o2  : erro nas saidas                         //
//                   o1  : erro na camada escondida                //
//                   dw2 : ajuste (diferenca) nas sinapses entre   //
//                         camada escondida e saida                //
//                   dw1 : ajuste (diferenca) nas sinapses entre   //
//                         entrada e camada escondida              //
//                                                                 //
/////////////////////////////////////////////////////////////////////

REDE_NEURAL backpropagation(REDE_NEURAL rede_local, SAIDA saida_local)
{
	const int c = nr_saidas; // nro de unidades na camada de saida

	const int b = nr_escondidos; // elementos na camada escondida 

	const int a = nr_entradas; // elementos na camada de entrada

	const float n = float(0.35); // taxa de aprendizado

	float o2[c], o1[b], soma3; // erros na saida

	float dw2[b][c], dw1[a][b]; // ajustes

	int i, j;

	/////////////////////////////////////////////////////////////////////
	//             1.Erros na camada de saida                          //
	//                                                                 //
	//                   o2[j]=o[j](1-o[j])(y[j]-o[j]) p/ j=0,...,C    //
	/////////////////////////////////////////////////////////////////////

	for(i=0;i<c;i++)  
		o2[i]=float(rede_local.saida[i]*(1.0-rede_local.saida[i])*
		      (saida_local.valor[i]-rede_local.saida[i]));

//	if(debug)
//	{
//		printf("O2\n\n");
//		for(i=0;i<c;i++)
//			printf("%f\n",o2[i]);
//		getch();
//	}

	/////////////////////////////////////////////////////////////////////
	//             2.Erros na camada escondida                         //
	//                                                                 //
	//                   o1[j]=h[j](1-h[j]).soma3                      //
	//                                                                 //
	//                      onde soma3 = soma(i=1 a c)o2[i].w2[i][j]   //
	/////////////////////////////////////////////////////////////////////

	for(j=0;j<b;j++) 
	{
		soma3=0.0;

		for(i=0;i<c;i++)
			soma3=soma3+(o2[i]*rede_local.sinapse2[j][i]); // soma3 para cada passo

		o1[j]=float(rede_local.escondido[j]*(1.0-rede_local.escondido[j])*soma3);
	}

//	if(debug)
//	{
//		printf("O1\n\n");
//		for(i=0;i<c;i++)
//			printf("%f\n",o1[i]);
//		getch();
//	}

	/////////////////////////////////////////////////////////////////////
	//             3.Ajuste entre camada escondida e saida             //
	//                                                                 //
	//                   dw2[i][j]=n.o2[j].h[i] p/ i=0,...,B e         //
	//                                             j=0,...,C           //
	/////////////////////////////////////////////////////////////////////

	for(i=0;i<b;i++)	
		for(j=0;j<c;j++)
		{
			dw2[i][j]=n*o2[j]*rede_local.escondido[i]; // calculo dos ajustes entre camada escondida e saida
			rede_local.sinapse2[i][j]=rede_local.sinapse2[i][j]+dw2[i][j]; // executar ajustes aproveitando o mesmo laco
		}

	/////////////////////////////////////////////////////////////////////
	//             4.Ajuste entre camada de entrada e camada escondida //
	//                                                                 //
	//                   dw1[i][j]=n.o1[j].x[i] p/ i=0,...,A e         //
	//                                             j=0,...,B           //
	/////////////////////////////////////////////////////////////////////

	for(i=0;i<a;i++)	
		for(j=0;j<b;j++)
		{
			dw1[i][j]=n*o1[j]*rede_local.entrada[i]; // calculo dos ajustes entre camada de entrada e escondida
			rede_local.sinapse1[i][j]=rede_local.sinapse1[i][j]+dw1[i][j]; // executar ajustes aproveitando o mesmo laco
		}

	return rede_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      extrai_saidas                                      //
// Descricao:   Rotina em si completamente banal que extrai as     //
//              saidas isolando-as numa variavel                   //
// Parametros:  Rede neural (com as saidas a serem trabalhadas)    //
// Retorno:     Saidas em forma de array                           //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

SAIDA extrai_saidas(REDE_NEURAL rede_local)
{
	int i;

	SAIDA saida_local;

	for(i=0;i<nr_saidas;i++)
		saida_local.valor[i]=rede_local.saida[i];

	return saida_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      cria_mapa_das_saidas                               //
// Descricao:   Rotina para apresentacao das saidas da rede em     //
//              forma da matriz inicial                            //
// Parametros:  Saida isolada da rede                              //
// Retorno:     Saidas em forma de matriz                          //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

MAPA cria_mapa_das_saidas(SAIDA saida_local)
{
	int i, j, k;

	MAPA mapa_local;

	k=0;

	for(i=0;i<x_max;i++)
		for(j=0;j<y_max;j++)
		{
			mapa_local.preferencia[i][j]=saida_local.valor[k];
			k++;
		}

	return mapa_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      cria_saidas_do_mapa                                //
// Descricao:   Rotina inversa da anterior para criacao da saida   //
//              na forma da rede a partir da matriz                //
// Parametros:  Saida da rede em forma de matriz                   //
// Retorno:     Saidas em forma sequencial                         //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

SAIDA cria_saidas_do_mapa(MAPA mapa_local)
{
	int i, j, k;

	SAIDA saida_local;

	k=0;

	for(i=0;i<x_max;i++)
		for(j=0;j<y_max;j++)
		{
			saida_local.valor[k]=mapa_local.preferencia[i][j];
			k++;
		}

	return saida_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      modificar_tabuleiro                                //
// Descricao:   Rotina para modificacao de um tabuleiro do conjunto//
//              fixo programado                                    //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.2 - Inclusao de modificacao de ate 21 labirintos //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void modificar_tabuleiro(void)
{
	char entrada;

	int cur_x, cur_y, tabuleiro_nr, modo;

	BOOL saida_laco;

	TABULEIRO tabuleiro_local;

	tabuleiro_local=init_tabuleiro(0);

	tabuleiro_local.robo.x=dummy;
	tabuleiro_local.robo.y=dummy;

	tabuleiro_local.destino.x=dummy;
	tabuleiro_local.destino.y=dummy;

	modo=menu1;

	saida_laco=False;

	do
	{
		clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

		titulo();

		caixa_mensagem(simples,0,6," MODIFICACAO TABULEIRO ");


		if(modo==menu1)

			tabuleiro_nr=escolha(0,10,labirinto); else
			tabuleiro_nr=escolha(0,10,labirinto2);

		if(modo==menu1 && tabuleiro_nr<10)
		{
			tabuleiro_local=data[tabuleiro_nr];
			saida_laco=True;
		}else if(modo==menu1)
			modo=menu2;
		else if(modo==menu2 && tabuleiro_nr<11)
		{
			tabuleiro_local=data[tabuleiro_nr+10];
			saida_laco=True;
		}else if(modo==menu2)
			modo=menu1;

	}while(saida_laco==False);

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();

	caixa_mensagem(simples,0,6," MODIFICACAO TABULEIRO ");

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLACK );

	caixa_mensagem(simples,0,21," r = robo, d = destino, o = obstaculo, . = vazio, enter = fim. ");

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLACK );

	cur_x=0;
	cur_y=0;

	while(entrada!=char(13))
	{
		mostra_tabuleiro(tabuleiro_local,False,False);

		gotoxy(x_start+1+(cur_x*2),y_start+1+cur_y);

		entrada=getch();

		switch(entrada)
		{
		case 'o': if(((cur_x!=tabuleiro_local.destino.x) || (cur_y!=tabuleiro_local.destino.y))
					  && ((cur_x!=tabuleiro_local.robo.x) || (cur_y!=tabuleiro_local.robo.y)))
					  tabuleiro_local.elemento[cur_x][cur_y]=obstaculo; break;
		case '.': tabuleiro_local.elemento[cur_x][cur_y]=vazio; break;
		case 'r': tabuleiro_local.robo.x=cur_x;
			tabuleiro_local.robo.y=cur_y;
			break;
		case 'd': tabuleiro_local.destino.x=cur_x;
			tabuleiro_local.destino.y=cur_y;
			break;
        case char(72) : if(cur_y>0) cur_y--; break;
		case char(80) : if(cur_y<y_max-1) cur_y++; break;
		case char(75) : if(cur_x>0) cur_x--; break;
		case char(77) : if(cur_x<x_max-1) cur_x++; break;
		}

	}
	
	cor_padrao();
	
	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	titulo();

	caixa_mensagem(simples,0,6," MODIFICACAO TABULEIRO ");

	if(tabuleiro_local.destino.x==dummy || tabuleiro_local.robo.x==dummy)
	{
		caixa_mensagem(alerta,0,13," Erro em coordenadas de robo ou destino. ");
		return;
	}

	mostra_tabuleiro(tabuleiro_local,False,False);

	if(caixa_mensagem(pergunta,0,21," Substituir tabuleiro original? ")==True)
	{
		if(modo==menu1)
			data[tabuleiro_nr]=tabuleiro_local; else
			data[tabuleiro_nr+10]=tabuleiro_local;
	}
}

/////////////////////////////////////////////////////////////////////
// Funcao:      mostra_tabuleiros                                  //
// Descricao:   Rotina mestre e frontend para uso com a rotina     //
//              mostra_tabuleiro mostrando todos os tabuleiros na  //
//              tela                                               //
// Parametros:  -                                                  //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: -                                                  //
/////////////////////////////////////////////////////////////////////

void mostra_tabuleiros(void)
{
	TABULEIRO tabuleiro_local;

	int i;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	cor_padrao();

	caixa_mensagem(simples,0,6," LABIRINTOS ATIVOS ");

	titulo();

	for(i=0;i<max_tabuleiros;i++)
	{

		tabuleiro_local=init_tabuleiro(i);

		// tabuleiro_local=estabelece_visao(tabuleiro_local);

		// tabuleiro_local=estabelece_giros(tabuleiro_local);

		mostra_tabuleiro(tabuleiro_local,False,False);

		tecla();

		clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

		clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

		caixa_mensagem(simples,0,6," LABIRINTOS ATIVOS ");

		titulo();
	
	}
}

/////////////////////////////////////////////////////////////////////
// Funcao:      mostra_saida                                       //
// Descricao:   Mostra a saida em forma de array                   //
// Parametros:  Saida da rede em forma de array                    //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: apenas para debug das funcoes de conversao         //
/////////////////////////////////////////////////////////////////////

void mostra_saida(SAIDA saida_local)
{
	int i;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	printf("DUMP DE ARRAY DE SAIDAS\n\n");

	for(i=0;i<nr_saidas;i++)
		printf("%f - ",saida_local.valor[i]);

	getch();
}

/////////////////////////////////////////////////////////////////////
// Funcao:      mostra_mapa                                        //
// Descricao:   Mostra a matriz da saida                           //
// Parametros:  Saida da rede em forma de matriz                   //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: apenas para debug das funcoes de conversao         //
/////////////////////////////////////////////////////////////////////

void mostra_mapa(MAPA mapa_local)
{
	int i, j;

	printf("DUMP DE MATRIZ DE SAIDAS\n\n");

	for(i=0;i<y_max;i++)
	{
		for(j=0;j<x_max;j++)
			printf("%f - ",mapa_local.preferencia[j][i]);
		printf("\n");
	}

	getch();

}

/////////////////////////////////////////////////////////////////////
// Funcao:      gera_saida_para_teste                              //
// Descricao:   Rotina para teste do algoritmo de aprendizado      //
// Parametros:  Os pontos a ficarem ligados na saida               //
// Retorno:     Saida em forma de array                            //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: apenas para debug                                  //
/////////////////////////////////////////////////////////////////////

SAIDA gera_saida_para_teste(int positivo1, int positivo2)
{
	SAIDA saida_local;

	int i;

	for(i=0;i<nr_saidas;i++)
		saida_local.valor[i]=0.0;

	saida_local.valor[positivo1]=1.0;

	saida_local.valor[positivo2]=1.0;

	return saida_local;
}

/////////////////////////////////////////////////////////////////////
// Funcao:      testa_teclado                                      //
// Descricao:   Funcao para te em forma de array                   //
// Parametros:  Saida da rede em forma de array                    //
// Retorno:     -                                                  //
// Versao:      0.1 - Versao inicial                               //
// Observacoes: apenas para debug das funcoes de conversao         //
/////////////////////////////////////////////////////////////////////

void testa_teclado(void)
{
	char ch_local;

    ch_local='+';

	while(ch_local!='x')
	{
		ch_local=getch();

		printf("%i ",abs(ch_local));
	}
}