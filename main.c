/**
 * Author: O vosso nome
 * Date: 29Nov2020
 * Description: Descrição ....
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// include bibliotecas internas
#include "ui.h"
#include "sgb.h"

/**
 * Process os pedidos no menu Clientes
 * @param banco
 **/
void processaMenuClientes(Banco *b)
{
    int opt;
    while (1)
    {
        opt = menuClientes();
        switch (opt)
        {
        case 1:
            inserirCliente(b);
            break;
        case 2:
            listarClientes(b);
            break;
        case 3:
            mostrarCliente(b);
            break;
        case 4:
            apagarCliente(b);
            break;
        case 5:
            procurarCliente(b);
            break;
        case 9:
            return; // Sai da função, logo do menu Clientes
        default:
            INVALID_OPTION;
        }
    }
}

/**
 * Process os pedidos no menu Contas
 * @param banco
 **/
void processaMenuContas(Banco *b)
{
    int opt;
    while (1)
    {
        opt = menuContas();
        switch (opt)
        {
        case 1:
            inserirConta(b);
            break;
        case 2:
            listarContas(b);
            break;
        case 3:
            mostrarContas(b);
            break;
        case 4:
            fecharConta(b);
            break;
        case 9:
            return; // Sai da função, logo do menu Contas
        default:
            INVALID_OPTION;
        }
    }
}

/**
 * Process os pedidos no meu Clientes
 * @param banco
 **/
void processaMenuMovimentos(Banco *b)
{
    int opt;
    while (1)
    {
        opt = menuMovimentos();
        switch (opt)
        {
        case 1:
            depositarDinheiro(b);
            break;
        case 2:
            levantarDinheiro(b);
            break;
        case 3:
            transferirDinheiro(b);
            break;
        case 9:
            return; // Sai da função, logo do menu Movimentos
        default:
            INVALID_OPTION;
        }
    }
}

int main(int argc, char const *argv[])
{
    Banco *b;
    int opt;
    b = iniciarBanco("ATEC Bank");
    while (1)
    {
        opt = menu();
        switch (opt)
        {
        case 1:
            processaMenuClientes(b);
            break;
        case 2:
            processaMenuContas(b);
            break;
        case 3:
            processaMenuMovimentos(b);
            break;
        case 4:
            mostrarInfoBanco(b);
            break;
        case 5:
            monstrarInfoCompleta(b);
            break;
        case 6:
            listarMovimentosBanco(b);
            break;
        case 7:
            avancarData(b);
            break;
        case 8:
            listarMelhoresClientes(b);
            break;
        case 9:
            puts("#A sair da aplicação Banco#");
            free_banco(b);      // Liberta recursos
            exit(EXIT_SUCCESS); // Sai da aplicação
        default:
            INVALID_OPTION;
            break;
        }
    }

    return EXIT_SUCCESS;
}