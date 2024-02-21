CTOOL := riscv64-unknown-linux-gnu-
CCL := /home/zhaodongyu/toolchain/riscv/Xuantie-900-gcc-linux-5.10.4-glibc-x86_64-V2.6.1
CC := ${CCL}/bin/${CTOOL}gcc

C_FLAGS := -march=rv64gcv0p7_zfh_xtheadc -mabi=lp64d -mtune=c906 -static

test:main.o saxpy.o  
	${CC} $(C_FLAGS) -o test  saxpy.o  main.o 

main.o:main.c
	${CC} $(C_FLAGS) -c main.c

saxpy.o:saxpy.S
	${CC} $(C_FLAGS) -c saxpy.S

clean:
	rm test main.o saxpy.o 