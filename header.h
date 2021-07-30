/*
*   Latex2Markdown Header
*/

/* Lexer interface */
extern int yylineno;
void yyerror(char *s, ...);

/* node types 
* C chapter
* S section
* SB subsection
* SSB subsubsection
* PG paragraph
* P phrase
*/

/* AST nodes */

struct ast{
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct phrase{
    int nodetype; 
    char *c;
};


/* AST contruction */

struct ast *newast(int nodetype, struct ast *l, struct ast *r);

/* AST avaliation */
double eval(struct ast *);

/* Free AST */
void treefree(struct ast *);

extern int yylineno;
void yyerror(char *s, ...);