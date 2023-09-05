#在一个目录中有多个.c文件，每个都要单独编译一个可执行程序
SRCS:=$(wildcard *.c)
EXES:=$(patsubst %.c,%,$(SRCS))
CC:=gcc
#$$相当于用$当转义字符
# all:
#命令行进行循环
# for i in $(EXES); do gcc $$i.c -o $$i; done


all:$(EXES)
%:%.c
	$(CC) $^ -o $@ -g -lpthread
clean:
	$(RM) $(EXES)

rebuild:clean all

#调试技巧
out:
	echo $(EXES)