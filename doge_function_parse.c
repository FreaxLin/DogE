#include <stdio.h>
#include "mpc.h"



int main(int argc, char **argv) {
    
    
    mpc_parser_t* Ident     = mpc_new("ident");
    mpc_parser_t* Number    = mpc_new("number");
    mpc_parser_t* Character = mpc_new("character");
    mpc_parser_t* String    = mpc_new("string");
    mpc_parser_t* Typeident = mpc_new("typeident");
    mpc_parser_t* Assign    = mpc_new("assign");
    mpc_parser_t* Args      = mpc_new("args");
    mpc_parser_t* Field     = mpc_new("field");
    mpc_parser_t* Invoke    = mpc_new("invoke");
    mpc_parser_t* Return    = mpc_new("return");
    mpc_parser_t* Function  = mpc_new("function");

    mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
        " ident     : /[a-zA-Z_][a-zA-Z0-9_]*/ ;                                        \n"
        " number    : /[0-9]+/ ;                                                        \n"
        " character : /'.'/ ;                                                           \n"
        " string    : /\"(\\\\.|[^\"])*\"/ ;                                            \n"
        " typeident : (\"int\" | \"char\") <ident> ;                                    \n"
        " invoke    : <ident> '(' (<ident>|<number>|<character>)* ')'      ;   \n"
        " assign    : (\"int\" | \"char\")? <ident> '=' (<character>|<number>|<invoke>|<ident>)';';                  \n"
        " args      : <typeident>? (',' <typeident>)* ;                                 \n"
        " field     : <typeident> ';'  ;                                                \n"
        " return    : \"return\" (<number> | <ident> | <character>) ';';                   \n" 
        " function  : (\"int\" | \"char\") <ident> '(' <args> ')' '{' (<field> | <assign>|<invoke>)* <return>'}' ;           \n",
        Ident, Number, Character, String, Typeident, Invoke, Assign, Args, Field, Return, Function, NULL);
  
    if (err != NULL) {
        mpc_err_print(err);
        mpc_err_delete(err);
        exit(1);
    }
    
    FILE* fp = fopen("function.doge","rb");
    mpc_result_t r;
    if (mpc_parse_file("doge", fp, Function, &r)) {
        mpc_ast_print(r.output);
        mpc_ast_delete(r.output);
    } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }
  
    mpc_cleanup(8, Ident, Number, Character, String, Typeident, Args, Field, Function);
    fclose(fp);
    return 0;
  
}