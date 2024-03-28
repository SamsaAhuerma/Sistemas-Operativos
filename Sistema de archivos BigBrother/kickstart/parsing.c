#include <stdlib.h>
#include <stdbool.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p){
    scommand scom = scommand_new();
    arg_kind_t type;
    char *arg;

    parser_skip_blanks(p);
    arg = parser_next_argument(p, &type);
    if (arg == NULL){
        printf("Error, el comando es vacío\n");
        scommand_destroy(scom);
        return NULL;
    }
    if (type == ARG_INPUT || type == ARG_OUTPUT){
        printf("Error de parseo, no hay argumento\n");
        scommand_destroy(scom);
        return NULL;
    }
    if (type == ARG_NORMAL){
        while (type == ARG_NORMAL && arg != NULL){
            scommand_push_back(scom, arg);
            parser_skip_blanks(p);
            arg = parser_next_argument(p, &type);
        }
        if (type == ARG_NORMAL && arg == NULL){
            return scom;
        }
        if (type == ARG_OUTPUT){
            if (arg == NULL){
                printf("error sintáctico: Redirección de salida vacía\n");
                scommand_destroy(scom);
                return NULL;
            }
            else{
                scommand_set_redir_out(scom, arg);
                parser_skip_blanks(p);
                arg = parser_next_argument(p, &type);
            }
            if (type == ARG_INPUT){
                if (arg == NULL){
                printf("error sintáctico: Redirección de entrada vacía\n");
                scommand_destroy(scom);
                return NULL;
                }
            }
            else{
                scommand_set_redir_in(scom, arg);
                parser_skip_blanks(p);
            }
            
        }
        else if (type == ARG_INPUT){
            if (arg == NULL){
                printf("error sintáctico: Redirección de entrada vacía\n");
                scommand_destroy(scom);
                return NULL;
            }
            else{
                scommand_set_redir_in(scom, arg);
                parser_skip_blanks(p);
                arg = parser_next_argument(p, &type);
            }
            if (type == ARG_OUTPUT){
                if (arg == NULL){
                printf("error sintáctico: Redirección de salida vacía\n");
                scommand_destroy(scom);
                return NULL;
                }
                else{
                scommand_set_redir_out(scom, arg);
                parser_skip_blanks(p);
                }
            }
            
        }
    }
    return scom;
}

pipeline parse_pipeline(Parser p){
    if (p == NULL || parser_at_eof(p)) {
        return NULL;
    }
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe = true, garbage = false;

    cmd = parse_scommand(p);
    error = (cmd == NULL);
    
    while (!error && another_pipe){
        pipeline_push_back(result, cmd);
        parser_op_pipe(p, &another_pipe);
        if(another_pipe){
            cmd = parse_scommand(p);
            error = (cmd == NULL);
        }
    }
    if (error){
        parser_garbage(p, &garbage);
        pipeline_destroy(result);
        return NULL;
    }
    bool is_background;
    parser_op_background(p, &is_background);
    pipeline_set_wait(result, !is_background);

    parser_garbage(p, &garbage);
    if (garbage){
        printf("Error: Caracteres no reconocidos después del pipeline\n");
        pipeline_destroy(result);
        return NULL;
    }

    return result;
}
