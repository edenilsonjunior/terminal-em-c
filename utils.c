#include "utils.h"
// Imprime e le o prompt
void prompt(char* comando, int size, char* repo_origim, History historico){
    
    time_t tempo_em_segundos = time(NULL);
    struct tm *data = localtime(&tempo_em_segundos);

    // Guarda o diretório atual
    char s[MAX_SIZE_STR]; 
    char* repo_atual = getcwd(s, sizeof(s));

    // Se o diretório atual for igual ao diretório original, ele imprime '$', caso contrário, ele imprime o diretório atual
    char* repositorio = strcmp(repo_origim, repo_atual) == 0 ? "$" : repo_atual;
    
    printf("user@host[%d:%d:%d] %s ", data->tm_hour, data->tm_min, data->tm_sec, repositorio);
    
    fgets(comando, size, stdin);
    comando[strlen(comando) -1] = '\0';

    guardar_comando(comando, historico);
}


// Guarda o comando no historico
void guardar_comando(const char* comando, History historico){

    int indice = historico.size;

    if (indice == 10)
    {
        for (int i = 0; i < 9; i++){
            strcpy(historico.comando[i],historico.comando[i + 1]);
        }
        indice--;
    }

    strcpy(historico.comando[indice], comando);
    indice++;

    historico.size = indice;
}


// Metodo que lida com os comandos: exit, cd, history, alias
char* lidar_internos(const char* comando, History historico, Alias_lista lista){

    if (strcmp(comando, "exit") == 0){
        _exit(0);

    }
    else if(strncmp(comando, "cd ", 3) == 0){
        comando_cd(comando);

    }
    else if(strcmp(comando, "history") == 0){
        listar_comandos(historico);

    }
    else if(strncmp(comando, "alias ", 6) == 0){
        adicionar_alias(comando, lista);

    }
    else if(strncmp(comando, "!", 1) == 0){
        return comando_n(comando, historico);
    }
    else{
        // Se não for nenhum dos comandos internos, ele retorna caso o comando seja um alias
        for (int i = 0; i < lista.size; i++)
        {
            if (strcmp(comando, lista.comandos[i].nome) == 0)
            {
                return lista.comandos[i].comando;
            }
        }
    }

    return NULL;
}


// Executa o comando cd
void comando_cd(char* comando){
    char diretorio[MAX_SIZE_STR];
    int indice_diretorio = 0;

    for (size_t i = 3; i <= strlen(comando); i++)
    {
        diretorio[indice_diretorio] = comando[i];
        indice_diretorio++;
    }      
    
    diretorio[indice_diretorio] = '\0';

    chdir(diretorio);
}


// Mostra todos os comandos do historico
void listar_comandos(History historico){

    for (int i = 0; i < historico.size; i++)
    {
        printf("%d %s\n", i, historico.comando[i]);
    }
}


// Adiciona um alias a lista
void adicionar_alias(const char* comando, Alias_lista lista){
    
    int tamanho = lista.size;

    if (tamanho >= MAX_ALIAS)
    {
        printf("Lista de alias cheia!");
        return;
    }

    char nome[MAX_SIZE_STR];
    char comando_alias[MAX_SIZE_STR];

    int total = 0;
    int indice = 6;
    while (comando[indice] != '=')
    {
        nome[total] = comando[indice];
        total++;
        indice++;
    }
    nome[total] = '\0';


    total = 0;
    while (comando[indice] != '\'')
    {
        if (comando[indice] == '\''){
            indice++;
        }else{
            comando_alias[total] = comando[indice];
            total++;
        }
    }

    tamanho++;

    strcpy(lista.comandos[tamanho].nome, nome);
    strcpy(lista.comandos[tamanho].comando, comando_alias);

    lista.size = tamanho;
}


// Retorna o comando da posicao n dentro do historico
char* comando_n(const char* comando, History historico){

    int n = atoi(&comando[1]);

    if (n >= 0 && n <= historico.size){
        return historico.comando[n];
    }

    return NULL;
}


// Executa o comando no filho
void executar_comando(const char* texto){

    char* argumentos[MAX_SIZE_STR];
    int linha = 0;
    int indice_original = 0;
    int indice_palavra = 0;

    // Loop que irá percorrer todo o texto(comando)
    while (texto[indice_original] != '\0') 
    {
        // se o texto for diferente de espaco
        if (texto[indice_original] != ' ') 
        {
            // se for o primeiro indice da palavra, ele aloca espaco, caso nao, ele apenas adiciona o caractere
            if (indice_palavra == 0)
            {
                argumentos[linha] = malloc(MAX_SIZE_STR * sizeof(char));
            }
            argumentos[linha][indice_palavra] = texto[indice_original];
            indice_palavra++;
        } 
        else 
        {
            // Se entrou aqui é por que achou um ' ', nesse caso adiciona o '\0', muda a linha da matriz e reseta o indice da palavra
            argumentos[linha][indice_palavra] = '\0';
            linha++;
            indice_palavra = 0;
        }
        indice_original++;
    }
    
    // adiciona o '\0' a ultima string
    argumentos[linha][indice_palavra] = '\0';

    // para o execvp funcionar, a ultima string deve ser NULL
    linha++;
    argumentos[linha] = NULL;
    
    execvp(argumentos[0], argumentos);
    perror("Erro ao executar o comando");
    exit(EXIT_FAILURE);
}
