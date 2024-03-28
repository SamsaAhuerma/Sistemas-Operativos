#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "tests/syscall_mock.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"

bool builtin_is_internal(scommand cmd){
    assert(cmd != NULL);
    char * arg = scommand_front(cmd);
    bool result = ((strcmp(arg,"cd") == 0) || (strcmp(arg,"help") == 0) || (strcmp(arg,"exit") == 0));
    return result;
}

bool builtin_alone(pipeline p){
    assert(p != NULL);
    bool res = (pipeline_length(p) == 1) && (builtin_is_internal(pipeline_front(p)));
    return res;
}

void builtin_run(scommand cmd){
    assert(builtin_is_internal(cmd));
    char * arg = scommand_front(cmd);
    if (strcmp(arg,"cd") == 0){
        scommand_pop_front(cmd);
        if (scommand_is_empty(cmd)){
            int error = chdir(getenv("HOME"));
            if(error != 0 ){
                printf("cd : %s : %s\n",strerror(2), scommand_front(cmd));
            }
        }
        else{
            const char * path=scommand_front(cmd);
            int res = chdir(path);
            if(res != 0 ){
                printf("cd : %s : %s\n",strerror(2), scommand_front(cmd));
            }
        }
    }
    else if (strcmp(arg,"help") == 0){
        printf("    MyBash2023\n    Autores: Consuelo Moyano, Ezequiel Ludueña, Martín Salinas y Samsa Ahuerma \n    Listado de comandos internos:\n\n        cd [PATH]\n            Cambia el directorio de trabajo actual al especificado en [PATH]. Si [Path] es vacio se cambia al Home.\n\n        help \n            Muestra el nombre de esta terminal, sus autores y el listado de comandos internos definidos.\n\n        exit\n            Sale de la terminal y finaliza su proceso. \n");
    }
    else if (strcmp(arg,"exit") == 0){
        signal(SIGQUIT, SIG_IGN);
        kill(0, SIGQUIT);
        exit(EXIT_SUCCESS);
    }
}
