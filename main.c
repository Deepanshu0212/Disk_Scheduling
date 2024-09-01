#include "Disk_Scheduling.h"

int main(){

    DiskState ds;
    ds.requests = malloc(100 * sizeof(int));
    printf("Enter number of request");
    scanf("%d",&ds.num_requests);

    printf("Enter current position");
    scanf("%d",& ds.head_position);

    printf("Enter request");
    for (int i = 0; i < ds.num_requests; i++) {
        scanf("%d", &ds.requests[i]);
    }

    PerformanceMetrics pf = cscan(&ds);

    printf("total_head_movement : %d" , pf.total_head_movement);
    printf(" avg_waiting_time : %f" , pf.avg_waiting_time);

    plot_results("sstf","disk_scheduling.png");

    system("gnuplot disk_scheduling.plt");

    printf("Graph has been generated as disk_scheduling.png\n");

    return 0;

    
}