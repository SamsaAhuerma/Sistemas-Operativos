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



void execute_pipeline(pipeline apipe){
    signal(SIGCHLD, SIG_IGN);
    if(pipeline_length(apipe) != 0){
        scommand frente = pipeline_front(apipe);
        while(builtin_is_internal(frente)){
            if(builtin_alone(apipe)){
                builtin_run(frente);
                pipeline_pop_front(apipe);
                break;
            }else if(builtin_is_internal(frente)){
                builtin_run(frente);
                pipeline_pop_front(apipe);
                frente = pipeline_front(apipe);
            }     
        }  
       if(pipeline_length(apipe) != 0){
            if(pipeline_get_wait(apipe)){
                //Correrlo en foreground
                unsigned int childs = exec_pipe_fg(apipe);
                while(childs > 0 ){
                    wait(NULL);
                    childs--;
                }        
            }else{
                //Correrlo en background
                pid_t pid = fork();
                if(pid<0){
                    //Fallo en crear el hijo
                    exit(EXIT_FAILURE);
                }else if(pid == 0){
                    //unsigned int childs = exec_pipe_fg(apipe);
                    exec_pipe_fg(apipe);
                    // while(childs > 0 ){
                    //     wait(NULL);
                    //     childs--;
                    // }
                    //childs--;
                    exit(EXIT_SUCCESS);
                }
            }
       }   
    }
    

}

char ** argv_getter(scommand cmd){
    int command_size = scommand_length(cmd);
    char ** argv = calloc(command_size+1,sizeof(char *));
    for (int i=0; i < command_size; i++){
        char * arg = strdup(scommand_front(cmd));
        argv[i] = arg;
        scommand_pop_front(cmd);
    }
    argv[command_size] = NULL;
    return argv;    
}

bool change_fd_in(scommand cmd){
    assert(cmd != NULL);

    char * redir_in = scommand_get_redir_in(cmd);

    if(redir_in != NULL){
        int fd_in = open(redir_in, O_RDONLY, S_IRWXU);
        //fallo el open
        if(fd_in == -1){
            return false;
        }
        int clone_fd = dup2(fd_in, STDIN_FILENO);
        //fallo dup2
        if(clone_fd == -1){
            close(fd_in);
            return false;
        }

        int close_fd = close(fd_in);
        //fallo close
        if(close_fd == -1){
            return false;
        }
    }
    return true;
}

bool change_fd_out(scommand cmd){
    assert(cmd != NULL);

    char * redir_out = scommand_get_redir_out(cmd);

    if(redir_out != NULL){
        int fd_out = open(redir_out, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        //fallo el open
        if(fd_out == -1){
            return false;
        }
        int clone_fd = dup2(fd_out, STDOUT_FILENO);
        //fallo dup2
        if(clone_fd == -1){
            close(fd_out);
            return false;
        }

        int close_fd = close(fd_out);
        //fallo close
        if(close_fd == -1){
            return false;
        }
    }
    return true;
}


void exec_any_scommand(scommand cmd){
    assert(cmd != NULL);
    if(builtin_is_internal(cmd)){
        builtin_run(cmd);
        exit(EXIT_SUCCESS);
    }else if(!scommand_is_empty(cmd)){
        bool res_redir_in = change_fd_in(cmd);
        if(!res_redir_in){
            exit(EXIT_FAILURE);
        }
        bool res_redir_out = change_fd_out(cmd);
        if(!res_redir_out){
            exit(EXIT_FAILURE);
        }
        char ** argv = argv_getter(cmd);
        if(argv == NULL){
            exit(EXIT_FAILURE);
        }
        execvp(argv[0],argv);
    //Si no se ejecuta el execvp entonces se ejecuta el exit
        printf("Orden no encontrada.\n");
        exit(EXIT_FAILURE);
    }else{
        exit(EXIT_SUCCESS);
    }  
}
    


//Ejecuta el pipeline de hasta 2 comandos
unsigned int exec_pipe_fg(pipeline apipe){
    assert(pipeline_length(apipe) <= 2);
    unsigned int pipelineLength = pipeline_length(apipe);
    unsigned int childs = 0;
    if (pipelineLength == 1){
        pid_t pid = fork();
        if(pid<0){
            //Fallo en crear el hijo
            exit(EXIT_FAILURE);
        }else if(pid == 0){
            //En el proceso hijo         
            scommand cmd = pipeline_front(apipe); 

            exec_any_scommand(cmd);
            
        }
            //Padre cuenta proceso
            pipeline_pop_front(apipe);
            childs++;
    }else if(pipelineLength == 2){
        int fd[2];
        if(pipe(fd) == -1){
            exit(EXIT_FAILURE);
        }
        pid_t pid = fork();
        if(pid<0){
            exit(EXIT_FAILURE);
        }else if(pid == 0){
            //En el proceso hijo hacemos la comunicacion de procesos via pipe
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            exec_any_scommand(pipeline_front(apipe));
        }
        childs++;
        pipeline_pop_front(apipe);
        if(builtin_is_internal(pipeline_front(apipe))){
            builtin_run(pipeline_front(apipe));
            pipeline_pop_front(apipe);
            exit(EXIT_SUCCESS);
        }else{
            pid_t pid2 = fork();
            if(pid2<0){
                exit(EXIT_FAILURE);
            }else if(pid2 == 0){
                //En el segundo proceso hijo hacemos la comunicacion del proceso via pipe
                dup2(fd[0], STDIN_FILENO);
                close(fd[1]);
                close(fd[0]);
                exec_any_scommand(pipeline_front(apipe));
            }
            childs++;
            pipeline_pop_front(apipe);
        }
        close(fd[1]);
        close(fd[0]);

    }
    assert(childs == 0 || childs == 1 || childs == 2);
    return childs;
}