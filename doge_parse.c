#include "mpc.h"

int main(int argc, char **argv) {
    mpc_parser_t* Ident     = mpc_new("ident");
    mpc_parser_t* Number    = mpc_new("number");
    mpc_parser_t* Character = mpc_new("character");
    mpc_parser_t* String    = mpc_new("string");
    mpc_parser_t* Path = mpc_new("path");
    mpc_parser_t* Package = mpc_new("package");
    mpc_parser_t* Class = mpc_new("class");
    mpc_parser_t* Typeident = mpc_new("typeident");
    mpc_parser_t* Args = mpc_new("args");
    mpc_parser_t* Field = mpc_new("field");
    mpc_parser_t* Function = mpc_new("function");
    mpc_parser_t* Body = mpc_new("body");
    mpc_parser_t* Doge = mpc_new("doge");
    

    mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
        " ident     : /[a-zA-Z_][a-zA-Z0-9_]*/ ;                                        \n"
        " number    : /[0-9]+/ ;                                                        \n"
        " character : /'.'/ ;                                                           \n"
        " string    : /\"(\\\\.|[^\"])*\"/ ;                                            \n"
        " path      : /[a-z]*/ ;                                                        \n"
        " package   : \"package\" <path>('.'<path>)* ;                                  \n"
        " class     : \"class\" <ident>;                                                \n"
        " typeident : (\"int\" | \"char\") <ident> ;                                    \n"
        " args      : <typeident>? (',' <typeident>)* ;                                 \n"
        " field     : <typeident> ';'            ;                                      \n"
        " function  : (\"int\" | \"char\") <ident> '(' <args> ')' '{'  '}' ;            \n"
        " body      : '{' (<field> | <function>)*'}';                                   \n"
        " doge      : /^/<package> <class> <body>/$/ ;                                  \n",
        Ident, Number, Character, String, Path, Package, Class, Typeident, Args, Field, Function, Body, Doge, NULL);
  
    if (err != NULL) {
        mpc_err_print(err);
        mpc_err_delete(err);
        exit(1);
    }
    
    FILE *fp;
    fp = fopen("Numberc.doge","rb");
    mpc_result_t r;
    if (mpc_parse_file("doge", fp, Doge, &r)) {
        mpc_ast_print(r.output);
        mpc_ast_delete(r.output);
    } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }
  
    mpc_cleanup(13, Ident, Number, Character, String, Path, Package, Class, Typeident, Args, Field, Function, Body, Doge);
    fclose(fp);
    return 0;
  
}