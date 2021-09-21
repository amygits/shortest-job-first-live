#include <stdlib.h>
#include <stdio.h>


struct process {

	int PID;
	int tau;
	double alpha;
	int* actualRT;	
        int est; // est. runtime for live runs
};


struct process* SYS_PROCESS[];
struct process* shortest[];
int time = 0;
int turnaround = 0;
int waiting = 0;


/*
 Creates a process with a size ticks array of runtime data
 */
struct process* process_init(int ticks) {
    
    printf("Initializing process..");
    struct process* newProcess = (struct process*) malloc(sizeof(newProcess));
    int *runtime = calloc(ticks, sizeof(int));
    newProcess->actualRT = runtime;
    printf("Initializing process complete.\n");
    return newProcess;
}

// Reads process data from file and stores into a process 
void readProcessData(FILE *file, int ticks, struct process* process) {
    
    printf("Reading process data..");
    int i;
    fscanf(file, "%d", &process->PID);
    //printf("PID: %d\n", process->PID);
    fscanf(file, "%d", &process->tau);
    //printf("TAU: %d\n", process->tau);
    fscanf(file, "%lf", &process->alpha);
    //printf("ALPHA: %.1lf\n", process->alpha);
    for(i = 0; i < ticks; i++){
        fscanf(file, "%d", &process->actualRT[i]);
        //printf("RUNTIME #%d: %d\n", i, process->actualRT[i]);
    }
    printf("Reading process data complete\n");
    
}

/*
 * Swaps two struct processes*
 */
void swap (struct process* xp, struct process* yp) {
    
    struct process temp = *xp;
    *xp = *yp;
    *yp = temp;
    
}

/*
 * Sorts an array of struct process pointers according to actual runtime at tick #
 */
void bubbleSort(int tick, struct process* procArr[], int processes){
    
    //printf("bubble sort\n");
    int i, j;
    for (i = 0; i < processes-1; i++){
        for (j = 0; j < processes-i-1; j++) {
            if (procArr[j]->actualRT[tick] > procArr[j + 1]->actualRT[tick]) {
                swap(&procArr[j], &procArr[j+1]);
            } 
        }
    }
}


/*
 * Calculates and displays shortest job first from dataset of processes
 * Also displays turnaround and waiting times
 * @param array of process pointers
 */
void shortestJobFirst(int ticks, int processes, struct process* procArr[]) {
    
    printf("Starting shortest jobs first simulation..\n");
    int tick;
    int proc;
    
    // For each tick
    for(tick = 0; tick < ticks; tick++){
        int sum = 0;
        printf("Simulating %d tick of processes at time %d\n", tick, time);
        
        // Sort process array based on actual runtime @ tick value
        bubbleSort(tick, procArr, processes);
        
        // Running through each process in process array
        for(proc = 0; proc < processes; proc++){
            printf("Process %d took %d.\n", procArr[proc]->PID, procArr[proc]->actualRT[tick]);
            time = time + procArr[proc]->actualRT[tick];
            sum = sum + procArr[proc]->actualRT[tick];
        }
        turnaround = turnaround + sum + procArr[0]->actualRT[tick];
        waiting = waiting + procArr[0]->actualRT[tick];
    }
    
    printf("Turnaround: %d\n", turnaround);
    printf("Waiting: %d\n", waiting);
    printf("End shortest job first sim.\n");
   
    }
     

void shortestJobFirstLive(int ticks, int processes, struct process* procArr[]) {
    printf("Starting shortest jobs first simulation..\n");
    int tick;
    int proc;
    
}


int main(int argc, char **argv) {

    char *filename;
    int processes;
    int ticks;
    
    if (argc != 2) {
        // Fail case output
        printf("Usage: %s <filename>\n", argv[0]);
    }
    
    else {
        filename = argv[1];
        printf("Filename: %s\n", filename);
        
        int len = strlen(filename);
        
        // if filename ends with ".txt"
        if(strcmp(&filename[len - 4], ".txt") == 0) {
        
            printf("Opening file...");
            FILE* readFile = fopen(filename, "r");
            printf("File opened.\n");
            printf("Attempting read...\n");
            fscanf(readFile, "%d %d", &ticks, &processes);
            //printf("ticks: %d  processes: %d\n", ticks, processes);
            
            int i;
            for (i = 0; i < processes; i++){
                SYS_PROCESS[i] = process_init(ticks);
                readProcessData(readFile, ticks, SYS_PROCESS[i]);
            }
            printf("Read complete\n");
            
            // run shortest jobs first 
            shortestJobFirst(ticks, processes, SYS_PROCESS);
            //reset timer
            time = 0;
            
            fclose(readFile);
        }
             // if filename doesn't end with ".txt"
         else {
        printf("Invalid file type\n");
         }
    }
    
    return (EXIT_SUCCESS);
}
