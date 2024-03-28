/* Ejecuta comandos simples y pipelines.
 * No toca ningún comando interno.
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include "command.h"


void execute_pipeline(pipeline apipe);
/*
 * Ejecuta un pipeline, identificando comandos internos, forkeando, y
 *   redirigiendo la entrada y salida. puede modificar `apipe' en el proceso
 *   de ejecución.
 *   apipe: pipeline a ejecutar
 * Requires: apipe!=NULL
 */
char ** argv_getter(scommand cmd);
bool change_fd_in(scommand cmd);
bool change_fd_out(scommand cmd);
unsigned int exec_pipe_fg(pipeline apipe);
void exec_any_scommand(scommand cmd);
#endif /* EXECUTE_H */
