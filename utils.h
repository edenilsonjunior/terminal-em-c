#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ALIAS 20
#define MAX_SIZE_STR 100

typedef struct struct_alias
{
    char nome[MAX_SIZE_STR];
    char comando[MAX_SIZE_STR];
} Alias;

typedef struct alias_lista {
    Alias comandos[MAX_ALIAS];
    int size;
}Alias_lista;

typedef struct history
{
    char comando[10][MAX_SIZE_STR];
    int size;
}History;


// Imprime e le o prompt
void prompt(char* comando, int size, char* repo_origim, History* historico);

// Guarda o comando no historico
void guardar_comando(char* comando, History* historico);

// Metodo que lida com os comandos: exit, cd, history, alias
char* lidar_internos(char* comando, History* historico, Alias_lista* lista);

// Executa o comando cd
void comando_cd(char* comando);

// Mostra todos os comandos do historico
void listar_comandos(History* historico);

// Adiciona um alias a lista
void adicionar_alias(char* comando, Alias_lista* lista);

// Retorna o comando da posicao n dentro do historico
char* comando_n(char* comando, History* historico);

// Executa o comando no filho
void executar_comando(char* texto);