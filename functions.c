/*
*   Advanced Calculator auxiliary functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "header.h"

struct ast *newast(char *nodetype, struct ast *l, struct ast *r){
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
    if(a==NULL) return;
    if(!strcmp(a->nodetype, "Begin")){
        treefree(a->l);
        treefree(a->r);
    }
    else if(!strcmp(a->nodetype, "Opts")
        || !strcmp(a->nodetype, "Class")
        || !strcmp(a->nodetype, "PK")){
        free(((struct phrase *)a->l)->c);
        treefree(a->r);
    }
    else if(!strcmp(a->nodetype, "Body")
        || !strcmp(a->nodetype, "Co")){
        if(a->l!=NULL)
            treefree(a->l);
    } else if(!strcmp(a->nodetype, "Opts")
        || !strcmp(a->nodetype, "Class")
        || !strcmp(a->nodetype, "PK")
        || !strcmp(a->nodetype, "CC")
        || !strcmp(a->nodetype, "B")
        || !strcmp(a->nodetype, "U")
        || !strcmp(a->nodetype, "I")
        || !strcmp(a->nodetype, "P")
        ){
        free(((struct phrase *)a->l)->c);
        treefree(a->r);
    }else if(!strcmp(a->nodetype, "C")
        || !strcmp(a->nodetype, "S")
        || !strcmp(a->nodetype, "SB")
        || !strcmp(a->nodetype, "SSB")
        || !strcmp(a->nodetype, "PG")
        ){
        treefree(a->l);
        treefree(a->r);
    }else if(!strcmp(a->nodetype, "E")
        || !strcmp(a->nodetype, "IT")){
        if(a->l != NULL)
            treefree(a->l);
        if(a->r != NULL)
            treefree(a->r);
    }else if(!strcmp(a->nodetype, "ITEM")){
        free(((struct phrase *)a->l)->c);
    } 

    free(a);
}

void writeOutput(struct ast *a){
    if(a==NULL) return;
    if(!strcmp(a->nodetype, "Begin")){
        printf("\nBegin\n");
        writeOutput(a->l);
        writeOutput(a->r);
    }
    else if(!strcmp(a->nodetype, "Opts")){
        printf("Opts: %s\n", ((struct phrase *)a->l)->c);
        writeOutput(a->r);
    }
    else if(!strcmp(a->nodetype, "Class")){
        printf("Class: %s\n", ((struct phrase *)a->l)->c);
        writeOutput(a->r);
    }
    else if(!strcmp(a->nodetype, "PK")){
        printf("Package: %s\n", ((struct phrase *)a->l)->c);
        writeOutput(a->r);
    }
    else if(!strcmp(a->nodetype, "Main")){
        printf("\nMain\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    } 
    else if(!strcmp(a->nodetype, "Title")
        || !strcmp(a->nodetype, "Author")
        ){
        printf("%s: %s\n", a->nodetype, ((struct phrase *)a->l)->c);
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(!strcmp(a->nodetype, "Body")){
        printf("\nBody\n");
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(!strcmp(a->nodetype, "C")){
        printf("#\n");
    }else if(!strcmp(a->nodetype, "S")){
        printf("##\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(!strcmp(a->nodetype, "SB")){
        printf("###\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(!strcmp(a->nodetype, "SSB")){
        printf("###\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(!strcmp(a->nodetype, "PG")){
        printf("####\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(!strcmp(a->nodetype, "CC")
        
    ){
        printf("%s\n", ((struct phrase *)a->l)->c);
    }

}

void yyerror(char *s, ...){
    va_list ap;
    va_start(ap,s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

/*void catstr(char **s1, char *s2){
    int i=0, j=0;
    char *ss1 = *s1, *ss2=s2;
    while(ss1 != NULL && *ss1!='\0'){
        ss1++;
        i++;
    }
    while(ss2 != NULL && *ss2!='\0'){
        ss2++;
        j++;
    }
    **s1 = realloc(**s1, i+j);
    strcat(**s1, s2);
}*/

int main(){
    FILE *input = fopen("inputFiles/input_1.tex", "r");
    
    if(!input){
        yyerror("Error in file opening");
        exit(0);
    }

    yyin = input;
    yyparse();
    fclose(input);
}