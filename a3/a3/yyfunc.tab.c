#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20110908

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)


#ifndef yyparse
#define yyparse    yyfuncparse
#endif /* yyparse */

#ifndef yylex
#define yylex      yyfunclex
#endif /* yylex */

#ifndef yyerror
#define yyerror    yyfuncerror
#endif /* yyerror */

#ifndef yychar
#define yychar     yyfuncchar
#endif /* yychar */

#ifndef yyval
#define yyval      yyfuncval
#endif /* yyval */

#ifndef yylval
#define yylval     yyfunclval
#endif /* yylval */

#ifndef yydebug
#define yydebug    yyfuncdebug
#endif /* yydebug */

#ifndef yynerrs
#define yynerrs    yyfuncnerrs
#endif /* yynerrs */

#ifndef yyerrflag
#define yyerrflag  yyfuncerrflag
#endif /* yyerrflag */

#ifndef yylhs
#define yylhs      yyfunclhs
#endif /* yylhs */

#ifndef yylen
#define yylen      yyfunclen
#endif /* yylen */

#ifndef yydefred
#define yydefred   yyfuncdefred
#endif /* yydefred */

#ifndef yydgoto
#define yydgoto    yyfuncdgoto
#endif /* yydgoto */

#ifndef yysindex
#define yysindex   yyfuncsindex
#endif /* yysindex */

#ifndef yyrindex
#define yyrindex   yyfuncrindex
#endif /* yyrindex */

#ifndef yygindex
#define yygindex   yyfuncgindex
#endif /* yygindex */

#ifndef yytable
#define yytable    yyfunctable
#endif /* yytable */

#ifndef yycheck
#define yycheck    yyfunccheck
#endif /* yycheck */

#ifndef yyname
#define yyname     yyfuncname
#endif /* yyname */

#ifndef yyrule
#define yyrule     yyfuncrule
#endif /* yyrule */
#define YYPREFIX "yyfunc"

#define YYPURE 0

#line 3 "ParserFunc.y"

        #include "ParseTree.h" 
        #include <stdio.h>
        #include <string.h>
        #include <stdlib.h>
        #include <iostream>

        extern "C" int yyfunclex();
        extern "C" int yyfuncparse();
        extern "C" void yyfuncerror(char *s);
  
        /* this is the final parse tree that is returned        */
        struct FuncOperator *finalfunc; 

#line 20 "ParserFunc.y"
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
#line 128 "yyfunc.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#define YYERROR_DECL() yyerror(const char *s)
#define YYERROR_CALL(msg) yyerror(msg)

extern int YYPARSE_DECL();

