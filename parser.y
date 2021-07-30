/*
*   Latex2Markdown Parser
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
%}

%union{
    struct ast *a;
    char *c;
}

/* token declaration */
%token <c> WORD ACCE
%token EOL

%token DOCCLASS PACKAGE TITLE AUTHOR DOCBEGIN DOCEND CHAPTER SECTION
%token SUBSECTION SUBSUBSECTION PARAGRAPH BF UNDERLINE IT ENUMBEGIN
%token ENUMEND ITEM ITEMBEGIN ITEMEND

%type <c> phrase 
%type <a> latexDocument conf id main body

%start latexDocument
%%

latexDocument:
    | DOCCLASS conf id main { $$ = newast('Begin', $2, newast('Conf',$3,$4)); }
    ;

conf: 
    | '[' phrase ']' conf { $$ = newast('Conf', $2, $4); }
    | '{' WORD '}' conf { $$ = newast('Conf', $2, $4); }
    | PACKAGE '{' phrase '}' conf { $$ = newast('Conf', $3, $5); }
    ;

id: TITLE '{' phrase '}' id { $$ = newast('Conf', $3, $5); }
    | AUTHOR '{' phrase '}' { $$ = newast('Conf', $3, NULL); }
    ;

main: DOCBEGIN body DOCEND { $$ = newast('', $2, NULL); }

body:
    | CHAPTER chapter body{ $$ = newast('C', $2, $3); }
    | SECTION section body{ $$ = newast('S', $2, $3); }
    | SUBSECTION subsection body{ $$ = newast('SB', $2, $3); }
    | SUBSUBSECTION subsubsection body{ $$ = newast('SSB', $2, $3); }
    | PARAGRAPH paragraph body{ $$ = newast('PG', $2, $3); }
    | phrase body { $$ = newast('P', $1, $2); }
    | phrase { $$ = newast('P', $1, NULL); }
    ;

chapter: '{' phrase '}' {}
    | '{' phrase '}' phrase {}
    ;

section: '{' phrase '}' {}
    | SECTION '{' phrase '}' phrase {}
    ;

subsection:'{' phrase '}' {}
    | SUBSECTION '{' phrase '}' phrase {}
    ;

subsubsection: '{' phrase '}' {}
    | SUBSUBSECTION '{' phrase '}' phrase {}
    ;

paragraph: '{' phrase '}' {}
    | PARAGRAPH '{' phrase '}' phrase {}
    ;

phrase: WORD phrase { strcat($1, $2);  $$ = $1; }
    | ACCE phrase {}
    | ACCE
    | WORD
    ;

