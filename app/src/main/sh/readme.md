## shell学习

### 基本语法

- # !
  这个的作用是告诉系统需要什么解释器来执行
- .sh 这个是文件后缀名 非强制要求 执行sh文件的两种方法 1. sh xxx.sh 这种只执行的方式 直接运行解释器 表名是shell 此时第一行的 #!/bin/zsh 可以不写 2.
  ./xxx.sh 这是执行可执行文件 第一行需要#!/bin/zsh 来指定解释器

- 变量
    1. 系统变量 例如 $HOME、$PWD、$USER 注意使用字符串模板的时候 需要用双引号 单引号是字符原样输出 且区分大小写
    2. 自定义变量 定义变量：变量名=变量值 符号两侧不能有空格 变量名一般习惯用大写 删除变量：unset 变量名 声明静态变量:readonly 变量名 静态变量不能unset
       使用变量：$变量名

    3. 将命令返回值付给变量
        1. A=`ls`反引号执行里面的命令
        2. A=$(ls) 等价于反引号

- 语法

1. export 变量名=变量值 将shell变量输出为环境变量
2. source 配置文件路径 让修改的配置信息立即生效
3. echo $变量名 检查环境变量是否生效

- 位置参数变量

1. $n: 0代表命令本身 1-9 代表1到9个参数 10以上使用花括号 ${10}
2. $*:命令行中所有参数 且把所有参数看成一个整体
3. $@：命令行中所有参数 且把所有参数区分对待
4. $#：所有参数个数

- 预定义变量 在赋值定义之前 事先在Shell脚本中直接引用的变量

1. $$:当前进程的PID进程号
2. $!:后台运行的最后一个进程PID 进程号
3. $?:最后一次执行的命令返回的状态 0 为正确 非零失败

- 运算符

1. $((运算式))或者$[运算式]
2. expr m + n 注意 expr 运算符间要有空格
3. expr m - n
4. expr \*，/，% 分别代表乘，除，取余

- 条件判断

  [condition] 注意 condition前后要有空格 非空返回0 0 为true 否则为false

1. if then

```markdown

    if [ 'test01' = 'test' ] 
    then
         echo '等于' 
    fi

```

2. case 分支

```markdown

    case $变量名 in
    "值1")
    如果变量值等于值1，则执行此处程序1
    ;;
    "值2")
    如果变量值等于值2，则执行此处程序2
    ;;
    ...省略其它分支...
    *)
    如果变量值不等于以上列出的值，则执行此处程序
    ;;
    esac

```

- for 循环

```markdown
    # 语法1
    for 变量名 in 值1 值2 值3...
    do
        程序
    done
    
    # 语法2
    for ((初始值;循环控制条件;变量变化))
    do
        程序
    done
```

- while 循环

```markdown

    while [ 条件判断式 ]
    do
        程序
    done 

```

- 自定义函数

```markdown

    [ function ] funname[()]
    {
        Action;
        [return int;]
    }
    
    # 调用
    funname 参数1 参数2...
```

- tr的用法

```markdown

    tr（translate缩写）主要用于删除文件中的控制字符，或进行字符转换。
    语法：tr [–c/d/s/t] [SET1] [SET2]
    SET1: 字符集1
    SET2：字符集2
    -c:complement，用SET2替换SET1中没有包含的字符
    -d:delete，删除SET1中所有的字符，不转换
    -s: squeeze-repeats，压缩SET1中重复的字符
    -t: truncate-set1，将SET1用SET2转换， 一般缺省为-t
```

- shell脚本中的if 参数-a至-z

```markdown

        if [ -f  file ]    如果文件存在
        if [ -d ...   ]    如果目录存在
        if [ -s file  ]    如果文件存在且非空
        if [ -r file  ]    如果文件存在且可读
        if [ -w file  ]    如果文件存在且可写
        if [ -x file  ]    如果文件存在且可执行`
    
        if [ int1 -eq int2 ]    如果int1等于int2   
        if [ int1 -ne int2 ]    如果不等于    
        if [ int1 -ge int2 ]       如果>=
        if [ int1 -gt int2 ]       如果>
        if [ int1 -le int2 ]       如果<=
        if [ int1 -lt int2 ]       如果<
      
        If  [ $a = $b ]                 如果string1等于string2 字符串允许使用赋值号做等号
        if  [ $string1 !=  $string2 ]   如果string1不等于string2       
        if  [ -n $string  ]             如果string 非空(非0），返回0(true)  
        if  [ -z $string  ]             如果string 为空
        if  [ $sting ]                  如果string 非空，返回0 (和-n类似)   

```