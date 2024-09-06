#include "../Header/Disk_Scheduling.h"

typedef PerformanceMetrics (*DiskSchedulingAlgo)(DiskState*);

void display_main_menu() {
    printf("Select an option:\n");
    printf("1. Simulate Disk Scheduling Algorithms\n");
    printf("2. Run Test Cases\n");
    printf("0. Exit\n");
}

void display_simulation_menu() {
    printf("Select a Disk Scheduling Algorithm:\n");
    printf("1. FCFS\n");
    printf("2. SSTF\n");
    printf("3. SCAN\n");
    printf("4. C-SCAN\n");
    printf("5. LOOK\n");
    printf("6. C-LOOK\n");
    printf("0. Exit\n");
}

void display_test_menu() {
    printf("Select a Disk Scheduling Algorithm to test:\n");
    printf("1. Test FCFS\n");
    printf("2. Test SSTF\n");
    printf("3. Test SCAN\n");
    printf("4. Test C-SCAN\n");
    printf("5. Test LOOK\n");
    printf("6. Test C-LOOK\n");
    printf("7. Test Edge Cases\n");
    printf("0. Exit\n");
}

int get_choice(int min, int max) {
    int choice;
    do {
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice < min || choice > max) {
            printf("Invalid choice. Please enter a number between %d and %d.\n", min, max);
        }
    } while (choice < min || choice > max);
    return choice;
}

void handle_algorithm(DiskSchedulingAlgo algo, char* algo_name, DiskState* state) {
    PerformanceMetrics metrics = algo(state);
    printf("%s Algorithm:\n", algo_name);
    printf("Total Head Movement: %d\n", metrics.total_head_movement);
    printf("Average Waiting Time: %.2f\n", metrics.avg_waiting_time);
    printf("Throughput: %.2f\n", metrics.throughput);
    printf("Standard Deviation of Response Time: %.2f\n", metrics.std_dev_response_time);

    plot_results("FCFS","Information/Disk_scheduling.png");
    
}


void simulate_algorithm(int choice, DiskState* state) {
    switch (choice) {
        case 1:
            fcfs_description();
            handle_algorithm(fcfs, "FCFS", state);
            break;
        case 2:
            sstf_description();
            handle_algorithm(sstf, "SSTF", state);
            break;
        case 3:
            scan_description();
            handle_algorithm(scan, "SCAN", state);
            break;
        case 4:
            cscan_description();
            handle_algorithm(cscan, "C-SCAN", state);
            break;
        case 5:
            look_description();
            handle_algorithm(look, "LOOK", state);
            break;
        case 6:
            clook_description();
            handle_algorithm(clook, "C-LOOK", state);
            break;
        case 0:
            printf("Exiting simulation...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

void run_test_cases() {
    int test_choice;
    do {
        display_test_menu();
        test_choice = get_choice(0, 7);

        switch (test_choice) {
            case 1:
                test_fcfs();
                break;
            case 2:
                test_sstf();
                break;
            case 3:
                test_scan();
                break;
            case 4:
                test_cscan();
                break;
            case 5:
                test_look();
                break;
            case 6:
                test_clook();
                break;
            case 7:
                test_edge_cases();
                break;
            case 0:
                printf("Exiting test cases...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (test_choice != 0);
}

void get_user_inputs(DiskState* state) {
    int num_requests;

    // Get number of requests
    do {
        printf("Enter the number of disk requests (max %d): ", MAX_REQUESTS);
        scanf("%d", &num_requests);
        if (num_requests < 1 || num_requests > MAX_REQUESTS) {
            printf("Invalid number of requests. Please enter a value between 1 and %d.\n", MAX_REQUESTS);
        }
    } while (num_requests < 1 || num_requests > MAX_REQUESTS);

    state->num_requests = num_requests;
    state->requests = (int*)malloc(num_requests * sizeof(int));

    // Input validation for head position
    do {
        printf("Enter the current head position (0 to %d): ", MAX_CYLINDER);
        scanf("%d", &state->head_position);
        if (state->head_position < 0 || state->head_position > MAX_CYLINDER) {
            printf("Invalid head position. Please enter a value between 0 and %d.\n", MAX_CYLINDER);
        }
    } while (state->head_position < 0 || state->head_position > MAX_CYLINDER);

    // Ask if random requests should be generated
    int random_choice;
    do {
        printf("Do you want to generate random requests? (1 for Yes, 0 for No): ");
        scanf("%d", &random_choice);
        if (random_choice != 0 && random_choice != 1) {
            printf("Invalid choice. Please enter 1 for Yes or 0 for No.\n");
        }
    } while (random_choice != 0 && random_choice != 1);

    if (random_choice == 1) {
        generate_random_requests(state, num_requests);
    } else {
        // Manually enter requests
        printf("Enter the cylinder requests (between 0 and %d):\n", MAX_CYLINDER);
        for (int i = 0; i < num_requests; i++) {
            do {
                printf("Request %d: ", i + 1);
                scanf("%d", &state->requests[i]);
                if (state->requests[i] < 0 || state->requests[i] > MAX_CYLINDER) {
                    printf("Invalid request. Please enter a value between 0 and %d.\n", MAX_CYLINDER);
                }
            } while (state->requests[i] < 0 || state->requests[i] > MAX_CYLINDER);
        }
    }
}

int main() {
    DiskState state;
    int main_choice;
    
    printf("Welcome to the Disk Scheduling Algorithm Simulator!\n");
    printf("This program allows you to simulate various disk scheduling algorithms.\n");
    
    do {
        display_main_menu();
        main_choice = get_choice(0, 2);

        switch (main_choice) {
            case 1:
                get_user_inputs(&state);  // Get user inputs for simulation
                int sim_choice;
                do {
                    display_simulation_menu();
                    sim_choice = get_choice(0, 6);
                    simulate_algorithm(sim_choice, &state);
                } while (sim_choice != 0);
                break;

            case 2:
                run_test_cases();  // Run the selected test cases
                break;

            case 0:
                printf("Exiting the program...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (main_choice != 0);

    // Free dynamically allocated memory
    free(state.requests);
    return 0;
}



