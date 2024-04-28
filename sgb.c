/**
 * Author: Marco Cândido, Miguel Barroso e Pedro Marques
 * Date: 14/04/2024
 * Description: Projeto de uma aplicação para um banco com funções simples como criação de clientes, contas e movimentos dos mesmos
 **/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ui.h"
#include "sgb.h"

#define TRUE 1
#define FALSE 0

//! ASSINATURAS FUNÇÕES AUXILIARES
int pesquisaMostraInfo(Banco *b, char *nome);

int verificarExisteClientes(Banco *b);

int verificarExisteContas(NoLClientes *aux);

int verificarSaldoCliente(NoLClientes *aux, double valor);

int verificarExisteMovimentosBanco(Banco *b);

int verificarExisteMovimentosConta(NoLContas *c);

NoLClientes *procurarNif(Banco *b, char *nif);

NoLClientes *getClientById(Banco *b, int id);

NoLContas *getAccountById(NoLClientes *aux, char *id_conta);

NoLClientes *getClientePositionName(LClientes *lcliente, Cliente *cliente);

NoLContas *getContaPositionName(LContas *lcontas, Conta *conta);

void inserirClienteLista(LClientes *lcliente, Cliente *cliente);

void inserirContaLista(LContas *lcontas, Conta *conta);

void inserirMovimentoLista(LMovimentos *lmov, Movimento *movimento);

void criarMovimento(Banco *b, NoLClientes *cliente, NoLContas *conta, double valor, int tipo, int validacao);

char *enumString(NoLMovimentos *movimentos, char *tipo);

void removeMovimentos(NoLContas *conta);

void removerConta(Banco *b, NoLClientes *cliente, NoLContas *conta);

void removerCliente(Banco *b, NoLClientes *cliente);

void removerContasLista(Banco *b, NoLClientes *cliente);

void removerClientesLista(Banco *b);

double calcularAtualizacao(double valor, double taxa, int dias);

//! FUNÇÕES AUXILIARES

/**
 * A função percorre a lista dos clientes do banco.
 * A função "strstr" encrontra o cliente que mais corresponde em comparação com o termo.
 * Caso nao haja clientes encontrados, é aprensado um erro.
 * Se for encontrado é demonstrado as informações do cliente.
 * @param banco @param termo
 * @return TRUE ou FALSE
 */
int pesquisaMostraInfo(Banco *b, char *termo)
{
    NoLClientes *cliente = b->clientes->first;
    int clientes_encontrados = 0;

    while (cliente)
    {
        if (strstr(cliente->c->nome, termo))
        {
            LIST_USERS_ITEM(cliente->c->id, cliente->c->nif, cliente->c->nome, cliente->c->nmr_contas, cliente->c->saldo_integrado);
            clientes_encontrados++;
        }
        if (cliente == b->clientes->last && clientes_encontrados == 0)
        {
            NO_CLIENTS_FOUND_ERROR(termo);
            return TRUE;
        }
        cliente = cliente->next;
    }
    CLIENTS_LOOKUP_FOOTER(clientes_encontrados, termo);
    return FALSE;
}

/**
 * A função verifica se existe clientes no banco.
 * A função retorna uma mensagem de erro se o valor for NULL.
 * @param banco
 * @return TRUE ou FALSE
 */
int verificarExisteClientes(Banco *b)
{
    NoLClientes *cliente = b->clientes->first;
    if (cliente == NULL)
    {
        NO_CLIENTS_ERROR;
        return TRUE;
    }
    return FALSE;
}

/**
 * A função verifica se existem contas nos clientes.
 * A função retorna uma mensagem de erro se o cliente não tiver contas.
 * @param banco
 * @return TRUE ou FALSE
 */
int verificarExisteContas(NoLClientes *cliente)
{
    NoLContas *conta = cliente->c->contas->first;
    if (conta == NULL)
    {
        CLIENT_DONT_HAVE_ACCOUNTS(cliente->c->id);
        return TRUE;
    }
    return FALSE;
}

/**
 * A função faz a comparação entre o valor e o saldo integrado do cliente.
 * Se o valor for menor que o saldo integrado do cliente aprensenta o erro.
 * @param cliente @param valor
 * @return TRUE ou FALSE
 */
int verificarSaldoCliente(NoLClientes *cliente, double valor)
{
    if (cliente->c->saldo_integrado < valor)
    {
        WITHDRAWAL_ERROR_NO_FUNDS(cliente->c->id, cliente->c->saldo_integrado);
        return TRUE;
    }
    return FALSE;
}

/**
 * A função verifica se existe movimentos no banco.
 * Retorna uma mensagem de erro caso os movimentos forem igual a NULL.
 * @param banco
 * @return TRUE ou FALSE
 */
int verificarExisteMovimentosBanco(Banco *b)
{
    if (b->movimentos_banco->first == NULL)
    {
        NO_MOVEMENTS_ERROR;
        return TRUE;
    }
    return FALSE;
}
/**
 * A função vai verificar se existem moviemntos na conta.
 * @param conta
 * @return TRUE ou FALSE
 */
int verificarExisteMovimentosConta(NoLContas *conta)
{
    if (conta->c->movimentos->first == NULL)
        return FALSE;
    return TRUE;
}

/**
 * A função percorre a lista de clientes no banco.
 * A função vai verificar se existe algum nif no banco correspondente.
 * @return cliente -> se o nif for igual
 * @param banco @param nif
 */
NoLClientes *procurarNif(Banco *b, char *nif)
{
    NoLClientes *cliente = b->clientes->first;
    while (cliente)
    {
        if (strcmp(nif, cliente->c->nif) == 0)
        {
            INSERT_CLIENT_ERROR_ALREADY_EXISTS(nif);
            return cliente;
        }
        if (cliente->next == NULL)
            break;
        cliente = cliente->next;
    }
    return NULL;
}

/**
 * A função percorre a lista de clientes do banco.
 * Se o id for diferente de algum id de cliente apresenta uma mensagem de erro.
 * @param banco @param id
 * @return cliente -> se o id for igual
 */
