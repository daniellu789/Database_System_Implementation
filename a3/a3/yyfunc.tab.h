#define Name 257
#define Float 258
#define Int 259
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
        struct FuncOperand *myOperand;
        struct FuncOperator *myOperator; 
        char *actualChars;
        char whichOne;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yyfunclval;
