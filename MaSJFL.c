#include <stdlib.h>
#include <stdio.h>


struct process {

	int PID;
	int tau;
	double alpha;
	int* actualRT;	
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

void swap (struct process* xp, struct process* yp) {
    
    struct process temp = *xp;
    *xp = *yp;
    *yp = temp;
    
}

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
 * @param array of process pointers
 */
void shortestJobFirst(int ticks, int processes, struct process* procArr[]) {
    printf("shortest job first lets gooo\n");
    int tick;
    int proc;
    int i;
    struct process* min;
    
    
    for(tick = 0; tick < ticks; tick++){
        int sum = 0;
        printf("Simulating %d tick of processes at time %d\n", tick, time);
        bubbleSort(tick, procArr, processes);
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
     printf("end shortest job first\n");
   
    }
     

int main(int argc, char **argv) {

    char *filename;
    int processes;
    int ticks;
    
    if (argc != 2) {
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
            
            shortestJobFirst(ticks, processes, SYS_PROCESS);
            fclose(readFile);
        }
             // if filename doesn't end with ".txt"
         else {
        printf("Invalid file type\n");
         }
    }
    
    return (EXIT_SUCCESS);
}