NoLClientes *getClientById(Banco *b, int id)
{
    NoLClientes *cliente = b->clientes->first;
    while (cliente)
    {
        if (id == cliente->c->id)
            return cliente;
        if (cliente == b->clientes->last)
        {
            NO_SUCH_CLIENT_ERROR(id);
            return NULL;
        }
        cliente = cliente->next;
    }
    return NULL;
}

/**
 * A função percorre a lista de contas no cliente.
 * A função vai verificar se existe alguma conta com o id de conta.
 * @param cliente @param id_conta
 * @return conta -> se o id for igual
 */
NoLContas *getAccountById(NoLClientes *cliente, char *id_conta)
{
    NoLContas *conta = cliente->c->contas->first;

    while (conta)
    {
        if (strcmp(id_conta, conta->c->id_conta) == 0)
            return conta;

        if (conta == cliente->c->contas->last)
        {
            NO_SUCH_ACCOUNT_ERROR(cliente->c->id, id_conta);
            return NULL;
        }
        conta = conta->next;
    }
    return NULL;
}

/**
 * A função percorre a lista de clientes.
 * @param lcliente @param cliente
 * @return NoLClientes -> atraves da função "strcmp"
 */
NoLClientes *getClientePositionName(LClientes *lcliente, Cliente *cliente)
{
    NoLClientes *noLCliente = lcliente->first;
    while (noLCliente)
    {
        if (strcmp(noLCliente->c->nome, cliente->nome) > 0)
            return noLCliente;
        noLCliente = noLCliente->next;
    }
    return NULL;
}

/**
 * A função percorre a lista de contas lcontas.
 * Compara o id da conta atual da lcontas com o id da conta, se for maior retorna o nó que precede a posição.
 * @param lcontas, @param conta
 * @return NoLContas -> atraves da função "strcmp"
 */
NoLContas *getContaPositionName(LContas *lcontas, Conta *conta)
{
    NoLContas *noLContas = lcontas->first;
    while (noLContas)
    {
        if (strcmp(noLContas->c->id_conta, conta->id_conta) > 0)
            return noLContas;
        noLContas = noLContas->next;
    }
    return NULL;
}

/**
 * A função cria um no de lista e insere-o dentro da lista.
 * @param lcliente @param cliente
 */
void inserirClienteLista(LClientes *lcliente, Cliente *cliente)
{
    NoLClientes *noLCliente1 = malloc(sizeof(NoLClientes));
    noLCliente1->c = cliente;
    // Se for o primeiro no da lista
    if (lcliente->first == NULL)
    {
        lcliente->first = noLCliente1;
        lcliente->last = noLCliente1;
        noLCliente1->prev = NULL;
        noLCliente1->next = NULL;
        return;
    }

    NoLClientes *noLCliente2 = getClientePositionName(lcliente, cliente);

    // inserir no inicio da lista
    if (noLCliente2 == lcliente->first)
    {
        noLCliente1->prev = NULL;
        lcliente->first->prev = noLCliente1;
        noLCliente1->next = lcliente->first;
        lcliente->first = noLCliente1;
        return;
    }

    // Inserir no fim da lista
    if (noLCliente2 == NULL)
    {
        lcliente->last->next = noLCliente1;
        noLCliente1->prev = lcliente->last;
        noLCliente1->next = NULL;
        lcliente->last = noLCliente1;
        return;
    }

    // Se cheguei aqui
    // Caso geral. algures no meio.
    noLCliente1->next = noLCliente2;
    noLCliente1->prev = noLCliente2->prev;
    noLCliente2->prev->next = noLCliente1;
    noLCliente2->prev = noLCliente1;
    return;
}

/**
 * A função cria um no de lista e insere-o dentro da lista.
 * @param lcontas @param conta
 */
void inserirContaLista(LContas *lcontas, Conta *conta)
{
    NoLContas *noLContas1 = malloc(sizeof(NoLContas));
    noLContas1->c = conta;
    // Se for o primeiro no da lista
    if (lcontas->first == NULL)
    {
        lcontas->first = noLContas1;
        lcontas->last = noLContas1;
        noLContas1->prev = NULL;
        noLContas1->next = NULL;
        return;
    }

    NoLContas *noLContas2 = getContaPositionName(lcontas, conta);

    // inserir no inicio da lista
    if (noLContas2 == lcontas->first)
    {
        noLContas1->prev = NULL;
        lcontas->first->prev = noLContas1;
        noLContas1->next = lcontas->first;
        lcontas->first = noLContas1;
        return;
    }

    // Inserir no fim da lista
    if (noLContas2 == NULL)
    {
        lcontas->last->next = noLContas1;
        noLContas1->prev = lcontas->last;
        noLContas1->next = NULL;
        lcontas->last = noLContas1;
        return;
    }

    // Se cheguei aqui
    // Caso geral. algures no meio.
    noLContas1->next = noLContas2;
    noLContas1->prev = noLContas2->prev;
    noLContas2->prev->next = noLContas1;
    noLContas2->prev = noLContas1;
    return;
}

/**
 * A função cria um no de lista movimentos e insere-o dentro da lista.
 * @param lmov @param movimento
 */
void inserirMovimentoLista(LMovimentos *lmov, Movimento *movimento)
{
    NoLMovimentos *noLMovimentos = malloc(sizeof(NoLMovimentos));
    noLMovimentos->m = movimento;
    // Se for o primeiro no da lista
    if (lmov->first == NULL)
    {
        lmov->first = noLMovimentos;
        lmov->last = noLMovimentos;
        noLMovimentos->prev = NULL;
        noLMovimentos->next = NULL;
        return;
    }
    lmov->last->next = noLMovimentos;
    noLMovimentos->prev = lmov->last;
    noLMovimentos->next = NULL;
    lmov->last = noLMovimentos;
    return;
}

/**
 * A função cria uma estrutura movimento e usa os parametros para os seus respetivos atributos.
 * A função tambem atualiza a informaçoes de saldo do b, e do cliente.
 * Insere-o dentro da lista.
 * @param b @param cliente @param conta @param valor @param tipo @param validacao
 */
