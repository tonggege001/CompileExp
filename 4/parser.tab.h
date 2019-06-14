
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
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
     T_pp = 296,
     T_ss = 297,
     T_cst_int = 298,
     T_cst_float = 299,
     T_cst_char = 300,
     T_ls = 301,
     T_rs = 302,
     T_lb = 303,
     T_rb = 304,
     T_lm = 305,
     T_rm = 306,
     T_semi = 307,
     T_comma = 308
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 17 "parser.y"

	int type_int;
	float type_float;
	char type_id[32];
	struct node * ptr;



/* Line 1676 of yacc.c  */
#line 114 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;

