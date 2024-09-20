# pid-launcher-kernel-module
# Process Monitoring and Launching Project

This repository contains two main components:

1. **Process Launcher** (`launch.c`): A user-space program that launches an executable and stores its PID.
2. **Kernel Module** (`pid.c`): A Linux kernel module that monitors a specific process by writing its PID to `/proc/pid` and reading its details.

## Files in the Repository

- `launch.c` - A user-space C program that launches a process and stores its PID in `/proc/pid`.
- `pid.c` - A Linux kernel module to monitor processes using their PID.
- `Makefile` - A Makefile to compile both `launch.c` and the kernel module.
- `README.md` - Documentation for this project.

---

## Compilation, Usage, and Cleanup

The following code snippet will **compile**, **run**, **insert the kernel module**, **write to `/proc/pid`**, **read process details**, **remove the kernel module**, and **clean up** compiled files:

```bash
# 1. Compile both the user-space program and kernel module
make

# 2. Run the user-space program to launch a ping process
./launch /bin/ping 8.8.8.8

# 3. Insert the kernel module
sudo insmod pid.ko

# 4. Write a PID to /proc/pid (use a valid PID from the previous step)
echo "<PID>" | sudo tee /proc/pid

# 5. Read process details from /proc/pid
cat /proc/pid

# 6. Remove the kernel module
sudo rmmod pid

# 7. Clean up the compiled files
make clean
