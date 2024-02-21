CTOOL := riscv64-unknown-linux-gnu-
CCL := /home/zhaodongyu/toolchain/riscv/Xuantie-900-gcc-linux-5.10.4-glibc-x86_64-V2.6.1
CC := ${CCL}/bin/${CTOOL}gcc

C_FLAGS := -march=rv64gcv0p7_zfh_xtheadc -mabi=lp64d -mtune=c906 -static

test:main.o memcpy.o
	${CC} $(C_FLAGS) -o test main.o memcpy.o

main.o:main.c
	${CC} $(C_FLAGS) -c main.c

memcpy.o:memcpy.S
	${CC} $(C_FLAGS) -c memcpy.S

clean:
	rm test main.o memcpy.o