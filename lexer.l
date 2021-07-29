/*
*   Latex2Markdown Parser
*/
%option noyywrap nodefault yylineno
%{
#include "header.h"
#include "parser.tab.h"
%}

/* float expoent */
EXP ([Ee][-+]?[0-9]+)

%%
"[" |
"]" |
"{" |
"}" { return yytext[0]; }

"\\documentclass" { return DOCCLASS; }
"\\usepackage" { return PACKAGE; }
"\\title" { return TITLE; }
"\\author" { return AUTHOR; }
"\\begin{document}" { return DOCBEGIN; }
"\\end{document}" { return DOCEND; }

[a-zA-Z][a-zA-Z0-9]* { yylval.s = lookup(yytext); return NAME; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? {yylval.d = atof(yytext); return NUMBER; }

"//".*
[ \t]

\\\n { printf("c> "); }

\n { return EOL; }

. { yyerror("Caracter desconhecido %c\n", *yytext); }

%%
