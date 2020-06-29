#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "dogec.h"
#include "util.h"

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

void init_compile(){
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

void destory_compile(){
    mpc_cleanup(17, Object, Ident, Number, Character, String, Factor, Term, Lexp, Stmt, Exp, Typeident, Decls, Args, Body, Procedure, Class, Doge);
}

char* generate_command(char* command_header, int position, void* value){
    int pos_size = 1;
    char p[10];
    for (int i = 0; i < 10; i++){
        p[i] = '\0';
    }
    char* result = NULL;
    if (position != -1){
        sprintf(p, "%d", position);
        // int_to_str(p, position);
        pos_size = strlen(p);
    }
    if (value != NULL){
        pos_size++;
    }
    char* temp = (char*) malloc(strlen(command_header) + pos_size);
    strcpy(temp, command_header);
    if (strlen(p) > 0){
        strcat(temp, p);
    }
    
    
    if (value != NULL){
        strcat(temp, "#");
        result = (char*) malloc(strlen(temp) + strlen(value));
        strcpy(result, temp);
        strcat(result, value);
        
    }else{
        result = (char*) malloc(strlen(temp) + 1);
        strcpy(result, temp);
    }
    free(temp);
    return result;
}

/*
 *  解析域值
 */
field_meta* compile_typeident(mpc_ast_t* typeident_info){
    //为该类方法区分配域空间
    field_meta* field = field_meta_new();
    field->value = NULL;

    //解析域类型
    if (strcmp(typeident_info->children[0]->contents, "int") == 0){
        field->type = 'I';
    }else if (strcmp(typeident_info->children[9]->contents, "char") == 0){
        field->type = 'C';
    }else{
        field->type = 'O';
    }

    //解析域名称
    int name_lenght = strlen(typeident_info->children[1]->contents);
    char* name = (char*) malloc(sizeof(char) * name_lenght);
    strcpy(name, typeident_info->children[1]->contents);

    field->field_name = name;

    //解析域值
    if (typeident_info->children_num > 2){
        char* type_tag = typeident_info->children[3]->tag;
        if ((strstr(type_tag, "number") && field->type == 'I') || (strstr(type_tag, "char") && field->type == 'C')){
            int value_lenght = strlen(typeident_info->children[3]->contents);
            void* value = malloc(sizeof(char) * value_lenght);
            strcpy(value, typeident_info->children[3]->contents);
            field->value = value;
        }
    }
    return field;
}

char* complie_exp(mpc_ast_t* exp_info, hashtable* ht){
    int children_num = exp_info->children_num;
    int index;
    char* command_header;
    for (int i = 0; i < children_num; i++){
        char* tag = exp_info->children[i]->tag;
        
        if (strstr(tag, "lexp|term|factor|ident")){
            index = *(int*)hashtable_get(ht, exp_info->children[i]->contents);
        }
        if (strstr(tag, "char") && strcmp("<", exp_info->children[i]->contents) == 0){
            command_header = "st_";
        }
        if (strstr(tag, "lexp|term|factor|number")){
            char* command = generate_command(command_header, index, exp_info->children[i]->contents);
            return command;
        }
    }
    return NULL;
}

typedef struct _BACKET{
    char bracket;
    struct _BACKET* next;
}backet_stack;

void complie_lexp(mpc_ast_t* lexp_info, def_meta* dm, hashtable* ht){
    // int children_num = lexp_info->children_num;
    // for (int i = 0; i < children_num; i++){
    //     char* tag = lexp_info->children[i]->tag;
    //     if (strstr(tag, "term|factor|ident")){
    //         int index = *(int*)hashtable_get(ht, lexp_info->children[i]->contents);
    //         char* command = generate_command("push_", index, NULL);
    //         printf("%s\n", command);
    //     }
    //     if (strstr(tag, "char") && strcmp("+", lexp_info->children[i]->contents) == 0){
    //         char* command = generate_command("add", -1, NULL);
    //         printf("%s\n", command);
    //     }
    // }
    int index = *(int*)hashtable_get(ht, lexp_info->children[0]->contents);
    char* first_param = generate_command("push_", index, NULL);
    // printf("%s\n", first_param);
    dm->command_array[dm->command_count] = first_param;
    dm->command_count = dm->command_count + 1;
    index = *(int*)hashtable_get(ht, lexp_info->children[2]->contents);
    char* second_param = generate_command("push_", index, NULL);
    // printf("%s\n", second_param);
    dm->command_array[dm->command_count] = second_param;
    dm->command_count = dm->command_count + 1;
    char* operation = generate_command("add", -1, NULL);
    // printf("%s\n", operation);
    dm->command_array[dm->command_count] = operation;
    dm->command_count = dm->command_count + 1;

}


void compile_procedure_stmt(mpc_ast_t* stmt_info, def_meta* dm, hashtable* ht){
    int children_num = stmt_info->children_num;
    int i = 0;
    while (i < children_num){
        char* tag = stmt_info->children[i]->tag;

        if (strstr(tag, "stmt")){
            compile_procedure_stmt(stmt_info->children[i], dm, ht);
        }

        if (strstr(tag, "string")){

            //解析while循环体
            if (strcmp("while", stmt_info->children[i]->contents) == 0){
                int loop_start;
                int j = i + 1;
                backet_stack header;
                header.next = NULL;
                if (strcmp(stmt_info->children[j]->contents, "(") == 0){
                    backet_stack bs;
                    bs.bracket = '(';
                    bs.next = header.next;
                    header.next = &bs;
                }
                j++;
                while (header.next != NULL){
                    if (strcmp(stmt_info->children[j]->contents, "(") == 0){
                        backet_stack bs;
                        bs.bracket = '(';
                        bs.next = header.next;
                        header.next = &bs;
                    }
                    if (strcmp(stmt_info->children[j]->contents, ")") == 0){
                        header.next = header.next->next;
                    }
                    if (strstr(stmt_info->children[j]->tag, "exp")){
                        dm->command_array[dm->command_count] = complie_exp(stmt_info->children[j], ht);
                        // printf("%s\n", dm->command_array[dm->command_count]);
                        loop_start = dm->command_count; 
                        dm->command_count = dm->command_count + 1;
                    }
                    j++;
                }
                char temp[10];
                sprintf(temp, "%d", dm->command_count + 1);
                dm->command_array[dm->command_count] = generate_command("jump", -1, temp);
                // printf("%s\n", dm->command_array[dm->command_count]);
                dm->command_count = dm->command_count + 1;
                compile_procedure_stmt(stmt_info->children[j], dm, ht);
                sprintf(temp, "%d", loop_start);
                char* jump = generate_command("jump", -1, temp);
                dm->command_array[dm->command_count] = jump;
                // printf("%s\n", dm->command_array[dm->command_count]);
                dm->command_count = dm->command_count + 1;
                sprintf(temp, "%d", dm->command_count);
                char* out_loop = generate_command(dm->command_array[loop_start + 1], -1, temp);
                free(dm->command_array[loop_start + 1]);
                dm->command_array[loop_start + 1] = out_loop;
                // printf("%s\n", dm->command_array[loop_start + 1]);
                i = j;
            }

            if (strcmp("print", stmt_info->children[i]->contents) == 0){
                int j = i;
                int* pos;
                while (strcmp(stmt_info->children[j]->contents, ";") != 0){
                    if (strstr(stmt_info->children[j]->tag, "lexp|term|factor|ident")){
                        char* ident = stmt_info->children[j]->contents;
                        pos = (int*)hashtable_get(ht, ident);
                        
                    }
                    j++;
                }
                
                char* command = generate_command("print_", *pos, NULL);
                // printf("%s\n", command);
                dm->command_array[dm->command_count] = command;
                dm->command_count = dm->command_count + 1;
                i = j;
            }
           
        }
        
        if (strstr(tag, "object")){
            int j = i;
            char* class_name = stmt_info->children[j]->contents;
            while (strcmp(stmt_info->children[j]->contents, ";") != 0){
                
                if (strstr(stmt_info->children[j]->tag, "ident")){
                    if (hashtable_get(ht, stmt_info->children[j]->contents) != NULL){
                        printf("重复定义:%s\n", stmt_info->children[j]->contents);
                    }else{
                        int* index = (int*) malloc(sizeof(int));
                        *index = dm->nums;
                        hashtable_set(ht, stmt_info->children[j]->contents, index);
                    
                        if (dm->nums == 0){
                            dm->off_set[dm->nums] = 0;
                        }else{
                            dm->off_set[dm->nums] = sizeof(void*);
                        }
                        dm->nums = dm->nums + 1;
                        dm->size = dm->size + sizeof(void*);
                    }                    
                }

                j++;
            }

            char* command = generate_command("objectset_", dm->nums - 1, NULL);
           
            char* new_command = (char*) malloc(6 + strlen(class_name));
            strcpy(new_command, "init#");
            strcat(new_command, class_name);
            dm->command_array[dm->command_count] = new_command;
            dm->command_count = dm->command_count + 1;
            dm->command_array[dm->command_count] = command;
            dm->command_count = dm->command_count + 1;
            // printf("%s\n", new_command);
            //  printf("%s\n", command);
            i = j;
            
        }
        
        if (strstr(tag, "ident")){
            int j = i;
            char* ident = stmt_info->children[j]->contents;
            while (strcmp(stmt_info->children[j]->contents, ";") != 0){
                if (strstr(stmt_info->children[j]->tag, "lexp")){
                    complie_lexp(stmt_info->children[j], dm, ht);
                }
                j++;
            }
            int index = *(int*)hashtable_get(ht, ident);
            char* command = generate_command("set_", index, NULL);
            // printf("%s\n", command);
            dm->command_array[dm->command_count] = command;
            dm->command_count = dm->command_count + 1;
            i = j;
        }
        i++;
    }
    
}

void compile_procedure_decls(mpc_ast_t* decls_info, def_meta* dm, hashtable* ht){
    int children_num = decls_info->children_num;
    int size = 0;
    for (int i = 0; i < children_num; i++){
        char* children_tag = decls_info->children[i]->tag;
        if (strstr(children_tag, "typeident")){
            field_meta* field = compile_typeident(decls_info->children[i]);
            int field_size = 0;
            if (field->type == 'I'){
                field_size = sizeof(int);
            }else if (field->type == 'C'){
                field_size = sizeof(char);
            }else{
                field_size = sizeof(void*);
            }
            if (hashtable_get(ht, field->field_name) != NULL){
                printf("重复定义:%s\n", field->field_name);
            }else{
                int* index = (int*) malloc(sizeof(int));
                *index = dm->nums;
                hashtable_set(ht, field->field_name, index);
            
                if (field->value != NULL){
                    char* command = generate_command("set_", *index, field->value);
                    // printf("%s\n", command);
                    dm->command_array[dm->command_count] = command;
                    dm->command_count = dm->command_count + 1;
                }
                
                if (dm->nums == 0){
                    dm->off_set[dm->nums] = 0;
                }else{
                    dm->off_set[dm->nums] = field_size;
                }    
                dm->nums = dm->nums + 1;
                dm->size = dm->size + field_size;
            }
        }
    }
}

void compile_procedure_body(mpc_ast_t* body_info, def_meta* dm, hashtable* ht){
    int body_children_num = body_info->children_num;

    for (int i = 1; i < body_children_num - 1; i++){
        char* tag = body_info->children[i]->tag;
        if (strstr(tag, "decls")){
            compile_procedure_decls(body_info->children[i], dm, ht);
            continue;
        }
        if (strstr(tag, "stmt")){
            compile_procedure_stmt(body_info->children[i], dm, ht);
            continue;
        }
    }
}



// typedef struct{
//     field_meta* fm;

// };

field_meta* compile_arg(mpc_ast_t* args_info){
    return NULL;
}

int compile_decls(mpc_ast_t* t, class_meta* cm, hashtable* ft){
    int children_num = t->children_num;
    int size = 0;
    for (int i = 0; i < children_num; i++){
        char* children_tag = t->children[i]->tag;
        if (strstr(children_tag, "typeident")){
            field_meta* field = compile_typeident(t->children[i]);
            cm->field_array[cm->field_count] = field->field_name;
            cm->field_count = cm->field_count + 1;
            int field_size = 0;
            if (field->type == 'I'){
                field_size = sizeof(int);
            }else if (field->type == 'C'){
                field_size = sizeof(char);
            }else{
                field_size = sizeof(void*);
            }
            field->offset = i * field_size;
            size = size + field_size;
            hashtable_set(ft, field->field_name, field);

        }
    }
    return size;
}

void compile_procedure(mpc_ast_t* t, class_meta* cm, hashtable* ht){
    int children_num = t->children_num;
    def_meta* dm = (def_meta*) malloc(sizeof(def_meta));
    //解析方法返回类型
    char* def_type = (char*) malloc(sizeof(char) * strlen(t->children[0]->contents));
    strcpy(def_type, t->children[0]->contents);
    dm->return_type = def_type;

    //解析方法名
    char* def_name = (char*) malloc(sizeof(char) * strlen(t->children[1]->contents));
    strcpy(def_name, t->children[1]->contents);
    dm->def_name = def_name;
    hashtable_set(ht, dm->def_name, dm);
    cm->def_array[cm->def_count] = dm->def_name;
    cm->def_count = cm->def_count + 1;
    dm->nums = 0;
    dm->size = 0;
    dm->off_set = (int*) malloc(10 * sizeof(int));
    dm->command_count = 0;
    dm->command_array = (char**) malloc(20 * sizeof(char *));

    //解析参数
    // field_meta* args = compile_arg(t->children[3]);

    //当没有参数时,方法体的索引位置为4
    int body_index = 0;
    if (strcmp(t->children[3]->contents, ")") == 0){
        body_index = 4;
    }else{
        body_index = 5;
    }
    hashtable* table_index = hashtable_create();
    compile_procedure_body(t->children[body_index], dm, table_index);

    for (int i = 0; i < children_num; i++){
        
        // printf("%s\n",t->children[i]->tag);
        
    }
}

class_meta compile_class(mpc_ast_t* t){
    int children_num = t->children_num;
    class_meta cm;
    if (strstr(t->children[1]->tag, "class")){
        mpc_ast_t* class_info = t->children[1];
        int class_children_num = class_info->children_num;
        hashtable* fb = hashtable_create();
        cm.field_table = fb;
        hashtable* def_ht = hashtable_create();
        cm.def_table = def_ht;
        cm.def_array = (char**) malloc(10 * sizeof(char *));
        cm.field_array = (char**) malloc(10 * sizeof(char *));
        cm.def_count = 0;
        cm.field_count = 0;
        for (int i = 0; i < class_children_num; i++){
            char* tag_name = class_info->children[i]->tag;
            if (strstr(tag_name, "object")){
                int class_name_lenght = strlen(class_info->children[i]->contents);
                char* class_name = (char*) malloc(sizeof(char) * class_name_lenght);
                strcpy(class_name, class_info->children[i]->contents);
                cm.class_name = class_name;
            }
            if (strstr(tag_name, "decls")){
                int field_size = compile_decls(class_info->children[i], &cm, fb);
                cm.size = field_size;
            }
            if (strstr(tag_name, "procedure")){
                compile_procedure(class_info->children[i], &cm, def_ht);
                
            }
        }
    }
    return cm;
}


class_meta compile_doge(char* file) {
            
    FILE* fp = fopen(file,"rb");
    mpc_result_t r;
    class_meta cm;
    if (mpc_parse_file("Numberc", fp, Doge, &r)) {
        cm = compile_class(r.output);
        // mpc_ast_print(r.output);
        mpc_ast_delete(r.output);
    } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }
    
    
    fclose(fp);
    return cm;
    
}

