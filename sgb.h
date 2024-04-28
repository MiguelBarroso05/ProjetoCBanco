/**
 * Author: Marco Cândido, Miguel Barroso e Pedro Marques
 * Date: 14/04/2024
 * Description: Implementa funções relativas à gestão bancária
 * Projeto final de avaliação de módulo UFCD 0782
 **/

/**
 * Esta biblioteca deve ser complementada com o vosso código
 **/
#ifndef SGB_H
#define SGB_H
/**
 * ######  Mensagens da Biblioteca ######
 **/
// Mensagens de resposta a pedidos
#include <stdio.h>

#define SHOW_BANK_INFO(DATA, NOME, NUM_CLIENTES, NUM_CONTAS, SALDO_TOTAL) printf("####  %s  ####\nData Atual: %d\nClientes: %d\nContas: %d\nValor em Cofre: %.2lf €\n", NOME, DATA, NUM_CLIENTES, NUM_CONTAS, SALDO_TOTAL)
#define NEW_DATE_OK(DATE,JUROS) printf("Nova data atual: %d\nJuros atualizados: %.2lf €\n",DATE,JUROS)
#define INSERT_CLIENT_SUCESS(ID, NAME) printf("Cliente %d, %s, inserido com sucesso\n", ID, NAME)
#define LIST_USERS_HEADER(TOTAL_CLIENTS) printf("#######  %d Clientes no Banco  #######\n", TOTAL_CLIENTS)
#define LIST_USERS_ITEM(ID, NIF, NAME, NUM_ACCOUNTS, TOTAL_VALUE) printf("%d - %s - %s - Contas: %d - %.2lf€\n", ID, NIF, NAME, NUM_ACCOUNTS, TOTAL_VALUE)
#define CLIENT_INFO(DATA, ID, NAME, NIF, NUM_ACCOUNTS, TOTAL_VALUE) printf("###### INFO CLIENTE: %d (data: %d) ######\nNOME: %s\nNIF: %s\nNUM. CONTAS: %d\nPOSIÇÃO INTEGRADA: %.2lf €\n", ID, DATA, NAME, NIF, NUM_ACCOUNTS, TOTAL_VALUE)
#define CLIENT_INFO_ACCOUNTS(ID_CONTA, SALDO) printf("  # Conta Num: %s - Saldo: %.2lf €\n", ID_CONTA, SALDO)
#define CLIENT_INFO_MOVEMENTS(ID_MOV, TIPO, VALOR) printf("   -> Movimento %d - %s : %.2lf €\n", ID_MOV, TIPO, VALOR)
#define CLIENT_DELETE_OK(ID) printf("Cliente %d apagado com sucesso\n", ID)
#define CLIENTS_LOOKUP_FOOTER(NUM_CLIENTS_FOUND, TERMO) printf("# Foram encontrados %d clientes com o termo: %s\n", NUM_CLIENTS_FOUND, TERMO)
#define ACCOUNT_CREATION_SUCESS(ID_ACCOUNT, ID_CLIENT) printf("Conta %s criada com sucesso para o cliente %d\n", ID_ACCOUNT, ID_CLIENT)
#define LIST_ACCOUNTS_HEADER(ID_CLIENT, NAME, TOTAL_VALUE) printf("## Cliente %d : %s : %.2lf € ##\n", ID_CLIENT, NAME, TOTAL_VALUE)
#define ACCOUNT_CLOSE_OK(ID_ACCOUNT, ID_CLIENT) printf("Conta %s pertencente ao cliente %d fechada com sucesso\n", ID_ACCOUNT, ID_CLIENT)
#define DEPOSIT_OK(VALUE, ID_ACCOUNT) printf("Depósito de %.2lf € efetuado com sucesso na conta %s\n", VALUE, ID_ACCOUNT)
#define WITHDRAW_OK(VALUE, ID_ACCOUNT) printf("Levantamento de %.2lf € efetuado com sucesso da conta %s\n", VALUE, ID_ACCOUNT)
#define TRANSFER_OK(VALUE, ID_ORIGIN, ID_DESTINATION) printf("Transferência de %.2lf € efetuada com sucesso. %s -> %s\n", VALUE, ID_ORIGIN, ID_DESTINATION)
#define LIST_BANK_MOVEMENTS_HEADER(TOTAL_MV) printf("#######  %d Movimentos no Banco  #######\n",TOTAL_MV)
#define LIST_BANK_MOVEMENTS_ITEM(ID,DIA,ID_CLIENTE,CODE_CONTA,TIPO,VALOR) printf("   -> [%04d] dia: %3d | Cliente: %2d [%s] | %13s : %.2lf €\n",ID,DIA,ID_CLIENTE,CODE_CONTA,TIPO,VALOR)
#define LIST_BEST_CLIENTS_HEADER puts("Lista dos Melhores Clientes:")
#define LIST_BEST_CLIENTS_ITEM(ID, NIF, NAME, TOTAL_VALUE) printf("   ->%d - %s - %s - %.2lf€\n", ID, NIF, NAME, TOTAL_VALUE)
// Mensagens de Erro
#define INSERT_CLIENT_ERROR_ALREADY_EXISTS(NIF) printf("Erro! Cliente com NIF:%s já existe!\n", NIF)
#define NO_CLIENTS_ERROR printf("Erro! Não existem clientes no banco\n")
#define NO_SUCH_CLIENT_ERROR(ID) printf("Erro! O cliente com o ID %d não existe\n", ID)
#define NO_CLIENTS_FOUND_ERROR(TERMO) printf("Não Foram encontrados clientes através da Pesquisa: %s\n", TERMO)
#define NEW_ACCOUNT_ERROR_CLIENT_OVRELOADED(ID) printf("Erro! O cliente ID %d já não pode possuir mais contas\n", ID)
#define CLIENT_DONT_HAVE_ACCOUNTS(ID) printf("O cliente %d não possui contas\n", ID)
#define NO_SUCH_ACCOUNT_ERROR(ID_CLIENT, ID_ACCOUNT) printf("Erro! O cliente com o ID %d não possuí a conta %s\n", ID_CLIENT, ID_ACCOUNT)
#define WITHDRAWAL_ERROR_NO_FUNDS(ID_CLIENT, FUNDS) printf("O cliente %d não possui saldo integrado suficiente (%.2lf) \n", ID_CLIENT, FUNDS)
#define NUMBER_OF_DAYS_INCORRECT(DIAS) printf("Erro! O número de dias inserido (%d) não é >=0.\n", DIAS)
#define NO_MOVEMENTS_ERROR printf("Erro! Não existem movimentos no banco\n")
// Macros com Strings para solicitar dados
#define ASK_NAME "Introduza o nome do Cliente"
#define ASK_NIF "Introduza o NIF do Cliente"
#define ASK_CLIENT_ID "Introduza o ID do cliente"
#define ASK_FIND_TERM "Introduza termo de pesquisa"
#define ASK_ACCOUNT_ID "Introduza o número da conta"
#define ASK_FOR_VALUE "Introduza o valor em €"
#define ASK_FOR_DAYS "Insira o número de dias a incrementar (>0)"
// MACROS COM STRINGS do TIPO de Movimentos
#define DEPOSITO_STR "DEPOSITO"
#define LEVANTAMENTO_STR "LEVANTAMENTO"
#define TRANSFERENCIA_STR "TRANSFERÊNCIA"
#define JUROS_STR "JURO"

