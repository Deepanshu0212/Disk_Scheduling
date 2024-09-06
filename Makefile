# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Default target
all: disk_scheduler

# Object files
OBJ = Disk_Scheduling.o test.o main.o

# Rule to create the executable
disk_scheduler: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile Disk_Scheduling.c
Disk_Scheduling.o: Disk_Scheduling.c Disk_Scheduling.h
	$(CC) $(CFLAGS) -c $<

# Rule to compile test.c
test_case.o: test.c Disk_Scheduling.h
	$(CC) $(CFLAGS) -c $<

# Rule to compile main.c
main.o: main.c Disk_Scheduling.h
	$(CC) $(CFLAGS) -c $<

# Clean up built files
clean:
	rm -f *.o disk_scheduler