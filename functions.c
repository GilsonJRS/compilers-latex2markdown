/*
*   Advanced Calculator auxiliary functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "header.h"
int LL;

//creates new ast subtree
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

//create new phrase node 
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

//free ast
void treefree(struct ast *a){
    if(a==NULL) return;
    if(strcmp(a->nodetype, "Begin")==0
        || strcmp(a->nodetype, "Main")==0
        || strcmp(a->nodetype, "C")==0
        || strcmp(a->nodetype, "S")==0
        || strcmp(a->nodetype, "SB")==0
        || strcmp(a->nodetype, "SSB")==0
        || strcmp(a->nodetype, "PG")==0
        || strcmp(a->nodetype, "IT")==0
        || strcmp(a->nodetype, "E")==0
        || strcmp(a->nodetype, "ENUM")==0
        || strcmp(a->nodetype, "ITEMI")==0
        || strcmp(a->nodetype, "L")==0){
            if(a->l!=NULL)
                treefree(a->l);
            if(a->r!=NULL)
                treefree(a->r);
    }
    else if(strcmp(a->nodetype, "Body")==0
        || strcmp(a->nodetype, "Co")==0){
        if(a->r!=NULL)
            treefree(a->r);
    }else if(strcmp(a->nodetype, "Class")==0
        || strcmp(a->nodetype, "Opts")==0
        || strcmp(a->nodetype, "PK")==0
        || strcmp(a->nodetype, "B")==0
        || strcmp(a->nodetype, "U")==0
        || strcmp(a->nodetype, "I")==0
        || strcmp(a->nodetype, "PH")==0){
        free(((struct phrase *)a->l)->c);
        free(a->l);
        if(a->r!=NULL)
            treefree(a->r);
    }else if(strcmp(a->nodetype, "CC")==0
        || strcmp(a->nodetype, "ITEME")==0
        || strcmp(a->nodetype, "ITEM")==0){
        free(((struct phrase *)a->l)->c);
        free(a->l);
    }
    free(a);
}

//write output markdown file
void writeOutput(struct ast *a){
    if(a==NULL) return;
    if(strcmp(a->nodetype, "Begin")==0
        || strcmp(a->nodetype, "Main")==0){
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }
    else if(strcmp(a->nodetype, "Body")==0
        || strcmp(a->nodetype, "Co")==0){
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "Class")==0
        || strcmp(a->nodetype, "Opts")==0
        || strcmp(a->nodetype, "PK")==0){
        fprintf(yyout,"%s : %s \n", a->nodetype, ((struct phrase *)a->l)->c);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "Title")==0
        || strcmp(a->nodetype, "Author")==0
        ){
        char *s = {strcmp(a->nodetype, "Title")==0 ? "# " : "#### "};
        fprintf(yyout,"%s %s: %s\n", s, a->nodetype, ((struct phrase *)a->l)->c);
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "C")==0){
        fprintf(yyout,"## ");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "S")==0){
        fprintf(yyout,"### ");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "SB")==0){
        fprintf(yyout,"#### ");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "SSB")==0){
        fprintf(yyout,"##### ");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "PG")==0){
        fprintf(yyout,"###### ");
        if(a->l!=NULL)
            writeOutput(a->l);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "CC")==0){
        fprintf(yyout,"%s\n", ((struct phrase *)a->l)->c);
    }else if(strcmp(a->nodetype, "B")==0){
        fprintf(yyout,"**%s**\n", ((struct phrase *)a->l)->c);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "U")==0){
        fprintf(yyout,"<u>%s</u>\n", ((struct phrase *)a->l)->c);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "I")==0){
        fprintf(yyout,"*%s*\n", ((struct phrase *)a->l)->c);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "PH")==0){
        if(a->l!=NULL)
            fprintf(yyout,"%s\n\n", ((struct phrase *)a->l)->c);
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "L")==0){
        LL=-1;
        fprintf(yyout,"\n");
        if(a->l!=NULL)
            writeOutput(a->l);
        fprintf(yyout,"\n");
        if(a->r!=NULL)
            writeOutput(a->r);
    }else if(strcmp(a->nodetype, "ENUM")==0
        || strcmp(a->nodetype, "ITEMI")==0){
            LL++;
            if(a->l!=NULL)
                writeOutput(a->l);
            LL--;
    }else if(strcmp(a->nodetype, "E")==0
        || strcmp(a->nodetype, "IT")==0){
            if(a->l!=NULL)
                writeOutput(a->l);
            if(a->r!=NULL)
                writeOutput(a->r);/*
            else
                fprintf(yyout,"\n");*/
    }else if(strcmp(a->nodetype, "ITEM")==0){
        char *s;
        if(LL > 0){
            s = (char *)calloc(LL,sizeof(char));
            int i = 0;
            for(i;i<LL;i++)
                strcat(s, "\t");
        }else{
            s = (char *)calloc(1,1);
        }
        if(a->l!=NULL)
            fprintf(yyout,"%s-%s\n", s,((struct phrase *)a->l)->c);
        free(s);
    }else if(strcmp(a->nodetype, "ITEME")==0){
        char *s;
        if(LL > 0){
            s = (char *)calloc(LL, sizeof(char));
            int i = 0;
            for(i;i<LL;i++)
                strcat(s, "\t");
        }else{
            s = (char *)calloc(1,1);
        }
        if(a->l!=NULL)
            fprintf(yyout,"%s1.%s\n", s, ((struct phrase *)a->l)->c);
        free(s);
    }
}

//to print erros
void yyerror(char *s, ...){
    va_list ap;
    va_start(ap,s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

//realloc space for strings
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
        yyout = output;
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