void criarMovimento(Banco *b, NoLClientes *cliente, NoLContas *conta, double valor, int tipo, int validacao)
{
    Movimento *mov = malloc(sizeof(Movimento));
    mov->id = b->next_id_movimento;
    b->next_id_movimento++;
    mov->id_cliente = cliente->c->id;
    mov->data = b->data;
    mov->tipo = tipo;
    strcpy(mov->id_conta, conta->c->id_conta);

    inserirMovimentoLista(conta->c->movimentos, mov);
    inserirMovimentoLista(b->movimentos_banco, mov);

    if (validacao == DEPOSITO)
    {
        cliente->c->saldo_integrado += valor;
        conta->c->saldo += valor;
        b->valor_total_banco += valor;
        mov->valor = valor;
    }
    if (validacao == LEVANTAMENTO)
    {
        cliente->c->saldo_integrado -= valor;
        conta->c->saldo -= valor;
        b->valor_total_banco -= valor;
        mov->valor = valor * -1;
    }
}

/**
 * A função passa o valor do tipo para string.
 * @param movimentos @param tipo
 * @return tipo em string.
 */
char *enumString(NoLMovimentos *movimentos, char *tipo)
{
    switch (movimentos->m->tipo)
    {
    case 0:
        strcpy(tipo, DEPOSITO_STR);
        break;

    case 1:
        strcpy(tipo, LEVANTAMENTO_STR);
        break;

    case 2:
        strcpy(tipo, TRANSFERENCIA_STR);
        break;

    case 3:
        strcpy(tipo, JUROS_STR);
        break;

    default:
        break;
    }
    return tipo;
}

/**
 * A função apaga todos movimentos da conta
 * @param conta
 */
void removeMovimentos(NoLContas *conta)
{
    NoLMovimentos *movimentos = conta->c->movimentos->last;

    if (movimentos == NULL)
        return;
    while (movimentos)
    {
        NoLMovimentos *movimentosAux = movimentos->prev;

        free(movimentos->m);
        free(movimentos);

        movimentos = movimentosAux;

        if (movimentos == NULL)
            return;

        movimentosAux = movimentosAux->prev;
    }
}

/**
 * A função remove a conta da lista de contas do cliente.
 * @param banco @param cliente @param conta
 */
void removerConta(Banco *b, NoLClientes *cliente, NoLContas *conta)
{
    while (conta)
    {
        b->valor_total_banco -= conta->c->saldo;
        cliente->c->saldo_integrado -= conta->c->saldo;
        b->contas_ativas--;
        cliente->c->nmr_contas--;

        if (conta == cliente->c->contas->first && conta == cliente->c->contas->last)
        {
            cliente->c->contas->first = NULL;
            cliente->c->contas->last = NULL;
            return;
        }

        if (conta == cliente->c->contas->first)
        {
            cliente->c->contas->first = conta->next;
            conta->next->prev = NULL;
            return;
        }

        if (conta == cliente->c->contas->last)
        {
            cliente->c->contas->last = conta->prev;
            conta->prev->next = NULL;
            return;
        }

        if (conta != cliente->c->contas->first && conta != cliente->c->contas->last)
        {
            conta->next->prev = conta->prev;
            conta->prev->next = conta->next;
            return;
        }
    }
}

/**
 * A função remove o cliente da lista de clientes do b.
 * A funçao dimiui por 1 a var clientes_ativos
 * @param b @param cliente
 */
void removerCliente(Banco *b, NoLClientes *cliente)
{
    while (cliente)
    {
        b->clientes_ativos--;

        if (cliente == b->clientes->first && cliente == b->clientes->last)
        {
            b->clientes->first = NULL;
            b->clientes->last = NULL;
            return;
        }

        if (cliente == b->clientes->first)
        {
            b->clientes->first = cliente->next;
            cliente->next->prev = NULL;
            return;
        }

        if (cliente == b->clientes->last)
        {
            b->clientes->last = cliente->prev;
            cliente->prev->next = NULL;
            return;
        }

        if (cliente != b->clientes->first && cliente != b->clientes->last)
        {
            cliente->next->prev = cliente->prev;
            cliente->prev->next = cliente->next;
            return;
        }
    }
}

/**
 * A função remove as contas todas do cliente, subtraindo ou adicionando o valor delas no banco.
 * @param banco @param cliente
 */
void removerContasLista(Banco *b, NoLClientes *cliente)
{
    NoLContas *conta = cliente->c->contas->last;

    if (conta == NULL)
        return;

    while (conta)
    {
        NoLContas *contaAux = conta->prev;

        b->valor_total_banco -= conta->c->saldo;
        b->contas_ativas--;

        free(conta->c->movimentos);
        free(conta->c);
        free(conta);

        conta = contaAux;

        if (conta == NULL)
            return;

        contaAux = contaAux->prev;
    }
}

/**
 * A função os nos das lista de clientes do b
 * @param b
 */
void removerClientesLista(Banco *b)
{
    NoLClientes *cliente = b->clientes->last;

    if (cliente == NULL)
        return;

    while (cliente)
    {
        NoLClientes *clienteAux = cliente->prev;

        free(cliente->c->contas);
        free(cliente->c);
        free(cliente);

        cliente = clienteAux;

        if (cliente == NULL)
            return;

        clienteAux = clienteAux->prev;
    }
}

/**
 * A função recebe um dia e verifica se o valor é positivo ou negativo para aplicar juros negativos ou positivos.
 * @param valor @param taxa @param dias
 * @return valor da conta com os respetivos juros aplicados.
 */
double calcularAtualizacao(double valor, double taxa, int dias)
{
    if (valor > 0.0)
        return valor * (pow(1 + taxa, (double)dias)) - valor;

    return -1.0 * (valor * (pow(1 + taxa, (double)dias)) - valor);
}

//! FUNÇÕES PÚBLICAS
//! BANCO
Banco *iniciarBanco(char *nome)
{
    Banco *b;
    b = malloc(sizeof(Banco)); // cria um espaço em memoria do tamanho banco

    memset(b->nome, '\0', MAX_NAME); // mete todos os caracteres da variavel b->nome igual a \0
    strcpy(b->nome, nome);           // copia a variavel
    b->valor_total_banco = 0;
    b->juro_positivo = 0.015;
    b->juro_negativo = 0.05;
    b->data = 0;
    b->next_id_cliente = 1;
    b->clientes_ativos = 0;
    b->contas_ativas = 0;
    b->next_id_movimento = 1;

    b->clientes = malloc(sizeof(LClientes));           // cria um espaço em memoria do tamanho LClientes))
    b->movimentos_banco = malloc(sizeof(LMovimentos)); // cria um espaço em memoria do tamanho LMovimentos))

    return b;
}

