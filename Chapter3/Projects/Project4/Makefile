# $(obj-m) specifies object files that are built as kernel modules
obj-m += color_list.o collatz.o
# /lib/modules/$(shell uname -r)/build: To build against current kernel
# M= tells the builder that we are building an external module from the data in this directory
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
