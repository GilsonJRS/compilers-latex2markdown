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
%token <c> WORD ACCE EOL

%token DOCCLASS PACKAGE TITLE AUTHOR DOCBEGIN DOCEND CHAPTER SECTION
%token SUBSECTION SUBSUBSECTION PARAGRAPH BF UNDERLINE IT ENUMBEGIN
%token ENUMEND ITEM ITEMBEGIN ITEMEND MAKETITLE

%type <c> phrase
%type <a> conf id main body content lists enum item itemE itemize latexDoc sectionC

%start latexDoc
%%

latexDoc: DOCCLASS conf id main { $$ = newast("Begin", $2, newast("Main",$3,$4)); writeOutput($$);treefree($$); }
    ;

conf:{ $$ = NULL; } 
    |'[' phrase ']' conf { $$ = newast("Opts", newPhrase($2), $4); }
    |'{' WORD '}' conf { $$ = newast("Class", newPhrase($2), $4); }
    | PACKAGE '{' phrase '}' conf{ $$ = newast("PK", newPhrase($3), $5); }
    ;

id: TITLE '{' phrase '}' id { $$ = newast("Title", newPhrase($3), $5); }
    | AUTHOR '{' phrase '}'{ $$ = newast("Author", newPhrase($3), NULL); }
    ;

main: DOCBEGIN MAKETITLE body DOCEND { $$ = newast("Body", NULL, $3); }
    | DOCBEGIN body DOCEND { $$ = newast("Body", NULL, $2); }
    ;

body: { $$ = NULL; }
    | CHAPTER sectionC body{ $$ = newast("C", $2, $3); }
    | SECTION sectionC body{ $$ = newast("S", $2, $3); }
    | SUBSECTION sectionC body{ $$ = newast("SB", $2, $3); }
    | SUBSUBSECTION sectionC body{ $$ = newast("SSB", $2, $3); }
    | PARAGRAPH sectionC body{ $$ = newast("PG", $2, $3); }
    | content { $$ = newast("Co", NULL, $1); }
    ;

sectionC: { $$ = NULL; } 
    | '{' '}' { $$ = NULL; }
    | '{' phrase '}' { $$ = newast("CC", newPhrase($2), NULL); } 
    ;

content: BF '{' phrase '}' body { $$ = newast("B", newPhrase($3), $5); }
    | UNDERLINE '{' phrase '}' body { $$ = newast("U", newPhrase($3), $5); }
    | IT '{' phrase '}' body { $$ = newast("I", newPhrase($3), $5); }
    | phrase body { $$ = newast("PH", newPhrase($1), $2); }
    | lists body { $$ = newast("L", $1, $2); }
    ;

lists: ENUMBEGIN enum ENUMEND { $$ = newast("ENUM", $2, NULL); }
    | ITEMBEGIN itemize ITEMEND { $$ = newast("ITEMI", $2, NULL); }
    ;

enum: itemE { $$ = newast("E", $1, NULL); }
    | itemE enum { $$ = newast("E", $1, $2); }
    | lists { $$ = newast("E", $1, NULL); }
    ;

itemE:{ $$ = NULL; }
    | ITEM '{' phrase '}' { $$ = newast("ITEME", newPhrase($3), NULL); }
    | ITEM phrase { $$ = newast("ITEME", newPhrase($2), NULL);}
    ;

itemize: item { $$ = newast("IT", $1, NULL); }
    | item itemize { $$ = newast("IT", $1, $2); }
    | lists { $$ = newast("IT", $1, NULL); }
    ;

item: { $$ = NULL; }
    | ITEM '{' phrase '}' { $$ = newast("ITEM", newPhrase($3), NULL); }
    | ITEM phrase { $$ = newast("ITEM", newPhrase($2), NULL);}
    ;

phrase: WORD phrase { catstr(&$1, $2); $$ = strcat($1, $2); $$ = $1; }
    | ACCE phrase { catstr(&$1, $2); $$ = strcat($1, $2); $$ = $1; }
    | EOL phrase { catstr(&$1, $2); $$ = strcat($1, $2); $$ = $1; }
    | WORD 
    | ACCE
    | EOL
    ;

