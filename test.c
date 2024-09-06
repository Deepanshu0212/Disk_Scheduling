#include <assert.h>
#include "Disk_Scheduling.h"

void test_fcfs() {
    DiskState state;
    state.num_requests = 5;
    int requests[] = {95, 180, 34, 119, 11};
    state.requests = requests;
    state.head_position = 50;

    PerformanceMetrics metrics = fcfs(&state);
    
    // Expected values
    int expected_total_head_movement = 469;
    
    assert(metrics.total_head_movement == expected_total_head_movement);
    printf("FCFS Test passed!\n");

    plot_results("FCFS","Disk_scheduling.png");
}

void test_sstf() {
    DiskState state;
    state.num_requests = 5;
    int requests[] = {95, 180, 34, 119, 11};
    state.requests = requests;
    state.head_position = 50;

    PerformanceMetrics metrics = sstf(&state);
    
    // Expected values
    int expected_total_head_movement = 208;
    
    assert(metrics.total_head_movement == expected_total_head_movement);
    printf("SSTF Test passed!\n");

    plot_results("SSTF","Disk_scheduling.png");
}

void test_scan() {
    DiskState state;
    state.num_requests = 5;
    int requests[] = {95, 180, 34, 119, 11};
    state.requests = requests;
    state.head_position = 50;

    PerformanceMetrics metrics = scan(&state);
    
    // Expected values
    int expected_total_head_movement = 337;
    
    assert(metrics.total_head_movement == expected_total_head_movement);
    printf("SCAN Test passed!\n");

    plot_results("SCAN","Disk_scheduling.png");
    
}

void test_cscan() {
    DiskState state;
    state.num_requests = 5;
    int requests[] = {95, 180, 34, 119, 11};
    state.requests = requests;
    state.head_position = 50;

    PerformanceMetrics metrics = cscan(&state);
    
    // Expected values
    int expected_total_head_movement = 382;
    
    assert(metrics.total_head_movement == expected_total_head_movement);
    printf("C-SCAN Test passed!\n");

    plot_results("C_SCAN","Disk_scheduling.png");

}

void test_look() {
    DiskState state;
    state.num_requests = 5;
    int requests[] = {95, 180, 34, 119, 11};
    state.requests = requests;
    state.head_position = 50;

    PerformanceMetrics metrics = look(&state);
    
    // Expected values
    int expected_total_head_movement = 299;
    
    assert(metrics.total_head_movement == expected_total_head_movement);
    printf("LOOK Test passed!\n");

    plot_results("LOOK","Disk_scheduling.png");
}

void test_clook() {
    DiskState state;
    state.num_requests = 5;
    int requests[] = {95, 180, 34, 119, 11};
    state.requests = requests;
    state.head_position = 50;

    PerformanceMetrics metrics = clook(&state);
    
    // Expected values
    int expected_total_head_movement = 322;
    
    assert(metrics.total_head_movement == expected_total_head_movement);
    printf("C-LOOK Test passed!\n");

    plot_results("C_LOOK","Disk_scheduling.png");
}

void test_edge_cases() {
    // Test for edge case where all requests are the same
    DiskState state;
    state.num_requests = 3;
    int requests[] = {100, 100, 100};
    state.requests = requests;
    state.head_position = 50;

    PerformanceMetrics fcfs_metrics = fcfs(&state);
    PerformanceMetrics sstf_metrics = sstf(&state);
    PerformanceMetrics scan_metrics = scan(&state);
    PerformanceMetrics cscan_metrics = cscan(&state);
    PerformanceMetrics look_metrics = look(&state);
    PerformanceMetrics clook_metrics = clook(&state);

    // All algorithms should give the same result
    int expected_total_head_movement = 50;
    assert(fcfs_metrics.total_head_movement == expected_total_head_movement);
    assert(sstf_metrics.total_head_movement == expected_total_head_movement);
    assert(scan_metrics.total_head_movement == 149);
    assert(cscan_metrics.total_head_movement == 348);
    assert(look_metrics.total_head_movement == expected_total_head_movement);
    assert(clook_metrics.total_head_movement == expected_total_head_movement);

    printf("Edge cases test passed!\n");
}