void mostrarInfoBanco(Banco *b)
{
    SHOW_BANK_INFO(b->data, b->nome, b->clientes_ativos, b->contas_ativas, b->valor_total_banco); // Mostra a informação do banco.
}

void monstrarInfoCompleta(Banco *b)
{
    NoLClientes *cliente = b->clientes->first; // Define um ponteiro para o primeiro cliente na lista de clientes do banco

    SHOW_BANK_INFO(b->data, b->nome, b->clientes_ativos, b->contas_ativas, b->valor_total_banco); // Mostra informações gerais sobre o banco

    while (cliente) // Loop para percorrer todos os clientes
    {
        if (cliente == NULL)
            return;

        NoLContas *conta = cliente->c->contas->first; // Define um ponteiro para a primeira conta do cliente

        CLIENT_INFO(b->data, cliente->c->id, cliente->c->nome, cliente->c->nif, cliente->c->nmr_contas, cliente->c->saldo_integrado); // Mostra informações sobre o cliente

        while (conta) // Loop para percorrer todas as contas do cliente
        {
            if (conta == NULL) // Condição redundante, já que o loop é controlado pela condição 'conta', que é testada na declaração do loop
                return;

            NoLMovimentos *movimento = conta->c->movimentos->last; // Define um ponteiro para o último movimento da conta

            CLIENT_INFO_ACCOUNTS(conta->c->id_conta, conta->c->saldo); // Mostra informações sobre a conta

            for (int i = 0; i < 5; i++) // Loop para mostrar os últimos 5 movimentos da conta
            {
                if (movimento == NULL) // Se não houver mais movimentos, sai do loop
                    break;

                char tipo[MAX_NAME];
                memset(tipo, '\0', MAX_NAME);
                enumString(movimento, tipo); // Converte o tipo de movimento em uma string legível

                CLIENT_INFO_MOVEMENTS(movimento->m->id, tipo, movimento->m->valor); // Mostra informações sobre o movimento

                if (movimento == conta->c->movimentos->first) // Se chegarmos ao primeiro movimento, saímos do loop
                    break;

                movimento = movimento->prev; // Move para o movimento anterior na lista de movimentos
            }
            conta = conta->next; // Move para a próxima conta do cliente
        }
        cliente = cliente->next; // Move para o próximo cliente na lista de clientes do banco
    }
}

void listarMovimentosBanco(Banco *b)
{
    NoLMovimentos *movimento = b->movimentos_banco->last; // Inicializa um ponteiro para o último movimento na lista de movimentos do banco

    if (verificarExisteMovimentosBanco(b)) // Verifica se não há movimentos no banco
        return;

    LIST_BANK_MOVEMENTS_HEADER(b->next_id_movimento - 1); // Imprime o cabeçalho da lista de movimentos, utilizando o próximo ID de movimento menos 1

    while (movimento) // Percorre sobre cada movimento na lista
    {
        if (movimento == NULL) // Verifica se o ponteiro de movimento é NULL
            return;

        char tipo[MAX_NAME];          // Declara um array de caracteres 'tipo' com o tamanho MAX_NAME
        memset(tipo, '\0', MAX_NAME); // Preenche o array com caracteres nulos

        enumString(movimento, tipo); // Converte o tipo de movimento para uma string legível

        // Imprime os detalhes do movimento atual utilizando uma macro específica
        LIST_BANK_MOVEMENTS_ITEM(movimento->m->id, movimento->m->data, movimento->m->id_cliente, movimento->m->id_conta, tipo, movimento->m->valor);

        movimento = movimento->prev; // Move para o movimento anterior na lista
    }
}

void avancarData(Banco *b)
{
    double juros = 0;
    int data = pedirInteiro(ASK_FOR_DAYS); // Solicita ao utilizador um número de dias para avançar a data

    if (data <= 0) // Verifica se o número de dias é válido
    {
        NUMBER_OF_DAYS_INCORRECT(data); // Mostra uma mensagem de erro se o número de dias for inválido
        return;
    }

    b->data += data; // Avança a data do banco pelo número de dias especificado

    NoLClientes *cliente = b->clientes->first;
    int tipo = JURO;
    int validacao; // validação do tipo de movimento que vai ser realizado na conta do cliente para transmitir à função "criarMovimento", que vai subtrair ou adicionar dependendo do tipo
    double valor;

    while (cliente) // Loop para percorrer todos os clientes
    {
        if (cliente == NULL) // Condição redundante, já que o loop é controlado pela condição 'cliente', que é testada na declaração do loop
            break;

        NoLContas *conta = cliente->c->contas->first;

        while (conta) // Loop para percorrer todas as contas do cliente
        {
            if (conta == NULL) // Condição redundante, já que o loop é controlado pela condição 'conta', que é testada na declaração do loop
                break;

            if (conta->c->saldo > 0) // Verifica se o saldo da conta é positivo
            {
                validacao = DEPOSITO;                                                 // Define a validação como DEPOSITO
                valor = calcularAtualizacao(conta->c->saldo, b->juro_positivo, data); // Calcula o valor do juro para o saldo positivo
                juros += valor;                                                       // Adiciona o valor do juro ao total de juros
                criarMovimento(b, cliente, conta, valor, tipo, validacao);            // Cria um movimento para o depósito
            }
            if (conta->c->saldo < 0) // Verifica se o saldo da conta é negativo
            {
                validacao = LEVANTAMENTO;                                             // Define a validação como LEVANTAMENTO
                valor = calcularAtualizacao(conta->c->saldo, b->juro_negativo, data); // Calcula o valor do juro para o saldo negativo
                juros -= valor;                                                       // Subtrai o valor do juro do total de juros
                criarMovimento(b, cliente, conta, valor, tipo, validacao);            // Cria um movimento para o levantamento
            }

            conta = conta->next; // Move para a próxima conta do cliente
        }

        cliente = cliente->next; // Move para o próximo cliente na lista de clientes do banco
    }

    NEW_DATE_OK(b->data, juros); // Mostra uma mensagem indicando que a nova data foi definida e os juros foram calculado
}

