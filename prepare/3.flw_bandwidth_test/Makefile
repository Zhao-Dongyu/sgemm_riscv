CTOOL := riscv64-unknown-linux-gnu-
CCL := /home/zhaodongyu/toolchain/riscv/Xuantie-900-gcc-linux-5.10.4-glibc-x86_64-V2.6.1
CC := ${CCL}/bin/${CTOOL}gcc

C_FLAGS := -march=rv64gcv0p7_zfh_xtheadc -mabi=lp64d -mtune=c906 -static

test:load_test.o load_flw.o
	${CC} $(C_FLAGS) -o test load_test.o load_flw.o

load_test.o:load_test.c
	${CC} $(C_FLAGS) -c load_test.c

load_flw.o:load_flw.S
	${CC} $(C_FLAGS) -c load_flw.S

clean:
	rm test load_test.o load_flw.o