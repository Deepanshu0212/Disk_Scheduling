#include "Disk_Scheduling.h"

PerformanceMetrics fcfs(DiskState *state){
    int total_head_mov =0;
    int curr_pos = state->head_position;

    for(int i=0;i<state->num_requests;i++){
        
        total_head_mov += abs(state->requests[i] - curr_pos);
        curr_pos = state->requests[i];
    }

    return calculate_metrics(state , total_head_mov);
}