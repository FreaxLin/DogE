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
      " typeident : (\"int\" | \"char\") <ident> ('=' (<number>|<character>))?;    \n"
      " decls     : (<typeident> ';')* ;                                           \n"
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

void parse_stmt(){

}

void parse_procedure_body(){

}


CLASS_FIELD* parse_typeident(mpc_ast_t* typeident_info){
    CLASS_FIELD* field = (CLASS_FIELD*) malloc(sizeof(CLASS_FIELD));
    field->next = NULL;
    field->type = typeident_info->children[0]->contents;
    field->name = typeident_info->children[1]->contents;
    if (typeident_info->children_num > 2){
        char* type_tag = typeident_info->children[3]->tag;
        if ((strstr(type_tag, "number") && strcmp(field->type, "int") == 0) || (strstr(type_tag, "char") && strcmp(field->type, "char"))){
            field->value = typeident_info->children[3]->contents;
        }
    }
    return field;
}

CLASS_FIELD* parse_arg(mpc_ast_t* args_info){
    if (strstr(args_info->tag, "args|typeident")){
        return parse_typeident(args_info);
    }
    int args_children_nums = args_info->children_num;
    CLASS_FIELD header;
    header.next = NULL;
    for (int i = 0; i < args_children_nums; i++){
        CLASS_FIELD* children_field = parse_typeident(args_info->children[i]);
        children_field->next = header.next;
        header.next = children_field;
    }
    return header.next;
}

CLASS_FIELD* parse_decls(mpc_ast_t* t){
    int children_num = t->children_num;
    CLASS_FIELD* header = (CLASS_FIELD*) malloc(sizeof(CLASS_FIELD));
    header->next = NULL;
    for (int i = 0; i < children_num; i++){
        char* children_tag = t->children[i]->tag;
        if (strstr(children_tag, "typeident")){
            CLASS_FIELD* field = parse_typeident(t->children[i]);
            field->next = header->next;
            header->next = field;
        }
    }
    return header;
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
    if (strstr(t->children[1]->tag, "class")){
        mpc_ast_t* class_info = t->children[1];
        int class_children_num = class_info->children_num;
        for (int i = 0; i < class_children_num; i++){
            char* tag_name = class_info->children[i]->tag;
            if (strstr(tag_name, "object")){
                printf("类名%s\n", class_info->children[i]->contents);
            }
            if (strstr(tag_name, "decls")){
                CLASS_FIELD* decls_point = parse_decls(class_info->children[i]);
                CLASS_FIELD* decls_point_temp = decls_point->next;
                while (decls_point_temp != NULL){
                    printf("%s\n", decls_point_temp->name);
                    decls_point_temp = decls_point_temp->next;
                }
                
            }
            if (strstr(tag_name, "procedure")){
                // CLASS_FIELD* decls_point = parse_decls(class_info->children[j]);
                // CLASS_FIELD* decls_point_temp = decls_point->next;
                // while (decls_point_temp != NULL){
                //     printf("%s\n", decls_point_temp->name);
                //     decls_point_temp = decls_point_temp->next;
                // }
                
            }
        }
    }
}


int parse_doge(char* file) {
            
    FILE* fp = fopen(file,"rb");
    mpc_result_t r;
    if (mpc_parse_file("Numberc", fp, Doge, &r)) {
        parse_class(r.output);
        mpc_ast_print(r.output);
        mpc_ast_delete(r.output);
    } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }

    fclose(fp);
    return 0;
    
}