#include <stdio.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdlib.h>
#include "dogec.h"
#include "miniVM.h"

static const struct option long_opts[] = {
    {"help", no_argument, NULL, 'h'},
    {"compile", required_argument, NULL, 'c'},
    {"main", required_argument, NULL, 'm'},
    {0, 0, 0}
};

void usage(){
    printf("%s\n", "options:");
    printf("\t -h, --help \n"
           "\t -c, --complie=FILE 编译文件的路径\n"
           "\t -m, --main=def 启动程序的方法\n"
    );
}

void print_class(class_meta* cm){
    printf("加载的类名为:%s\n", cm->class_name);
    printf("加载的属性个数为:%d\n", cm->field_count);
    for (int i = 0; i < cm->field_count; i++){
        printf("加载的属性为:%s\n", cm->field_array[i]);

    }
    printf("加载的方法个数为:%d\n", cm->def_count);
    for (int i = 0; i < cm->def_count; i++){
        printf("加载的方法为:%s\n", cm->def_array[i]);
        def_meta* dm = (def_meta*) hashtable_get(cm->def_table, cm->def_array[i]);
        printf("方法局部变量表大小%d\n", dm->size);
        printf("方法局部变量表个数%d\n", dm->nums);
        printf("方法%s命令如下\n", cm->def_array[i]);
        for (int j = 0; j < dm->command_count; j++){
            printf("%s\n", dm->command_array[j]);
        }
    }
}

int main(int argc, char **argv) {

    if (argc == 1){
        usage();
        exit(1);
    }

    char *start_def = "compute";
    int c;
    int long_index;
    char* file_path;
    while(( c = getopt_long(argc, argv, "hc:m:", long_opts, &long_index)) != EOF){
        switch(c){
            case 'c':
                file_path = optarg;
                break;
            case 'm':
                start_def = optarg;
                break;
            case 'h':
            default:
                usage();
                exit(1);
        }
    }

    struct stat s_buf;
    stat(file_path, &s_buf);
    int file_number = 0;
    if (S_ISDIR(s_buf.st_mode)){
        printf("目录:%s\n", file_path);
    }else{
        printf("%s\n", file_path);
    }

    init_compile();
    class_meta cm = compile_doge(file_path);
    print_class(&cm);
    destory_compile();
    
    bootstrap(&cm, start_def);

    return 0;
    
}