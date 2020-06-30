# Doge
Doge是用C编写的一种模范Java的小型语言，包括编译，加载，虚拟机命令执行和垃圾回收算法。

语法示例如下:
```
Class Test{
    int m = 1;
    int k;
    int compute(){
        int i = 1;
        int j = 1;
        while(i < 10){
            Test t = new Test();
            print(i);
            i = i + j;
        }
        print(i);
        return 0;
    }

}
```
## 编译和载入
编译中词法解析用的是[mpc](https://github.com/orangeduck/mpc)
之后是语法解析，解析后直接将类所有的相关信息都载入到方法区

## 命令执行
方法执行会开辟一个栈空间去加载栈帧，一个方法对应一个栈帧，栈帧内存会初始化本地变量表和操作数栈
## gc算法
使用标记清除算法

## example
### 编译程序
gcc main.c dogec.c method_area.c util.c miniVM.c stack.c heap.c ./extern_dependency/mpc.c ./extern_dependency/hashtable.c -o main
### 执行
./main -c/Users/sai/Doge/example/Numberc.doge  -mcompute

其中 -c参数为代码文件路径 -m参数为开始执行的方法

输出如下
```
加载的类名为:Test
加载的属性个数为:2
加载的属性为:m
加载的属性为:k
加载的方法个数为:1
加载的方法为:compute
方法局部变量表大小16
方法局部变量表个数3
方法compute命令如下
set_0#1
set_1#1
st_0#10
jump#4#12
init#Test
object_2
print_0
push_0
push_1
add
set_0
jump#2
print_0
初始化堆100B空间
开始执行compute方法
程序输出:1
程序输出:2
程序输出:3
程序输出:4
程序输出:5
程序输出:6
堆空间不足,启动gc
总共释放了80B堆空间
程序输出:7
程序输出:8
程序输出:9
程序输出:10
compute方法执行完毕,虚拟机退出
```



