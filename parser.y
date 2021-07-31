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
%type <a> latexDocument conf id main body content lists enum item itemize

%start latexDocument
%%

latexDocument:
    | DOCCLASS conf id main { writeOutput(newast("Begin", $2, newast("Conf",$3,$4))); }
    ;

conf: /* nothing */
    | '[' phrase ']' conf { $$ = newast("Opts", newPhrase($2), $4); }
    | '{' WORD '}' conf { $$ = newast("Class", $2, $4); }
    | PACKAGE '{' phrase '}' conf { $$ = newast("PK", newPhrase($3), $5); }
    ;

id: TITLE '{' phrase '}' id { $$ = newast("Title", newPhrase($3), $5); }
    | AUTHOR '{' phrase '}' { $$ = newast("Author", newPhrase($3), NULL); }
    ;

main: DOCBEGIN body DOCEND { $$ = newast("Body", $2, NULL); }

body: /* nothing */
    | CHAPTER content body{ $$ = newast('C', $2, $3); }
    | SECTION content body{ $$ = newast('S', $2, $3); }
    | SUBSECTION content body{ $$ = newast('SB', $2, $3); }
    | SUBSUBSECTION content body{ $$ = newast('SSB', $2, $3); }
    | PARAGRAPH content body{ $$ = newast('PG', $2, $3); }
    | content { $$ = newast('P', $1, NULL); }
    ;

content: /* nothing */ 
    | '{' phrase '}' content { $$ = newast('CC', newPhrase($2), $4); }
    | BF '{' phrase '}' content { $$ = newast('B', newPhrase($3), $5); }
    | UNDERLINE '{' phrase '}' content { $$ = newast('U', newPhrase($3), $5); }
    | IT '{' phrase '}' content { $$ = newast('I', newPhrase($3), $5); }
    | phrase content { $$ = newast('P', newPhrase($1), $2); }
    | lists content { $$ = newast('L', $1, $2); }
    | body
    ;

lists: ENUMBEGIN enum ENUMEND { $$ = newast('E', $2, NULL); }
    | ITEMBEGIN itemize ITEMEND { $$ = newast('IT', $2, NULL); }
    ;

enum: item { $$ = newast('E', $2, NULL); }
    | item enum { $$ = newast('E', $1, $2); }
    | lists { $$ = newast('E', $2, NULL); }
    ;

itemize: item { $$ = newast('IT', $2, NULL); }
    | item enum { $$ = newast('IT', $1, $2); }
    | lists 
    ;

item: 
    | ITEM '{' phrase '}' { $$ = newast('E', newPhrase($3), NULL); }
    | ITEM phrase { $$ = newast('E', newPhrase($2), NULL);}
    ;

phrase: 
    | WORD phrase { strcat($1, $2); $$ = $1; }
    | ACCE phrase { strcat($1, $2); $$ = $1; }
    | ACCE
    | WORD
    ;