#define MAX_NAME 100        // Dimensão máxima de qualquer nome que se utilize
#define CODE_SIZE 4         // Código da conta (3 caracteres + \0)
#define MAX_N_MOVEMENTS 100 // Caso não se utilizem listas
#define MAX_N_ACCOUNTS 3    // Dimensão máxima de Contas
#define MAX_N_CLIENTS 30    // Caso não se utilizem listas
#define NIF_SIZE 10         // 9 caracteres + \0

/**
 *Estruturas do programa
 **/

// Assinaturas das Estruturas
typedef struct _banco Banco;

typedef struct _cliente Cliente;

typedef struct _conta Conta;

typedef struct _movimento Movimento;

typedef struct _lista_clientes LClientes;

typedef struct _no_lista_clientes NoLClientes;

typedef struct _lista_contas LContas;

typedef struct _no_lista_contas NoLContas;

typedef struct _lista_movimentos LMovimentos;

typedef struct _no_lista_movimentos NoLMovimentos;

typedef enum _tipoMovimento TipoMovimento;

// Estruturas
typedef struct _lista_movimentos
{
    NoLMovimentos *first;
    NoLMovimentos *last;
} LMovimentos;

typedef struct _no_lista_movimentos
{
    Movimento *m;             // Estrutura de Movimentos
    NoLMovimentos *next;
    NoLMovimentos *prev;
} NoLMovimentos;

typedef struct _lista_contas
{
    NoLContas *first;
    NoLContas *last;
} LContas;

typedef struct _no_lista_contas
{
    Conta *c;                 // Estrutura de Conta
    NoLContas *next;
    NoLContas *prev;
} NoLContas;

typedef struct _lista_clientes
{
    NoLClientes *first;
    NoLClientes *last;
} LClientes;

typedef struct _no_lista_clientes
{
    Cliente *c;               // Estrutura de Cliente
    NoLClientes *next;
    NoLClientes *prev;
} NoLClientes;

typedef struct _banco
{
    char nome[MAX_NAME];
    float valor_total_banco;  // Valor total de todas as contas de todos os clientes do Banco
    float juro_positivo;      // Valor do juro a considerar para bonificar as contas com saldo positivo
    float juro_negativo;      // Valor do juro a cobrar nas contas com saldo negativo
    int data;
    int next_id_cliente;      // Usado para incrementar cada vez que seja criado um novo cliente e atribuir este id
    int next_id_movimento;    // Usado para incrementar cada vez que seja realizado um novo movimento e atribuir este id
    int clientes_ativos;
    int contas_ativas;
    LMovimentos *movimentos_banco;
    LClientes *clientes;
} Banco;

