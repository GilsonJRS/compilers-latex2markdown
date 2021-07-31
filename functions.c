/*
*   Advanced Calculator auxiliary functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "header.h"

struct ast *newast(int nodetype, struct ast *l, struct ast *r){
    struct ast *a = malloc(sizeof(struct ast));

    if(!a){
        yyerror("no space");
        exit(0);
    }
    a->nodetype = nodetype;
    a->l=l;
    a->r=r;
    return a;
}

struct ast *newPhrase(char *c){
    struct phrase *a = malloc(sizeof(struct phrase));

    if(!a){
        yyerror("no space");
        exit(0);
    }
    a->nodetype = "P";
    a->c = c;
    return (struct ast *)a;
}


/* free AST */
void treefree(struct ast *a){
    switch(a->nodetype){
        case '+':
        case '-':
        case '*':
        case '/':
        case '1': case '2': case '3': case '4': case '5':case '6':
        case 'L':
            treefree(a->r);
        case 'C': case 'F':
            treefree(a->l);
        case 'K': case 'N':
            break;
        case '=':
            free(((struct symasgn *)a)->v);
            break;
        case 'I': case 'W':
            free(((struct flow *)a)->cond);
            if(((struct flow *)a)->tl) treefree(((struct flow *)a)->tl);
            if(((struct flow *)a)->el) treefree(((struct flow *)a)->el);
            break;
        default:
            printf("erro interno: free bad node %c\n", a->nodetype);
    }
    free(a);
}

double writeOutput(struct ast *a){
    return 0.0;
    double v;
    
    if(!a){
        yyerror("erro interno, null eval");
        return 0.0;
    }

    switch(a->nodetype){
       
    }
    return v;
}

void yyerror(char *s, ...){
    va_list ap;
    va_start(ap,s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

int main(){
    return yyparse();
}