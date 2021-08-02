/*
*   Latex2Markdown Lexer
*/
%option noyywrap nodefault yylineno
%{
#include "header.h"
#include "parser.tab.h"
%}

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
"\\chapter" { return CHAPTER; }
"\\section" { return SECTION; }
"\\subsection" { return SUBSECTION; }
"\\subsubsection" { return SUBSUBSECTION; }
"\\paragraph" { return PARAGRAPH; }
"\\bf" { return BF; }
"\\underline" { return UNDERLINE; }
"\\it" { return IT; }
"\\begin{enumerate}" { return ENUMBEGIN; }
"\\end{enumerate}" { return ENUMEND; }
"\\item" { return ITEM; }
"\\begin{itemize}" { return ITEMBEGIN; }
"\\end{itemize}" { return ITEMEND; }
"\\maketitle" { return MAKETITLE; }

"%".* {/* comment */}
" "|"."|"," { yylval.c = strdup(yytext); return ACCE; }
[a-zA-Z0-9]+   { yylval.c = strdup(yytext); return WORD; }
[ \t] {}
\n 
. 
%%