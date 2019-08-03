obj-m += printer.o

all:    
	make -C ../../Kernel M=$(PWD) modules
clean:    
	make -C ../../Kernel M=$(PWD) clean


