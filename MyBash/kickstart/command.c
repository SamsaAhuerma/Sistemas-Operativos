#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <assert.h>
#include <strings.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"
#include "strextra.h"


// Comando simple (scommand)
// Es del tipo ([char*], char*, char*) --> haciendo referencia al (argumento (comando) , redirecionamiento in (<), redirecionamiento out (>))

struct scommand_s{
    GSList *arg;
    char *redir_in;
    char *redir_out;
};

scommand scommand_new(){
    scommand res = malloc(sizeof(struct scommand_s));

    res->arg = NULL;
    res->redir_in = NULL;
    res->redir_out = NULL;

    assert(res != NULL && scommand_is_empty(res) && scommand_get_redir_in (res) == NULL && scommand_get_redir_out(res) == NULL);
    return res;
}

scommand scommand_destroy(scommand self){
    assert(self != NULL);

    g_slist_free_full(self->arg, free); 
    self->arg = NULL;  
    free(self->redir_in);
    self->redir_in = NULL;
    free(self->redir_out);
    self->redir_out = NULL;
    free(self);
    self=NULL;

    assert(self == NULL);
    return self;
}

void scommand_push_back(scommand self, char * argument){
    assert(self!=NULL && argument!=NULL);

    self->arg = g_slist_append(self->arg, argument);

    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self){
    assert(self != NULL && !scommand_is_empty(self));
    GSList *aux = NULL;
    aux = g_slist_append(aux,g_slist_nth_data(self->arg, 0));
    self->arg= g_slist_remove(self->arg, aux->data);
    g_slist_free_full(aux, free); 
}

void scommand_set_redir_in(scommand self, char * filename){
    assert(self != NULL);
    if(self->redir_in != NULL){
        free(self->redir_in);
    }
    self->redir_in = filename;
}

void scommand_set_redir_out(scommand self, char * filename){
    assert(self != NULL);
    if(self->redir_out != NULL){
        free(self->redir_out);
    }
    self->redir_out = filename;
}

bool scommand_is_empty(const scommand self){
    assert(self != NULL);
    bool res = scommand_length(self) == 0;
    return res;
}

unsigned int scommand_length(const scommand self){
    assert(self != NULL);
    unsigned int res = g_slist_length(self->arg);
    return res;
}

char * scommand_front(const scommand self){
    assert(self != NULL && !scommand_is_empty(self));
    char * res = g_slist_nth_data(self->arg, 0);
    assert(res != NULL);
    return res;
}

char * scommand_get_redir_in(const scommand self){
    assert(self!=NULL);
    char * res = self->redir_in;
    return res;
}
char * scommand_get_redir_out(const scommand self){
    assert(self!=NULL);
    char * res = self->redir_out;
    return res;
}

char * scommand_to_string(const scommand self){
	assert(self!= NULL);
	char *res = strdup("");
	unsigned int length = scommand_length(self);

	for(unsigned int i = 0; i < length; ++i){
		res = merge_and_free(res,g_slist_nth_data(self->arg, i));
		res = merge_and_free(res," ");
	}
	if (self->redir_in!=NULL){
		res = merge_and_free(res," ");
		res = merge_and_free(res,"<");
		res = merge_and_free(res," ");
		res = merge_and_free(res,self->redir_in);
	}
	if (self->redir_out!=NULL){
		res = merge_and_free(res," ");
		res = merge_and_free(res,">");
		res = merge_and_free(res," ");
		res = merge_and_free(res,self->redir_out);
	}
	assert(scommand_is_empty(self) || scommand_get_redir_in(self)==NULL || scommand_get_redir_out(self)==NULL || strlen(res)>0);
	return res;
}

//pipeline implementation
struct pipeline_s{
    GList *scommand;
    bool bof;
};

pipeline pipeline_new(void){
    pipeline res = malloc(sizeof(struct pipeline_s));
    res->scommand = NULL;
    res->bof = true;
   
    assert(pipeline_is_empty(res) && pipeline_get_wait(res));
    return res;
}

pipeline pipeline_destroy(pipeline self){
    assert(self != NULL);
    
    unsigned int length = pipeline_length(self);
    for(unsigned int i=0 ; i < length;++i){
        pipeline_pop_front(self);
    }
    g_list_free(self->scommand);

    self->scommand = NULL;
    free(self);
    self = NULL;

    assert(self == NULL);
    return self;

}

void pipeline_push_back(pipeline self, scommand sc){
    assert(self!=NULL && sc!=NULL);
   
    self->scommand = g_list_append(self->scommand, sc);

    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self){
    assert(self!=NULL && !pipeline_is_empty(self));
    scommand command = g_list_nth_data(self->scommand,0);
    self->scommand = g_list_remove(self->scommand, command);
    scommand_destroy(command);
}

void pipeline_set_wait(pipeline self, const bool w){
    assert(self!=NULL);

    self->bof = w;  
}

bool pipeline_is_empty(const pipeline self){
    assert(self!=NULL);
    bool res = false;

    if (g_list_length(self->scommand) == 0){
        res = true;  
    }
    return res;
}

unsigned int pipeline_length(const pipeline self){
    assert(self!=NULL);
   
    unsigned int length = g_list_length(self->scommand);

    assert((length==0) == pipeline_is_empty(self));
    return length;
}

scommand pipeline_front(const pipeline self){
    assert(self!=NULL && !pipeline_is_empty(self));

    scommand scommandd = g_list_nth_data(self->scommand,0);

    assert(scommandd!=NULL);
    return scommandd;
}

bool pipeline_get_wait(const pipeline self){
    assert(self!=NULL);

    return self->bof;
}

char * pipeline_to_string(const pipeline self){
    assert(self != NULL);
	char *res =strdup("");
	char *str = NULL;

	unsigned int length = pipeline_length(self);

    for(unsigned int i = 1; i < length; i++) {
		res = merge_and_free(res, str=scommand_to_string(g_list_nth_data(self->scommand,i)));
		free(str);
		res = merge_and_free(res," ");
		res = merge_and_free(res,"|"); 
		res = merge_and_free(res, " ");

    }
	if(!pipeline_get_wait(self)){
		res = merge_and_free(res," ");
		res = merge_and_free(res,"&");
	}
	assert(pipeline_is_empty(self) || pipeline_get_wait(self) || strlen(res)>0);
    return res;
}
