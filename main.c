#include "Disk_Scheduling.h"

void display_menu() {
    printf("Select a Disk Scheduling Algorithm:\n");
    printf("1. FCFS\n");
    printf("2. SSTF\n");
    printf("3. SCAN\n");
    printf("4. C-SCAN\n");
    printf("5. LOOK\n");
    printf("6. C-LOOK\n");
    printf("0. Exit\n");
}

int get_user_input(){
    int choice;

    do{
       printf("Enter your choice: ");
       scanf("%d", &choice);

       if(choice <0 || choice >6){
         printf("Invalid choice. Please enter a number between 0 and 6.\n");
       }

    } while(choice <0 || choice >6);

    return choice;
}

int main(){
    
    printf("Welcome to the Disk Scheduling Algorithm Simulator!\n");
    printf("This program allows you to simulate various disk scheduling algorithms.\n");
    printf("You can choose to manually enter the disk requests or generate them randomly.\n\n");

    DiskState state;
    int num_requests;

    do {
        printf("Enter the number of disk requests (max %d): ", MAX_REQUESTS);
        scanf("%d", &num_requests);

        if (num_requests < 1 || num_requests > MAX_REQUESTS) {
            printf("Invalid number of requests. Please enter a value between 1 and %d.\n", MAX_REQUESTS);
        }
    } while (num_requests < 1 || num_requests > MAX_REQUESTS);

    state.num_requests = num_requests;
    state.requests = (int*)malloc(num_requests * sizeof(int));

    // Input validation for head position
    do {
        printf("Enter the current head position (0 to %d): ", MAX_CYLINDER);
        scanf("%d", &state.head_position);

        if (state.head_position < 0 || state.head_position > MAX_CYLINDER) {
            printf("Invalid head position. Please enter a value between 0 and %d.\n", MAX_CYLINDER);
        }
    } while (state.head_position < 0 || state.head_position > MAX_CYLINDER);

    int random_choice;
    do {
        printf("Do you want to generate random requests? (1 for Yes, 0 for No): ");
        scanf("%d", &random_choice);

        if (random_choice != 0 && random_choice != 1) {
            printf("Invalid choice. Please enter 1 for Yes or 0 for No.\n");
        }
    } while (random_choice != 0 && random_choice != 1);

    if (random_choice == 1) {
        generate_random_requests(&state, num_requests);
    } else {
        printf("Enter the cylinder requests (between 0 and %d):\n", MAX_CYLINDER);
        for (int i = 0; i < num_requests; i++) {
            do {
                printf("Request %d: ", i + 1);
                scanf("%d", &state.requests[i]);

                if (state.requests[i] < 0 || state.requests[i] > MAX_CYLINDER) {
                    printf("Invalid request. Please enter a value between 0 and %d.\n", MAX_CYLINDER);
                }
            } while (state.requests[i] < 0 || state.requests[i] > MAX_CYLINDER);
        }
    }
    
    int choice;
    do {
        display_menu();
        choice = get_user_input();

        switch (choice) {
            case 1:
                handle_algorithm(fcfs, "FCFS", &state);
                fcfs_description();
                break;
            case 2:
                handle_algorithm(sstf, "SSTF", &state);
                sstf_description();
                break;
            case 3:
                handle_algorithm(scan, "SCAN", &state);
                scan_description();
                break;
            case 4:
                handle_algorithm(cscan, "C-SCAN", &state);
                cscan_description();
                break;
            case 5:
                handle_algorithm(look, "LOOK", &state);
                look_description();
                break;
            case 6:
                handle_algorithm(clook, "C-LOOK", &state);
                clook_description();
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);

    free(state.requests);
    return 0;


    
}