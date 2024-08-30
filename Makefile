# Compiler to use
CC = gcc
# Compiler flags
CFLAGS = -Wall -Wextra

# Default target
all: disk_scheduler

# Rule to create the static library
libdisk_scheduling.a: Disk_Scheduling.o
	ar rcs libdisk_scheduling.a Disk_Scheduling.o

# Rule to compile Disk_Scheduling.c into an object file
Disk_Scheduling.o: Disk_Scheduling.c Disk_Scheduling.h
	$(CC) $(CFLAGS) -c Disk_Scheduling.c

# Rule to compile and link the main program
disk_scheduler: main.c libdisk_scheduling.a
	$(CC) $(CFLAGS) main.c -L. -ldisk_scheduling -o disk_scheduler

# Clean up built files
clean:
	rm -f *.o *.a disk_scheduler