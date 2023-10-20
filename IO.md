# IO

分为标准IO(stdio),系统IO(sysio)
每一个系统的系统IO不一样，这时需要标准IO来统一接口，如C语言的stdio
C语言的fopen在linux系统下依赖系统IO open,window依赖openfile