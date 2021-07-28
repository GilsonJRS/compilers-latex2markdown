/*
*   Advanced Calculator declarations
*/

/* Lexer interface */
extern int yylineno;
void yyerror(char *s, ...);

/* symbol table */
struct symbol{
    char *name; /* variable name */
    double value;
    struct ast *func; /* function stmt */
    struct symlist *syms; /* argument list */
};

/* fixed size symbol table */
#define NHASH 9997
struct symbol symtab[NHASH];

struct symbol *lookup(char *name);

/* symbol list for argument list */
struct symlist{
    struct symbol *sym;
    struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* node types 
* + - * /
* 0-7 comparison operator, 04 equal, 02 less than, 01 bigger than
* L expression or command list
* I IF command
* W WHILE command
* N reference symbol
* = assignment
* S symbol list
* F pre-defined function call
* C user defined function call
*/

enum bifs{ /* Pre defined functions */
    B_sqrt = 1,
    B_exp,
    B_log,
    B_print
};

/* AST nodes */

struct ast{
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct fncall{ /* Pre defined function*/
    int nodetype; /* F type */
    struct ast *l;
    enum bifs functype;
};

struct ufncall{ /* User defined function */
    int nodetype; /* C type */
    struct ast *l; /* argument list */
    struct symbol *s;
};

struct flow{
    int nodetype; /* W or I type */
    struct ast *cond; /* condition */
    struct ast *tl; /* then branch or do list */
    struct ast *el; /* optional else branch */
};

struct numval{
    int nodetype; /* K type */
    double number;
};

struct symref{
    int nodetype; /* N type */
    struct symbol *s;
};

struct symasgn{
    int nodetype; /* = type*/
    struct symbol *s;
    struct ast *v; /* assignment value */
};

/* AST contruction */

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

/* function definition */
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);

/* AST avaliation */
double eval(struct ast *);

/* Free AST */
void treefree(struct ast *);

extern int yylineno;
void yyerror(char *s, ...);