#define Name 257
#define Float 258
#define Int 259
#define YYERRCODE 256
static const short yyfunclhs[] = {                       -1,
    0,    0,    0,    0,    0,    2,    2,    2,    2,    1,
    1,    1,
};
static const short yyfunclen[] = {                        2,
    3,    5,    3,    1,    2,    1,    1,    1,    1,    1,
    1,    1,
};
static const short yyfuncdefred[] = {                     0,
   12,   10,   11,    0,    0,    0,    0,    0,    5,    6,
    7,    8,    9,    0,    0,    1,    0,    2,
};
static const short yyfuncdgoto[] = {                      6,
    7,   14,
};
static const short yyfuncsindex[] = {                   -40,
    0,    0,    0,  -40,  -40,    0,  -36,  -33,    0,    0,
    0,    0,    0,  -40,  -36,    0,  -40,    0,
};
static const short yyfuncrindex[] = {                     0,
    0,    0,    0,    0,    0,    0,    1,    0,    0,    0,
    0,    0,    0,    0,    2,    0,    0,    0,
};
static const short yyfuncgindex[] = {                    -1,
    0,   -5,
};
#define YYTABLESIZE 219
static const short yyfunctable[] = {                      4,
    4,    3,    8,    9,    5,   12,   11,   15,   10,   17,
   13,    0,   16,    0,    0,   18,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    4,    3,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    1,    2,    3,
};
static const short yyfunccheck[] = {                     40,
    0,    0,    4,    5,   45,   42,   43,   41,   45,   15,
   47,   -1,   14,   -1,   -1,   17,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   41,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  257,  258,  259,
};
#define YYFINAL 6
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 259
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'","'+'",0,"'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"Name",
"Float","Int",
};
static const char *yyrule[] = {
"$accept : CompoundExp",
"CompoundExp : SimpleExp Op CompoundExp",
"CompoundExp : '(' CompoundExp ')' Op CompoundExp",
"CompoundExp : '(' CompoundExp ')'",
"CompoundExp : SimpleExp",
"CompoundExp : '-' CompoundExp",
"Op : '-'",
"Op : '+'",
"Op : '*'",
"Op : '/'",
"SimpleExp : Float",
"SimpleExp : Int",
"SimpleExp : Name",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 151 "ParserFunc.y"


#line 307 "yyfunc.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = data->s_mark - data->s_base;
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 48 "ParserFunc.y"
	{
        yyval.myOperator = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));     
        yyval.myOperator->leftOperator = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));
        yyval.myOperator->leftOperator->leftOperator = NULL;
        yyval.myOperator->leftOperator->leftOperand = yystack.l_mark[-2].myOperand;
        yyval.myOperator->leftOperator->right = NULL;
        yyval.myOperator->leftOperand = NULL;
        yyval.myOperator->right = yystack.l_mark[0].myOperator;
        yyval.myOperator->code = yystack.l_mark[-1].whichOne;  

        finalfunc = yyval.myOperator;
}
break;
case 2:
#line 62 "ParserFunc.y"
	{
        yyval.myOperator = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));     
        yyval.myOperator->leftOperator = yystack.l_mark[-3].myOperator;
        yyval.myOperator->leftOperand = NULL;
        yyval.myOperator->right = yystack.l_mark[0].myOperator;
        yyval.myOperator->code = yystack.l_mark[-1].whichOne;  

        finalfunc = yyval.myOperator;
}
break;
case 3:
#line 73 "ParserFunc.y"
	{
        yyval.myOperator = yystack.l_mark[-1].myOperator;

        finalfunc = yyval.myOperator;
}
break;
case 4:
#line 80 "ParserFunc.y"
	{
        yyval.myOperator = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));     
        yyval.myOperator->leftOperator = NULL;
        yyval.myOperator->leftOperand = yystack.l_mark[0].myOperand;
        yyval.myOperator->right = NULL;       

        finalfunc = yyval.myOperator;
}
break;
case 5:
#line 90 "ParserFunc.y"
	{
        yyval.myOperator = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));     
        yyval.myOperator->leftOperator = yystack.l_mark[0].myOperator;
        yyval.myOperator->leftOperand = NULL;
        yyval.myOperator->right = NULL;       
        yyval.myOperator->code = '-';

        finalfunc = yyval.myOperator;
}
break;
case 6:
#line 102 "ParserFunc.y"
	{
        yyval.whichOne = '-';
}
break;
case 7:
#line 107 "ParserFunc.y"
	{
        yyval.whichOne = '+';
}
break;
case 8:
#line 112 "ParserFunc.y"
	{
        yyval.whichOne = '*';
}
break;
case 9:
#line 117 "ParserFunc.y"
	{
        yyval.whichOne = '/';
}
break;
case 10:
#line 126 "ParserFunc.y"
	{
        /* construct and send up the operand containing the FP number*/
        yyval.myOperand = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        yyval.myOperand->code = DOUBLE;
        yyval.myOperand->value = yystack.l_mark[0].actualChars;
}
break;
case 11:
#line 134 "ParserFunc.y"
	{
        /* construct and send up the operand containing the integer*/
        yyval.myOperand = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        yyval.myOperand->code = INT;
        yyval.myOperand->value = yystack.l_mark[0].actualChars;
}
break;
case 12:
#line 142 "ParserFunc.y"
	{
        /* construct and send up the operand containing the name*/
        yyval.myOperand = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        yyval.myOperand->code = NAME;
        yyval.myOperand->value = yystack.l_mark[0].actualChars;
}
break;
#line 622 "yyfunc.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
