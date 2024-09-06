# Disk Scheduling Algorithms

This project implements various disk scheduling algorithms in C. The goal of disk scheduling algorithms is to manage and optimize the order in which disk requests are processed. Different algorithms have different strategies for minimizing the total head movement, waiting time, throughput, and more.

## Table of Contents
- [Disk Scheduling Algorithms](#disk-scheduling-algorithms)
  - [Table of Contents](#table-of-contents)
  - [Implemented Algorithms](#implemented-algorithms)
  - [Directory Structure](#structure)
  - [Features](#features)
  - [Usage](#usage)
  - [Performance Metrics](#performance-metrics)
  - [Testing](#testing)
  - [How to Run](#how-to-run)
  - [Dependencies](#dependencies)

## Implemented Algorithms
The following disk scheduling algorithms are implemented:
1. **First Come First Serve (FCFS)**
2. **Shortest Seek Time First (SSTF)**
3. **SCAN (Elevator Algorithm)**
4. **C-SCAN (Circular SCAN)**
5. **LOOK**
6. **C-LOOK**

Each algorithm has its own description function to explain the approach used and the sequence of operations.


## Directory Structure

- **[Header](Header)**:
  - [Disk_Scheduling.h](Header/Disk_Scheduling.h): Header file containing the function prototypes and data structures.

- **[Implementation](Implementation)**:
  - [Disk_Scheduling.c](Implementation/Disk_Scheduling.c): Contains the implementation of disk scheduling algorithms.
  - [main.c](Implementation/main.c): Integrates user interaction, function calls, and testing.

- **[Test](Test)**:
  - [test.c](Test/test.c): Contains various test cases to verify each algorithm against expected outputs.

- **[bin](bin)**:
  - Contains binary executable files generated from the source code.

- **[Information](Information)**:
  - All content like total movement, plots in the form of `.png` files will be generated in this folder.


## Features

- **Multiple Algorithms**: Six different disk scheduling algorithms to choose from.
- **Performance Metrics**: Total head movement, average waiting time, throughput, and standard deviation of response time are calculated for each algorithm.
- **Random Input Generation**: Ability to randomly generate disk requests.
- **Graph Plotting**: Graph plotting functionality to visualize the results.
- **Edge Case Handling**: Edge cases such as invalid inputs and edge behavior of the algorithms are handled effectively.

## Usage

1. **Choose a Disk Scheduling Algorithm**: Users can select from the available disk scheduling algorithms.
2. **Random or Custom Input**: Users can either generate random disk requests or input them manually.
3. **Performance Metrics**: The program calculates key performance metrics for each algorithm.
4. **Graph Plotting**: Results are saved in a file and can be visualized graphically.


## Performance Metrics
The following performance metrics are calculated for each algorithm:
- **Total Head Movement**: The total number of cylinder movements the disk head makes.
- **Average Waiting Time**: The average time each request waits before being served.
- **Throughput**: Number of requests processed over time.
- **Standard Deviation of Response Time**: A measure of the variability in response times.

## Testing
This project includes unit tests for each algorithm to verify the correctness of the implementation. Test cases are defined in separate functions to ensure algorithms handle both typical and edge case scenarios. These include:
- FCFS
- SSTF
- SCAN
- C-SCAN
- LOOK
- C-LOOK
- Edge cases (such as repeated requests)

To run the tests, simply call the appropriate test function in `main.c`.


## How to run

### Clone the repository
``` bash
git clone https://github.com/Deepanshu0212/Disk_Scheduling.git
```

### Change Directory
``` bash
cd Disk_Scheduling
```

### For Windows (Using Mingw)
``` bash
mingw32-make
./bin/disk_scheduler
mingw32-make clean
```

### For Linux
``` bash
make
./bin/disk_scheduler
make clean
```

## Dependencies

### Windows

 **Gnuplot**:
   - Download the latest version of Gnuplot for Windows from the [official Gnuplot website](http://www.gnuplot.info/download.html).
   - Follow the installer instructions to complete the installation.
   - Ensure that the `bin` directory of Gnuplot is added to your system's `PATH` environment variable.

### Linux

``` bash
     sudo apt-get update
     sudo apt-get install gnuplot
```

