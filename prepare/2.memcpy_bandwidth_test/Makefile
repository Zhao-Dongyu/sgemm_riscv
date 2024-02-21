CTOOL := riscv64-unknown-linux-gnu-
CCL := /home/zhaodongyu/toolchain/riscv/Xuantie-900-gcc-linux-5.10.4-glibc-x86_64-V2.6.1
CC := ${CCL}/bin/${CTOOL}gcc

C_FLAGS := -march=rv64gcv0p7_zfh_xtheadc -mabi=lp64d -mtune=c906 -static

mbw:mbw.o
	${CC} $(C_FLAGS) -o mbw  mbw.o

clean:
	rm mbw mbw.o