void listarMelhoresClientes(Banco *b)
{
    if (verificarExisteClientes(b)) // Verifica se existem clientes no banco
        return;

    Cliente *best_clients[5] = {NULL}; // Cria um array para armazenar os 5 melhores clientes

    NoLClientes *cliente = b->clientes->first; // Define um ponteiro para o primeiro cliente na lista de clientes do banco

    LIST_BEST_CLIENTS_HEADER; // Mostra o cabeçalho da lista de melhores clientes

    while (cliente) // Loop para percorrer todos os clientes
    {
        for (int i = 0; i < 5; i++) // Loop para encontrar os 5 melhores clientes
        {
            if (best_clients[i] == NULL || cliente->c->saldo_integrado > best_clients[i]->saldo_integrado || (cliente->c->saldo_integrado == best_clients[i]->saldo_integrado && cliente->c->id < best_clients[i]->id))
            {
                for (int j = 4; j > i; j--) // Move os clientes para abrir espaço para o novo melhor cliente
                {
                    best_clients[j] = best_clients[j - 1];
                }
                best_clients[i] = cliente->c; // Armazena o cliente como um dos melhores
                break;
            }
        }
        cliente = cliente->next; // Move para o próximo cliente na lista de clientes do banco
    }

    for (int i = 0; i < 5; i++) // Loop para mostrar os 5 melhores clientes
    {
        if (best_clients[i] == NULL) // Verifica se não há mais clientes a serem listados
            return;

        LIST_BEST_CLIENTS_ITEM(best_clients[i]->id, best_clients[i]->nif, best_clients[i]->nome, best_clients[i]->saldo_integrado); // Mostra informações sobre o cliente
    }
}

void free_banco(Banco *b)
{
    if (b->next_id_cliente > 1) // Verifica se existem clientes no banco
    {
        NoLClientes *cliente = b->clientes->last; // Define um ponteiro para o último cliente na lista de clientes do banco

        while (cliente) // Loop para percorrer todos os clientes
        {
            if (cliente == NULL) // Condição redundante, já que o loop é controlado pela condição 'cliente', que é testada na declaração do loop
                break;

            if (cliente->c->nmr_contas > 0) // Verifica se o cliente possui contas
            {
                NoLContas *conta = cliente->c->contas->last; // Define um ponteiro para a última conta do cliente

                while (conta) // Loop para percorrer todas as contas do cliente
                {
                    if (conta == NULL) // Condição redundante, já que o loop é controlado pela condição 'conta', que é testada na declaração do loop
                        break;

                    if (verificarExisteMovimentosConta(conta)) // Verifica se existem movimentos associados à conta
                        removeMovimentos(conta);               // Remove todos os movimentos associados à conta

                    conta = conta->prev; // Move para a conta anterior na lista de contas do cliente
                }
                removerContasLista(b, cliente); // Remove todas as contas do cliente da lista de contas do banco
            }
            cliente = cliente->prev; // Move para o cliente anterior na lista de clientes do banco
        }

        if (b->clientes->last != NULL) // Verifica se ainda existem clientes na lista de clientes do banco
            removerClientesLista(b);   // Remove todos os clientes da lista de clientes do banco
    }

    if (b->next_id_movimento > 1) // Verifica se existem movimentos no banco
    {
        NoLMovimentos *movimentos = b->movimentos_banco->last; // Define um ponteiro para o último movimento na lista de movimentos do banco

        while (movimentos) // Loop para percorrer todos os movimentos
        {
            NoLMovimentos *movimentosAux = movimentos->prev; // Salva uma referência ao movimento anterior antes de liberar a memória do movimento atual

            free(movimentos); // Libera a memória alocada para o movimento atual

            movimentos = movimentosAux; // Move para o movimento anterior na lista de movimentos do banco

            if (movimentos == NULL) // Verifica se chegamos ao fim da lista de movimentos
                break;

            movimentosAux = movimentosAux->prev; // Move para o movimento anterior na lista de movimentos do banco
        }
    }

    free(b->movimentos_banco); // Libera a memória alocada para a lista de movimentos do banco
    free(b->clientes);         // Libera a memória alocada para a lista de clientes do banco
    free(b);                   // Libera a memória alocada para a estrutura do banco
}

//! CLIENTES
void inserirCliente(Banco *b)
{
    Cliente *cliente = NULL;
    char nif[NIF_SIZE];
    char nome_prov[MAX_NAME];
    memset(nif, '\0', NIF_SIZE);       // Inicializa o array de NIF com '\0'
    memset(nome_prov, '\0', MAX_NAME); // Inicializa o array de nome temporário com '\0'

    pedirString(nome_prov, ASK_NAME, MAX_NAME); // Solicita o nome do cliente ao usuário
    pedirString(nif, ASK_NIF, NIF_SIZE);        // Solicita o NIF do cliente ao usuário
    if (procurarNif(b, nif))                    // Verifica se o NIF já existe no banco
        return;                                 // Retorna se o NIF já existe

    cliente = malloc(sizeof(Cliente));         // Aloca memória para a estrutura do cliente
    cliente->contas = malloc(sizeof(LContas)); // Aloca memória para a lista de contas do cliente

    strcpy(cliente->nome, nome_prov); // Copia o nome temporário para o nome do cliente
    strcpy(cliente->nif, nif);        // Copia o NIF para o NIF do cliente

    cliente->id = b->next_id_cliente; // Atribui o próximo ID de cliente ao novo cliente
    b->next_id_cliente += 1;          // Atualiza o próximo ID de cliente no banco

    inserirClienteLista(b->clientes, cliente);        // Insere o cliente na lista de clientes do banco
    INSERT_CLIENT_SUCESS(cliente->id, cliente->nome); // Mostra uma mensagem indicando que o cliente foi inserido com sucesso
    b->clientes_ativos++;                             // Incrementa o número de clientes ativos no banco
}

