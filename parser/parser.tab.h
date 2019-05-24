/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_int = 258,
    T_char = 259,
    T_float = 260,
    T_string = 261,
    T_intArr = 262,
    T_floatArr = 263,
    T_identifier = 264,
    T_func = 265,
    T_alloc = 266,
    T_free = 267,
    T_if = 268,
    T_else = 269,
    T_while = 270,
    T_break = 271,
    T_continue = 272,
    T_return = 273,
    T_getarr = 274,
    T_not = 275,
    T_lnot = 276,
    T_div = 277,
    T_mul = 278,
    T_mod = 279,
    T_sub = 280,
    T_add = 281,
    T_leftmove = 282,
    T_rightmove = 283,
    T_big = 284,
    T_bige = 285,
    T_small = 286,
    T_smalle = 287,
    T_equal = 288,
    T_nequal = 289,
    T_land = 290,
    T_lxor = 291,
    T_lor = 292,
    T_and = 293,
    T_or = 294,
    T_assign = 295,
    T_cst_int = 296,
    T_cst_float = 297,
    T_cst_char = 298,
    T_ls = 299,
    T_rs = 300,
    T_lb = 301,
    T_rb = 302,
    T_semi = 303,
    T_comma = 304
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 21 "parser.y" /* yacc.c:1909  */

	int type_int;
	float type_float;
	char type_id[32];
	struct node * ptr;

#line 111 "parser.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
