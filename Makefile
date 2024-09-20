
### `Makefile`

```makefile
# Makefile for compiling both launch.c and pid.c kernel module

# Kernel build variables
KDIR := /lib/modules/$(shell uname -r)/build

# Targets
all: launch pid.ko

# Compile the user-space program launch.c
launch: launch.c
	gcc -o launch launch.c

# Compile the kernel module pid.c
pid.ko: pid.c
	make -C $(KDIR) M=$(PWD) modules

# Clean up object files and kernel module
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f launch