void listarClientes(Banco *b)
{
    NoLClientes *cliente = b->clientes->first;
    if (verificarExisteClientes(b)) // Verifica se existem clientes no banco
        return;
    LIST_USERS_HEADER(b->clientes_ativos);
    while (cliente) // Loop para percorrer todos os clientes
    {
        LIST_USERS_ITEM(cliente->c->id, cliente->c->nif, cliente->c->nome, cliente->c->nmr_contas, cliente->c->saldo_integrado);
        cliente = cliente->next; // Move para o próximo cliente na lista de clientes do banco
    }
}

void mostrarCliente(Banco *b)
{
    if (verificarExisteClientes(b)) // Verifica se existem clientes no banco
        return;

    NoLClientes *cliente = b->clientes->first; // Define um ponteiro para o primeiro cliente na lista de clientes do banco

    int id = pedirInteiro(ASK_CLIENT_ID); // Solicita ao usuário o ID do cliente a ser mostrado
    cliente = getClientById(b, id);       // Obtém o cliente com o ID especificado

    if (cliente == NULL) // Verifica se o cliente não foi encontrado
        return;

    CLIENT_INFO(b->data, cliente->c->id, cliente->c->nome, cliente->c->nif, cliente->c->nmr_contas, cliente->c->saldo_integrado); // Mostra informações básicas sobre o cliente

    NoLContas *contas = cliente->c->contas->first; // Define um ponteiro para a primeira conta do cliente

    if (cliente->c->nmr_contas > 0) // Verifica se o cliente possui contas
    {
        while (contas) // Loop para percorrer todas as contas do cliente
        {
            NoLMovimentos *movimentos = contas->c->movimentos->first; // Define um ponteiro para o primeiro movimento da conta

            if (contas == NULL) // Verifica se a conta é nula (condição redundante)
                return;

            CLIENT_INFO_ACCOUNTS(contas->c->id_conta, contas->c->saldo); // Mostra informações sobre a conta

            while (movimentos) // Loop para percorrer todos os movimentos da conta
            {
                if (movimentos == NULL) // Verifica se o movimento é nulo
                    break;

                char tipo[MAX_NAME];          // Declara um array de caracteres para armazenar o tipo do movimento
                memset(tipo, '\0', MAX_NAME); // Inicializa o array com '\0'
                enumString(movimentos, tipo); // Obtém a representação do tipo do movimento como uma string legível

                CLIENT_INFO_MOVEMENTS(movimentos->m->id, tipo, movimentos->m->valor); // Mostra informações sobre o movimento

                movimentos = movimentos->next; // Move para o próximo movimento na lista de movimentos
            }

            contas = contas->next; // Move para a próxima conta do cliente
        }
    }
}

void apagarCliente(Banco *b)
{
    NoLClientes *cliente = b->clientes->first;
    if (verificarExisteClientes(b)) // Verifica se não há clientes no banco
        return;

    int id = pedirInteiro(ASK_CLIENT_ID);
    cliente = getClientById(b, id); // Obtém o cliente pelo ID fornecido
    if (cliente == NULL)            // Retorna se o cliente não foi encontrado
        return;

    if (cliente->c->nmr_contas > 0) // Verifica se o cliente possui contas associadas
    {
        NoLContas *conta = cliente->c->contas->last;
        while (conta) // Loop para percorrer todas as contas do cliente
        {
            if (verificarExisteMovimentosConta(conta)) // Verifica se existem movimentos na conta
                removeMovimentos(conta);               // Remove os movimentos associados à conta

            conta = conta->prev;

            if (conta == NULL) // Sai do loop se chegar ao inicio da lista
                break;
        }
        removerContasLista(b, cliente); // Remove todas as contas associadas ao cliente da lista de contas do banco
    }

    removerCliente(b, cliente); // Remove o cliente da lista de clientes do banco
    free(cliente->c->contas);   // Libera a memória alocada para as contas associadas ao cliente
    free(cliente->c);           // Libera a memória alocada para o cliente
    free(cliente);              // Libera a memória alocada para o nó da lista de clientes
    CLIENT_DELETE_OK(id);
}

void procurarCliente(Banco *b)
{
    char termo[MAX_NAME];
    memset(termo, '\0', MAX_NAME); // Coloca todas as posições do array "id_conta" com '\0', o que permite ao programa parar a leitura antes de chegar ao '\0' no final do array sempre que o id introduzindo não tenha a dimensão total do mesmo.

    if (verificarExisteClientes(b)) // Verifica se não há clientes no banco
        return;

    pedirString(termo, ASK_FIND_TERM, MAX_NAME);
    if (pesquisaMostraInfo(b, termo)) // Realiza a busca e mostra informações do cliente se encontrado
        return;
}

//! CONTAS
void inserirConta(Banco *b)
{
    NoLClientes *cliente = b->clientes->first;
    Conta *conta = NULL;
    char id_prov[CODE_SIZE];
    memset(id_prov, '\0', CODE_SIZE); // Coloca todas as posições do array "id_conta" com '\0', o que permite ao programa parar a leitura antes de chegar ao '\0' no final do array sempre que o id introduzindo não tenha a dimensão total do mesmo.

    if (verificarExisteClientes(b)) // Verifica se existem clientes no banco
        return;

    int id = pedirInteiro(ASK_CLIENT_ID);
    cliente = getClientById(b, id); // Obtém o cliente com o ID especificado

    if (cliente == NULL) // Verifica se o cliente não foi encontrado
        return;

    if (cliente->c->nmr_contas == MAX_N_ACCOUNTS) // Verifica se o cliente atingiu o limite máximo de contas
    {
        NEW_ACCOUNT_ERROR_CLIENT_OVRELOADED(id); // Mostra uma mensagem de erro indicando que o cliente atingiu o limite máximo de contas
        return;
    }

    conta = malloc(sizeof(Conta));                   // Aloca memória para a estrutura da conta
    conta->movimentos = malloc(sizeof(LMovimentos)); // Aloca memória para a lista de movimentos da conta

    pedirString(id_prov, ASK_ACCOUNT_ID, CODE_SIZE);
    strcpy(conta->id_conta, id_prov); // Copia o ID da conta temporária para a conta

    inserirContaLista(cliente->c->contas, conta); // Insere a conta na lista de contas do cliente

    ACCOUNT_CREATION_SUCESS(conta->id_conta, cliente->c->id);

    cliente->c->nmr_contas++; // Incrementa o número de contas do cliente
    b->contas_ativas++;       // Incrementa o número de contas ativas no banco
}

