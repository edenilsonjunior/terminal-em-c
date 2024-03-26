#include "utils.h"

int main()
{
    char comando[MAX_SIZE_STR];

    Alias_lista lista;
    lista.size = 0;

    History historico;
    historico.size = 0;


    // Guarda o repositorio original
    char s[MAX_SIZE_STR]; 
    char* repo_origim = getcwd(s, 100);

    while (1)
    {
        prompt(comando, MAX_SIZE_STR, repo_origim, &historico);

        // Se a funcao retornar NULL, quer dizer que o comando é um comando interno
        char* comando_aux = lidar_internos(comando, &historico, &lista);
        
        if (comando_aux == NULL) continue;
        
        strcpy(comando, comando_aux);
        
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