#include "Disk_Scheduling.h"

PerformanceMetrics calculate_metrics(DiskState* state, int total_head_movement) {
    PerformanceMetrics metrics;
    metrics.total_head_movement = total_head_movement;
    
    int total_waiting_time = 0;
    int* waiting_times = malloc(state->num_requests * sizeof(int));
    int current_time = 0;
    int current_position = state->head_position;
    
    for (int i = 0; i < state->num_requests; i++) {
        int seek_time = abs(state->requests[i] - current_position);
        current_time += seek_time;
        waiting_times[i] = current_time;
        total_waiting_time += waiting_times[i];
        current_position = state->requests[i];
    }
    
    metrics.avg_waiting_time = (double)total_waiting_time / state->num_requests;
    metrics.throughput = (double)state->num_requests / current_time;
    
    double mean_response_time = metrics.avg_waiting_time;
    double sum_squared_diff = 0;
    
    for (int i = 0; i < state->num_requests; i++) {
        double diff = waiting_times[i] - mean_response_time;
        sum_squared_diff += diff * diff;
    }
    
    metrics.std_dev_response_time = sqrt(sum_squared_diff / state->num_requests);
    
    free(waiting_times);
    return metrics;
}
void plot_results(char* algorithm, char* output_file) {
    FILE *fp = fopen("disk_scheduling.plt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "set terminal png\n");
    fprintf(fp, "set output '%s'\n", output_file);
    fprintf(fp, "set title '%s Disk Scheduling'\n", algorithm);
    fprintf(fp, "set xlabel 'Request Sequence'\n");
    fprintf(fp, "set ylabel 'Cylinder'\n");
    fprintf(fp, "set yrange [0:199]\n");
    fprintf(fp, "set grid ytics\n");
    fprintf(fp, "set ytic 50\n");
    fprintf(fp, "total_movement = system('type total_movement.txt')\n");
    fprintf(fp, "set label sprintf('Total Movement: %%s', total_movement) at graph 0.02, graph 0.98 font ',10' tc rgb 'black'\n");
    fprintf(fp, "plot 'disk_scheduling.dat' with linespoints title 'Head Movement' lt rgb 'green' pt 7 ps 1\n");

    fclose(fp);

}



// Fcfs algo...
PerformanceMetrics fcfs(DiskState *state){

    FILE *fp = fopen("disk_scheduling.dat", "w");
    // if (fp == NULL) {
    //     printf("Error opening file!\n");
    //     return ;
    // }

    int total_head_mov =0;
    int curr_pos = state->head_position;
    fprintf(fp, "%d %d\n", 0, curr_pos);

    for(int i=0;i<state->num_requests;i++){
        
        total_head_mov += abs(state->requests[i] - curr_pos);
        curr_pos = state->requests[i];
        fprintf(fp, "%d %d\n", i+1, curr_pos);
    }
     

    fclose(fp);
    
    fp = fopen("total_movement.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d", total_head_mov);
        fclose(fp);
    }

    return calculate_metrics(state , total_head_mov);
}