#include <stdio.h>
#include <sys/stat.h>
#include <getopt.h>

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
           "\t -m, --main=FILE 启动程序的方法\n"
    );
}

int main(int argc, char **argv) {

    char *str;
    int c;
    int long_index;
    
    while(( c = getopt_long(argc, argv, "hc:m:", long_opts, &long_index)) != EOF){
        switch(c){
            case 'c':
                printf("1: %s\n", optarg);
                break;
            case 'm':
                str = optarg;
                printf("2: %s\n", optarg);
                break;
            case 'h':
            default:
                usage();
                exit(1);
        }
    }
    return 0;
    // char* doge_path = argv[1];
    // struct stat s_buf;
    // stat(doge_path, &s_buf);
    // char** file_paths[1];
    // int file_number = 0;
    // if (S_ISDIR(s_buf.st_mode)){

    // }else{
    //     printf("%s\n", doge_path);
    //     file_number++;
    // }
    // for (int i = 0; i < file_number; i++){
    //     printf("%s\n", file_paths[0]);
    // }
}