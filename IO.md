# 标准IO

分为标准IO(stdio),系统IO(sysio)
每一个系统的系统IO不一样，这时需要标准IO来统一接口，如C语言的stdio
C语言的fopen在linux系统下依赖系统IO open,window依赖openfile

面试题：
在linux gcc环境下：
```C
char * ptr = "abc";
ptr[0] = 'x';
```
是否正确？
回答：不正确，这是一个字符串常量的原因是因为在C和C++中，用双引号括起来的字符串字面值（例如："abc"）被视为字符串常量。这些字符串常量被存储在程序的只读内存段中，通常称为文本段或代码段。这是为了确保字符串字面值的不可变性，以便在编译时进行优化和安全性。
因此，当你使用以下代码创建一个指针：
```C
char *ptr = "abc";
```
ptr 指向的是内存中的字符串常量 "abc"，这个字符串常量是只读的，不允许在运行时进行修改。
如果你想在运行时修改字符串内容，你应该使用字符数组或动态内存分配。例如：
```C
char str[] = "abc";
str[0] = 'x'; // 这是允许的，现在 str 包含 "xbc"
```
或
```C
char *ptr = malloc(strlen("abc") + 1);
strcpy(ptr, "abc");
ptr[0] = 'x'; // 这也是允许的，现在 ptr 包含 "xbc"
```

```C
FILE* fopen(const *path, const *mode); //path: 路径， mode: 打开形式
int fclose(FILE *stream); //关闭文件流
```

getchar() 相当于 getc(stdin)
```C
int getc(FILE *stream);
int fgetc(FILE *stream);
```
getc是宏不是函数，fgetc是函数
宏占用编译时间，但不占用运行时间；函数占用运行时间，不占用编译时间

putchar(c) 相当于put(c, stdout)
```C
int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);
int puts(const char *s);
```