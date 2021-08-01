/*
*   Latex2Markdown Parser
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
%}

%union{
    struct ast *a;
    char *c;
}

/* token declaration */
%token <c> WORD ACCE

%token DOCCLASS PACKAGE TITLE AUTHOR DOCBEGIN DOCEND CHAPTER SECTION
%token SUBSECTION SUBSUBSECTION PARAGRAPH BF UNDERLINE IT ENUMBEGIN
%token ENUMEND ITEM ITEMBEGIN ITEMEND MAKETITLE

%type <c> phrase
%type <a> conf id main body content lists enum item itemize latexDoc

%start latexDoc
%%

latexDoc: DOCCLASS conf id main { $$ = newast("Begin", $2, newast("Main",$3,$4)); writeOutput($$);treefree($$); }
    ;

conf:{ $$ = NULL; } 
    |'[' phrase ']' conf { $$ = newast("Opts", newPhrase($2), $4); }
    |'{' WORD '}' conf { $$ = newast("Class", newPhrase($2), $4); }
    | PACKAGE '{' phrase '}' conf { $$ = newast("PK", newPhrase($3), $5); }
    ;

id: TITLE '{' phrase '}' id { $$ = newast("Title", newPhrase($3), $5); }
    | AUTHOR '{' phrase '}'{ $$ = newast("Author", newPhrase($3), NULL); }
    ;

main: DOCBEGIN MAKETITLE body DOCEND { $$ = newast("Body", NULL, $3); }
    | DOCBEGIN body DOCEND { printf("asdf"); $$ = newast("Body", NULL, $2); }
    ;

body: { $$ = NULL; }
    | CHAPTER content body{ $$ = newast("C", $2, $3); }
    | SECTION content body{ $$ = newast("S", $2, $3); }
    | SUBSECTION content body{ $$ = newast("SB", $2, $3); }
    | SUBSUBSECTION content body{ $$ = newast("SSB", $2, $3); }
    | PARAGRAPH content body{ $$ = newast("PG", $2, $3); }
    | content { $$ = newast("Co", $1, NULL); }
    ;

content: { $$ = NULL; } 
    |'{' phrase '}' content { $$ = newast("CC", newPhrase($2), $4); }
    | BF '{' phrase '}' content { $$ = newast("B", newPhrase($3), $5); }
    | UNDERLINE '{' phrase '}' content { $$ = newast("U", newPhrase($3), $5); }
    | IT '{' phrase '}' content { $$ = newast("I", newPhrase($3), $5); }
    | phrase content { $$ = newast("P", newPhrase($1), $2); }
    | lists content { $$ = newast("L", $1, $2); }
    ;

lists: ENUMBEGIN enum ENUMEND { $$ = newast("E", $2, NULL); }
    | ITEMBEGIN itemize ITEMEND { $$ = newast("IT", $2, NULL); }
    ;

enum: item { $$ = newast("E", $1, NULL); }
    | item enum { $$ = newast("E", $1, $2); }
    | lists { $$ = newast("E", $1, NULL); }
    ;

itemize: item { $$ = newast("IT", $1, NULL); }
    | item enum { $$ = newast("IT", $1, $2); }
    | lists { $$ = newast("IT", $1, NULL); }
    ;

item: { $$ = NULL; }
    | ITEM '{' phrase '}' { $$ = newast("ITEM", newPhrase($3), NULL); }
    | ITEM phrase { $$ = newast("ITEM", newPhrase($2), NULL);}
    ;

phrase: WORD phrase { $$ = strcat($1, $2); $$ = $1; }
    | ACCE phrase { $$ = strcat($1, $2); $$ = $1; }
    | WORD 
    | ACCE 
    ;