typedef struct _cliente
{
    int id;
    char nome[MAX_NAME];
    char nif[NIF_SIZE];
    double saldo_integrado;   // Saldo acumulado de todas as contas do cliente
    int nmr_contas;           // Usado para verificar o número total de contas do cliente que não pode ser superior a 3
    LContas *contas;
} Cliente;

typedef struct _conta
{
    double saldo;
    char id_conta[CODE_SIZE];
    LMovimentos *movimentos;
} Conta;

typedef enum _tipoMovimento
{
    DEPOSITO,
    LEVANTAMENTO,
    TRASFERENCIA,
    JURO
} TipoMovimento;

typedef struct _movimento
{
    int id;                   // Id atribuido ao movimento
    int id_cliente;           // Id do cliente que efetuou o movimento
    char id_conta[CODE_SIZE]; // id da conta que efetuou o movimento
    int data;
    float valor;
    TipoMovimento tipo;       // Indica o tipo de movimento realizado: DEPOSITO, LEVANTAMENTO, TRANSFERENCIA, JURO
} Movimento;

/**
 * !###### Funções Visíveis no exterior da biblioteca######
 **/

/**
 * Incicia a estrutura Banco
 * @param nome do Banco
 * @return ponteiro para banco criado
 **/
Banco *iniciarBanco(char *nome);

/**
 * Mostra a informação detalhada do Banco
 * @param b
 **/
void mostrarInfoBanco(Banco *b);

/**
 * Mostra a informação detalhada do Banco, Clientes e Contas
 * @param b
 **/
void monstrarInfoCompleta(Banco *b);

/**
 * Lista todos os movimentos do Banco
 * @param b
 **/
void listarMovimentosBanco(Banco *b);

/**
 * Lista os 5 melhores clientes por ordem decrescente (com maior saldo integrado primeiro)
 * @param b
 **/
void listarMelhoresClientes(Banco *b);

/**
 * Avança a data atribuida pelo utilizador, criando juros nas contas ativas dos clientes.
 * @param b
 **/
void avancarData(Banco *b);

/**
 * Liberta todos os recursos associados ao Banco
 * @param b Banco a ser libertado
 **/
void free_banco(Banco *b);

/**
 * Incicia a estrutura Cliente
 * Solicita os dados ao utilizador do cliente e aloca-os na lista de clientes
 * @param b
 **/
void inserirCliente(Banco *b);

/**
 * Lista todos os clientes no banco
 * @param b
 **/
void listarClientes(Banco *b);

/**
 * Solicita um ID válido ao utilizador e lista todas as informações do ID
 * @param b
 **/
void mostrarCliente(Banco *b);

/**
 * Solicita um ID válido ao utilizador e liberta todos os recursos associados ao Cliente
 * @param b Cliente a ser libertado
 **/
void apagarCliente(Banco *b);

/**
 * Solicita um termo ao cliente e pesquisa por nomes de clientes com o termo introduzido
 * @param b
 **/
void procurarCliente(Banco *b);

/**
 * Incicia a estrutura Conta
 * Solicita um ID válido de cliente ao utilizador e os dados da conta a alocar na lista de contas
 * do cliente associado
 * @param b
 **/
void inserirConta(Banco *b);

/**
 * Solicita um ID válido de cliente ao utilizador e lista todas as contas associadas ao ID
 * @param b
 **/
void listarContas(Banco *b);

/**
 * Solicita um ID válido de cliente ao utilizador e o número da conta a apresentar
 * @param b
 **/
void mostrarContas(Banco *b);

/**
 * Solicita um ID válido de cliente ao utilizador e o número da conta a fechar. Liberta todos
 * os recursos associados à conta
 * @param b Conta a ser libertada
 **/
void fecharConta(Banco *b);

/**
 * Incicia a estrutura Movimentos do tipo DEPOSITO
 * Solicita um ID válido de cliente ao utilizador e o número da conta associado para depositar dinheiro
 * @param b
 **/
void depositarDinheiro(Banco *b);

/**
 * Incicia a estrutura Movimentos do tipo LEVANTAMENTO
 * Solicita um ID válido de cliente ao utilizador e o número da conta associado para levantar dinheiro
 **/
void levantarDinheiro(Banco *b);

/**
 * Incicia a estrutura Movimentos do tipo TRANSFERENCIA
 * Solicita um ID válido de cliente ao utilizador e o número da conta associado para transferir dinheiro
 * de, para um ID válido de cliente ao utilizador e o número da conta associado
 * @param b
 **/
void transferirDinheiro(Banco *b);

#endif