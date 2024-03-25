#include "utils.h"

int main()
{
    // Lista de alias
    Alias_lista lista;
    lista.size = 0;

    // Historico de comandos
    History historico;
    historico.size = 0;
    
    // Comando do usuario
    char comando[MAX_SIZE_STR];

    // Guarda o repositorio original
    char s[MAX_SIZE_STR]; 
    char* repo_origim = getcwd(s, 100);

    while (1)
    {
        prompt(comando, sizeof(comando), repo_origim, historico);

        /*
            A funcao lidar_internos lida com as possibilidades de:
            1-exit/2-cd/3-history/4-alias/5-!n
            
            Caso o usuario digite um comando que está na lista de alias, ele retorna o comando
            Outra possibilidade de retorno é a função retornar o comando respectivo do !n
        */  
        char* comando_aux = lidar_internos(comando, historico, lista);

        /*
            Se o comando_aux for diferente de null, quer dizer que a funcao retornou
            um comando do alias ou o respectivo comando do !n
        */
        if (comando_aux != NULL){
            strcpy(comando, comando_aux);
        }

        pid_t pid = fork();
        if (pid < 0){
            perror("fork");
            _exit(1);
        }
        else if (pid == 0){
            executar_comando(comando);
        } 
        else {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}