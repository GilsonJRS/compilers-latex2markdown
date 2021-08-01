/*
*   Latex2Markdown Header
*/

/* Lexer interface */
extern int yylineno;
extern FILE *yyin;
extern int yyleng;
void yyerror(char *s, ...);
int yylex();
int yyparse();

/* node types 
* C chapter
* S section
* SB subsection
* SSB subsubsection
* PG paragraph
* CC subs content
* P phrase
* B Bold
* U underline
* I italic
* L lists
* E enumerate
* IT itemize
* ITEM item
* PK package
*/

/* AST nodes */

struct ast{
    char *nodetype;
    struct ast *l;
    struct ast *r;
};

struct phrase{
    char *nodetype;
    char *c;
};


/* AST contruction */

struct ast *newast(char *nodetype, struct ast *l, struct ast *r);
struct ast *newPhrase(char *c);


/* AST avaliation */
void writeOutput(struct ast *);

/* Free AST */
void treefree(struct ast *);

extern int yylineno;
void yyerror(char *s, ...);

void catstr(char **s1, char *s2);
