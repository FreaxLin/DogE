#include <stdio.h>
#include <sys/stat.h>
#include "mpc.h"
#include "doge_parse.h"

static mpc_parser_t* Object;
static mpc_parser_t* Ident;
static mpc_parser_t* Number;
static mpc_parser_t* Character;
static mpc_parser_t* String;
static mpc_parser_t* Factor;
static mpc_parser_t* Term;
static mpc_parser_t* Lexp;
static mpc_parser_t* Stmt;
static mpc_parser_t* Exp;
static mpc_parser_t* Typeident;
static mpc_parser_t* Decls;
static mpc_parser_t* Args;
static mpc_parser_t* Body;
static mpc_parser_t* Procedure;
static mpc_parser_t* Class;
static mpc_parser_t* Doge;
static mpc_err_t* err;

void init_parse(){
    Object        = mpc_new("object");
    Ident         = mpc_new("ident");
    Number        = mpc_new("number");
    Character     = mpc_new("character");
    String        = mpc_new("string");
    Factor        = mpc_new("factor");
    Term          = mpc_new("term");
    Lexp          = mpc_new("lexp");
    Stmt          = mpc_new("stmt");
    Exp           = mpc_new("exp");
    Typeident     = mpc_new("typeident");
    Decls         = mpc_new("decls");
    Args          = mpc_new("args");
    Body          = mpc_new("body");
    Procedure     = mpc_new("procedure");
    Class         = mpc_new("class");
    Doge          = mpc_new("doge");

    err = mpca_lang(MPCA_LANG_DEFAULT,
      " object      : /[A-Z][a-zA-Z]*/ ;                                           \n"
      " ident     : /[a-z][a-zA-Z0-9_]*/ ;                                         \n"
      " number      : /[0-9]+/ ;                                                   \n"
      " character : /'.'/ ;                                                        \n"
      " string      : /\"(\\\\.|[^\"])*\"/ ;                                       \n"
      "                                                                            \n"
      " factor      : '(' <lexp> ')'                                               \n"
      "            | <number>                                                      \n"
      "            | <character>                                                   \n"
      "            | <string>                                                      \n"
      "            | <ident> '(' <lexp>? (',' <lexp>)* ')'                         \n"
      "            | <ident> ;                                                     \n"
      "                                                                            \n"
      " term        : <factor> (('*' | '/' | '%') <factor>)* ;                     \n"
      " lexp        : <term> (('+' | '-') <term>)* ;                               \n"
      "                                                                            \n"
      " stmt        : '{' <stmt>* '}'                                              \n"
      "            | \"while\" '(' <exp> ')' <stmt>                                \n"
      "            | \"if\"      '(' <exp> ')' <stmt>                              \n"
      "            | <ident> '=' <lexp> ';'                                        \n"
      "            | \"print\" '(' <lexp>? ')' ';'                                 \n"
      "            | \"return\" <lexp>? ';'                                        \n"
      "            | (<ident>'.')? <ident> '(' <ident>? (',' <ident>)* ')' ';'     \n"
      "            | <object> <ident> '=' \"new\" <object>'(' <args> ')' ';' ;     \n"
      "                                                                            \n"
      " exp        : <lexp> '>' <lexp>                                             \n"
      "            | <lexp> '<' <lexp>                                             \n"
      "            | <lexp> \">=\" <lexp>                                          \n"
      "            | <lexp> \"<=\" <lexp>                                          \n"
      "            | <lexp> \"!=\" <lexp>                                          \n"
      "            | <lexp> \"==\" <lexp> ;                                        \n"
      "                                                                            \n"
      " typeident : (\"int\" | \"char\") <ident> ;                                 \n"
      " decls     : (<typeident> ('=' (<number>|<character>))?';')* ;              \n"
      " args        : <typeident>? (',' <typeident>)* ;                            \n"
      " body        : '{' <decls> <stmt>*'}' ;                                     \n"
      " procedure : (\"int\" | \"char\") <ident> '(' <args> ')' <body> ;           \n"
      " class     : \"Class\" <object> '{' <decls><procedure>* '}';                \n"
      " doge        : /^/ <class> /$/ ;                                            \n",
      Object, Ident, Number, Character, String, Factor, Term, Lexp, Stmt, Exp, 
      Typeident, Decls, Args, Body, Procedure, Class, Doge, NULL);

    if (err != NULL) {
        mpc_err_print(err);
        mpc_err_delete(err);
        exit(1);
    }
}

void destory_parse(){
    mpc_cleanup(17, Object, Ident, Number, Character, String, Factor, Term, Lexp, Stmt, Exp, Typeident, Decls, Args, Body, Procedure, Class, Doge);
}


CLASS_FIELD* parse_decls(mpc_ast_t* t){
    int children_num = t->children_num;
    CLASS_FIELD header;
    CLASS_FIELD* result = &header;
    for (int i = 0; i < children_num; i++){
        char* children_tag = t->children[i]->tag;
        
        CLASS_FIELD field;
        header.next = &field;
        if (strstr(children_tag, "typeident")){
            mpc_ast_t* typeident_info = t->children[i];
            char* type = typeident_info->children[0]->contents;
            char* ident_name = typeident_info->children[1]->contents;
            field.name = ident_name;
            field.type = type;
        }
        if (strstr(children_tag, "char") && strcmp(t->children[i]->contents, "=") == 0){
            char* value = t->children[i+1]->contents;
            
            if ((strstr(t->children[i+1]->tag, "number") && strcmp(field.type, "int") == 0) 
                || (strstr(t->children[i+1]->tag, "character") && strcmp(field.type, "char") == 0)){
                field.value = value;
            }else{
                printf("%s不匹配%s\n", field.type, value);
                exit(1);
            }            
        }
        
        header = *header.next;
    }
    return result;
}
void parse_procedure(mpc_ast_t* t){
    int children_num = t->children_num;
    for (int i = 0; i < children_num; i++){
        if (strstr(t->children[i]->tag, "Class")) {
            
        }
    }
}

void parse_class(mpc_ast_t* t){
    int children_num = t->children_num;
    for (int i = 0; i < children_num; i++){
        if (strstr(t->children[i]->tag, "class")) {
            mpc_ast_t* class_info = t->children[i];
            int class_children_num = class_info->children_num;
            for (int j = 0; j < class_children_num; j++){
                char* tag_name = class_info->children[j]->tag;
                if (strstr(tag_name, "object")){
                    printf("类名%s\n", class_info->children[j]->contents);
                }
                if (strstr(tag_name, "decls")){
                    CLASS_FIELD* decls_point = parse_decls(class_info->children[j])->next;
                    while (decls_point != NULL){
                        printf("%s\n", decls_point->name);
                        decls_point = decls_point->next;
                    }
                    
                }
                if (strstr(tag_name, "procedure")){
                    parse_procedure(class_info->children[j]);
                }
            }
        }
    }
}


int parse_doge(char* file) {
            
    FILE* fp = fopen(file,"rb");
    mpc_result_t r;
    if (mpc_parse_file("Numberc", fp, Doge, &r)) {
        // parse_class(r.output);
        mpc_ast_print(r.output);
        mpc_ast_delete(r.output);
    } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }

    fclose(fp);
    return 0;
    
}