void listarContas(Banco *b)
{
    NoLClientes *cliente = b->clientes->first;

    if (verificarExisteClientes(b)) // Verifica se existem clientes no banco
        return;

    int id = pedirInteiro(ASK_CLIENT_ID);
    cliente = getClientById(b, id); // Obtém o cliente com o ID especificado

    if (cliente == NULL) // Verifica se o cliente não foi encontrado
        return;

    if (verificarExisteContas(cliente)) // Verifica se o cliente possui contas
        return;

    NoLContas *contas = cliente->c->contas->first;

    LIST_ACCOUNTS_HEADER(cliente->c->id, cliente->c->nome, cliente->c->saldo_integrado);

    while (contas) // Loop para percorrer todas as contas do cliente
    {
        CLIENT_INFO_ACCOUNTS(contas->c->id_conta, contas->c->saldo);
        contas = contas->next; // Move para a próxima conta do cliente
    }
}

void mostrarContas(Banco *b)
{
    NoLClientes *cliente = b->clientes->first;
    char idconta[CODE_SIZE];
    memset(idconta, '\0', CODE_SIZE); // Coloca todas as posições do array "id_conta" com '\0', o que permite ao programa parar a leitura antes de chegar ao '\0' no final do array sempre que o id introduzindo não tenha a dimensão total do mesmo.

    if (verificarExisteClientes(b)) // Verifica se existem clientes no banco
        return;

    NoLContas *conta = cliente->c->contas->first;

    int id = pedirInteiro(ASK_CLIENT_ID); // Solicita ao usuário o ID do cliente
    cliente = getClientById(b, id);       // Obtém o cliente com o ID especificado

    if (cliente == NULL) // Verifica se o cliente não foi encontrado
        return;

    if (verificarExisteContas(cliente)) // Verifica se o cliente possui contas
        return;

    pedirString(idconta, ASK_ACCOUNT_ID, CODE_SIZE);

    conta = getAccountById(cliente, idconta); // Obtém a conta com o ID especificado pertencente ao cliente

    if (conta == NULL) // Verifica se a conta não foi encontrada
        return;

    LIST_ACCOUNTS_HEADER(cliente->c->id, cliente->c->nome, cliente->c->saldo_integrado);
    CLIENT_INFO_ACCOUNTS(conta->c->id_conta, conta->c->saldo);

    NoLMovimentos *movimentos = conta->c->movimentos->last;

    while (movimentos) // Loop para percorrer todos os movimentos da conta
    {
        if (movimentos == NULL) // Verifica se o movimento é nulo
            return;

        char tipo[MAX_NAME];
        memset(tipo, '\0', MAX_NAME); // Coloca todas as posições do array "id_conta" com '\0', o que permite ao programa parar a leitura antes de chegar ao '\0' no final do array sempre que o id introduzindo não tenha a dimensão total do mesmo.
        enumString(movimentos, tipo); // Obtém a representação do tipo do movimento como uma string legível

        CLIENT_INFO_MOVEMENTS(movimentos->m->id, tipo, movimentos->m->valor);

        movimentos = movimentos->prev; // Move para o movimento anterior na lista de movimentos
    }
}

void fecharConta(Banco *b)
{
    NoLClientes *cliente = b->clientes->first;

    char idconta[CODE_SIZE];
    memset(idconta, '\0', CODE_SIZE); // Coloca todas as posições do array "id_conta" com '\0', o que permite ao programa parar a leitura antes de chegar ao '\0' no final do array sempre que o id introduzindo não tenha a dimensão total do mesmo

    if (verificarExisteClientes(b)) // Verifica se não há clientes no banco
        return;

    int id = pedirInteiro(ASK_CLIENT_ID);
    cliente = getClientById(b, id); // Obtém o cliente pelo ID fornecido
    if (cliente == NULL)            // Retorna se o cliente não foi encontrado
        return;

    NoLContas *conta = cliente->c->contas->first;

    if (verificarExisteContas(cliente)) // Verifica se o cliente possui contas
        return;

    pedirString(idconta, ASK_ACCOUNT_ID, CODE_SIZE);
    conta = getAccountById(cliente, idconta); // Obtém a conta pelo ID fornecido

    if (conta == NULL) // Retorna se a conta não foi encontrada
        return;

    if (verificarExisteMovimentosConta(conta)) // Verifica se a conta possui movimentos
        removeMovimentos(conta);               // Remove os movimentos associados à conta

    removerConta(b, cliente, conta); // Remove a conta do cliente e do banco

    free(conta->c->movimentos); // Libera a memória alocada para os movimentos da conta
    free(conta->c);             // Libera a memória alocada para a estrutura da conta
    free(conta);                // Libera a memória alocada para o nó da lista de contas

    ACCOUNT_CLOSE_OK(idconta, id); // Informa ao usuário que a conta foi fechada com sucesso
}

//! MOVIMENTOS
void depositarDinheiro(Banco *b)
{
    NoLClientes *cliente = b->clientes->first;
    int tipo = DEPOSITO;
    int validacao = DEPOSITO; // validação do tipo de movimento que vai ser realizado na conta do cliente para transmitir à função "criarMovimento", que vai subtrair ou adicionar dependendo do tipo
    char id_conta[CODE_SIZE];
    memset(id_conta, '\0', CODE_SIZE); // Coloca todas as posições do array "id_conta" com '\0', o que permite ao programa parar a leitura antes de chegar ao '\0' no final do array sempre que o id introduzindo não tenha a dimensão total do mesmo

    if (verificarExisteClientes(b)) // Retorna um erro e sai da função se não existir clientes
        return;

    int id = pedirInteiro(ASK_CLIENT_ID);
    cliente = getClientById(b, id); // Verifica se existe um cliente com o ID introduzido. Retorna um erro se o resultado for NULL ou a posição do cliente na lista se o ID for igual

    if (cliente == NULL) // Verifica se o ciente retornou NULL da função anterior para sair da função "depositarDinheiro"
        return;
    if (verificarExisteContas(cliente)) // Retorna um erro e sai da função se não existir contas ativas no cliente
        return;

    pedirString(id_conta, ASK_ACCOUNT_ID, CODE_SIZE);
    NoLContas *conta = getAccountById(cliente, id_conta); // Verifica se existe uma conta com o ID de conta introduzido. Retorna um erro se o resultado for NULL ou a posição da conta na lista se o ID for igual

    if (conta == NULL) // Verifica se a conta retornou NULL da função anterior para sair da função "depositarDinheiro"
        return;

    double valor = pedirDouble(ASK_FOR_VALUE);

    criarMovimento(b, cliente, conta, valor, tipo, validacao); // Adiciona o movimento à lista de movimentos do banco e da conta do cliente

    DEPOSIT_OK(valor, conta->c->id_conta);
}

