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
    if(strcmp(a->nodetype, "Begin")==0){
        treefree(a->l);
        treefree(a->r);
    }
    else if(strcmp(a->nodetype, "Opts")==0
        || strcmp(a->nodetype, "Class")==0
        || strcmp(a->nodetype, "PK")==0){
        free(((struct phrase *)a->l)->c);
        treefree(a->r);
    }
    else if(strcmp(a->nodetype, "Body")==0
        || strcmp(a->nodetype, "Co")==0){
        if(a->l!=NULL)
            treefree(a->l);
    } else if(strcmp(a->nodetype, "Opts")==0
        || strcmp(a->nodetype, "Class")==0
        || strcmp(a->nodetype, "PK")==0
        || strcmp(a->nodetype, "CC")==0
        || strcmp(a->nodetype, "B")==0
        || strcmp(a->nodetype, "U")==0
        || strcmp(a->nodetype, "I")==0
        || strcmp(a->nodetype, "P")==0
        ){
        free(((struct phrase *)a->l)->c);
        treefree(a->r);
    }else if(strcmp(a->nodetype, "C")==0
        || strcmp(a->nodetype, "S")==0
        || strcmp(a->nodetype, "SB")==0
        || strcmp(a->nodetype, "SSB")==0
        || strcmp(a->nodetype, "PG")==0
        ){
        treefree(a->l);
        treefree(a->r);
    }else if(strcmp(a->nodetype, "E")==0
        || strcmp(a->nodetype, "IT")==0){
        if(a->l != NULL)
            treefree(a->l);
        if(a->r != NULL)
            treefree(a->r);
    }else if(strcmp(a->nodetype, "ITEM")==0){
        free(((struct phrase *)a->l)->c);
    } 

    free(a);
}

void writeOutput(struct ast *a){
    if(a==NULL) return;
    if(strcmp(a->nodetype, "Begin")==0){
        printf("\nBegin\n");
        writeOutput(a->l);
        writeOutput(a->r);
    }
    else if(strcmp(a->nodetype, "Opts")==0){
        printf("Opts: %s\n", ((struct phrase *)a->l)->c);
        writeOutput(a->r);
    }
    else if(strcmp(a->nodetype, "Class")==0){
        printf("Class: %s\n", ((struct phrase *)a->l)->c);
        writeOutput(a->r);
    }
    else if(strcmp(a->nodetype, "PK")==0){
        printf("Package: %s\n", ((struct phrase *)a->l)->c);
        writeOutput(a->r);
    }
    else if(strcmp(a->nodetype, "Main")==0){
        printf("\nMain\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    } 
    else if(strcmp(a->nodetype, "Title")==0
        || strcmp(a->nodetype, "Author")==0
        ){
        printf("%s: %s\n", a->nodetype, ((struct phrase *)a->l)->c);
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "Body")==0){
        printf("\nBody\n");
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "C")==0){
        printf("#\n");
    }else if(strcmp(a->nodetype, "S")==0){
        printf("##\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "SB")==0){
        printf("###\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "SSB")==0){
        printf("###\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "PG")==0){
        printf("####\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "CC")==0){
        printf("%s\n", ((struct phrase *)a->l)->c);
    }else if(strcmp(a->nodetype, "B")==0){
        printf("**%s**\n", ((struct phrase *)a->l)->c);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "U")==0){
        printf("<u>%s</u>\n", ((struct phrase *)a->l)->c);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "I")==0){
        printf("*%s*\n", ((struct phrase *)a->l)->c);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "PH")==0){
        if(a->l!=NULL)
            printf("%s\n", ((struct phrase *)a->l)->c);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "Co")==0){
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "L")==0){
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "E")==0
        || strcmp(a->nodetype, "IT")==0){
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "ITEM")==0){
        if(a->l!=NULL)
            printf("-%s\n", ((struct phrase *)a->l)->c);
    }else if(strcmp(a->nodetype, "ITEME")==0){
        if(a->l!=NULL)
            printf("1.%s\n", ((struct phrase *)a->l)->c);
    }
    
}

void yyerror(char *s, ...){
    va_list ap;
    va_start(ap,s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

void catstr(char **s1, char *s2){
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
    *s1 = (char *)realloc(*s1, i+j+1);
}

int main(int argc, char *argv[]){
    if(argc == 1) return 0;
    int i,res;
    for(i=1;i<argc;i+=2){
        FILE *input = fopen(argv[i], "r");
        FILE *output = fopen(argv[i+1], "w");

        if(!input){
            yyerror("Error in file opening");
            continue;
        }

        yyin = input;
        res = yyparse();
        if(!res){
            fclose(input);
            fclose(output);
        }else{
            fclose(input);
            fclose(output);
            remove(argv[i+1]);
        }
        
    }
    
}