void levantarDinheiro(Banco *b)
{
    NoLClientes *cliente = b->clientes->first;
    int tipo = LEVANTAMENTO;
    int validacao = LEVANTAMENTO; // validação do tipo de movimento que vai ser realizado na conta do cliente para transmitir à função "criarMovimento", que vai subtrair ou adicionar dependendo do tipo
    char id_conta[CODE_SIZE];
    memset(id_conta, '\0', CODE_SIZE); // Coloca todas as posições do array "id_conta" com '\0', o que permite ao programa parar a leitura antes de chegar ao '\0' no final do array sempre que o id introduzindo não tenha a dimensão total do mesmo.

    if (verificarExisteClientes(b)) // Retorna um erro e sai da função se não existir clientes
        return;

    int id = pedirInteiro(ASK_CLIENT_ID);
    cliente = getClientById(b, id); // Verifica se existe um cliente com o ID introduzido. Retorna um erro se o resultado for NULL ou a posição do cliente na lista se o ID for igual

    if (cliente == NULL) // Verifica se o ciente retornou NULL da função anterior para sair da função "transferirDinheiro"
        return;
    if (verificarExisteContas(cliente)) // Retorna um erro e sai da função se não existir contas ativas no cliente
        return;

    pedirString(id_conta, ASK_ACCOUNT_ID, CODE_SIZE);
    NoLContas *conta = getAccountById(cliente, id_conta); // Verifica se existe uma conta com o ID de conta introduzido. Retorna um erro se o resultado for NULL ou a posição da conta na lista se o ID for igual

    if (conta == NULL) // Verifica se a conta retornou NULL da função anterior para sair da função "levantarDinheiro"
        return;

    double valor = pedirDouble(ASK_FOR_VALUE);

    if (verificarSaldoCliente(cliente, valor)) // Verifica se o cliente tem saldo integrado suficiente para fazer um levantamento, se o retorno da função for NULL sai da função "levantarDinheiro"
        return;

    criarMovimento(b, cliente, conta, valor, tipo, validacao); // Adiciona o movimento à lista de movimentos do banco e da conta do cliente

    WITHDRAW_OK(valor, conta->c->id_conta);
}

void transferirDinheiro(Banco *b)
{
    NoLClientes *cliente1 = b->clientes->first;
    int tipo = TRASFERENCIA;
    int validacao1 = LEVANTAMENTO; // validação do tipo de movimento que vai ser realizado na conta do cliente para transmitir à função "criarMovimento", que vai subtrair ou adicionar dependendo do tipo
    char id_conta[CODE_SIZE];
    memset(id_conta, '\0', CODE_SIZE); // Coloca todas as posições do array "id_conta" com '\0', o que permite ao programa parar a leitura antes de chegar ao '\0' no final do array sempre que o id introduzindo não tenha a dimensão total do mesmo.

    if (verificarExisteClientes(b)) // Retorna um erro e sai da função se não existir clientes
        return;

    int id1 = pedirInteiro(ASK_CLIENT_ID);
    cliente1 = getClientById(b, id1); // Verifica se existe um cliente com o ID introduzido. Retorna um erro se o resultado for NULL ou a posição do cliente na lista se o ID for igual

    if (cliente1 == NULL) // Verifica se o ciente retornou NULL da função anterior para sair da função "transferirDinheiro"
        return;
    if (verificarExisteContas(cliente1)) // Retorna um erro e sai da função se não existir contas ativas no cliente
        return;

    pedirString(id_conta, ASK_ACCOUNT_ID, CODE_SIZE);
    NoLContas *conta1 = getAccountById(cliente1, id_conta); // Verifica se existe uma conta com o ID de conta introduzido. Retorna um erro se o resultado for NULL ou a posição da conta na lista se o ID for igual

    if (conta1 == NULL) // Verifica se a conta retornou NULL da função anterior para sair da função "transferirDinheiro"
        return;

    double valor = pedirDouble(ASK_FOR_VALUE);

    if (verificarSaldoCliente(cliente1, valor)) // Verifica se o cliente tem saldo integrado suficiente para fazer uma transferência, se o retorno da função for NULL sai da função "transferirDinheiro"
        return;

    NoLClientes *cliente2 = b->clientes->first;
    int validacao2 = DEPOSITO; // validação do tipo de movimento que vai ser realizado na conta do cliente para transmitir à função "criarMovimento", que vai subtrair ou adicionar dependendo do tipo
    int id2 = pedirInteiro(ASK_CLIENT_ID);
    cliente2 = getClientById(b, id2); // Verifica se existe um cliente com o ID introduzido. Retorna um erro se o resultado for NULL ou a posição do cliente na lista se o ID for igual

    if (cliente2 == NULL) // Verifica se o ciente retornou NULL da função anterior para sair da função "transferirDinheiro"
        return;
    if (verificarExisteContas(cliente2)) // Retorna um erro e sai da função se não existir contas ativas no cliente
        return;

    pedirString(id_conta, ASK_ACCOUNT_ID, CODE_SIZE);
    NoLContas *conta2 = getAccountById(cliente2, id_conta); // Verifica se existe uma conta com o ID de conta introduzido. Retorna um erro se o resultado for NULL ou a posição da conta na lista se o ID for igual

    if (conta2 == NULL) // Verifica se a conta retornou NULL da função anterior para sair da função "transferirDinheiro"
        return;

    criarMovimento(b, cliente1, conta1, valor, tipo, validacao1);
    criarMovimento(b, cliente2, conta2, valor, tipo, validacao2);

    TRANSFER_OK(valor, conta1->c->id_conta, conta2->c->id_conta);
}