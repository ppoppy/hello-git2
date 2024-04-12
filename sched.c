#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum process_state{
    RUNNING,
    READY,
    TERMINATED,
}STATE;

typedef struct process{
    char* name;
    int size;
    int exec_time;
    int time_slice;
    STATE state;
}PROC;

typedef struct  list_node{
       PROC* proc;
    struct list_node* next;
}NODE;

//선언
PROC* proc_arr;
NODE* rq;
PROC* running;

PROC *init(){ 
    
    FILE *work_file;
    char work_str[100];
    work_file = fopen("workload.txt","r");
    if(work_file == NULL){
        printf("파일열기 실패\n");
        return 0;
    }
    else{
        int count=-1;
        fgets(work_str,100,work_file); 
        if(count==-1){
            count=atoi(work_str); 
        }
        proc_arr=malloc(sizeof(PROC)*count);
        NODE* current = rq;
        int i=0;
        while(feof(work_file)==0){
            fgets(work_str,100,work_file); 
            char *str;
            proc_arr[i].name = malloc(strlen(work_str) + 1);
            strcpy(proc_arr[i].name, strtok(work_str, " "));
            str=strtok(NULL," ");
            
            proc_arr[i].size = atoi(str);
            proc_arr[i].exec_time = 0;
            proc_arr[i].time_slice = 10;
            proc_arr[i].state = READY;
            NODE* new_node = malloc(sizeof(NODE));
            new_node->proc = &proc_arr[i];
            new_node->next = NULL;
            // 리스트에 노드 추가
            if (current == NULL) {
                rq = new_node;
                current = rq;
            } else {
                current->next = new_node;
                current = current->next;
            }
            i++;
            
        }
        fclose(work_file);
        return proc_arr;
    }
    
}
void print_running(int* time){
    NODE* current_node=rq;
    NODE* timeslice_zero=malloc(sizeof(NODE));
    printf("\n%d ms: ", *time);
    //run
    if(running!=NULL){
        if(running->time_slice==0){
            
            //rq=rq->proc;
            if(running->size > running->exec_time){ //running : p3, rq: p4    
                running->state=READY;
                running->time_slice=10;
                printf("\nrunning: %s, runningtime: %d\n",running->name,running->time_slice);
                if(current_node!=NULL){ //c: p4
                    printf("run %s, [",current_node->proc->name);
                    current_node=current_node->next;
                    if(current_node!=NULL){
                        while(current_node->next!=NULL){
                            printf("%s, ",current_node->proc->name);
                            current_node=current_node->next;
                        }
                        printf("%s, %s]",current_node->proc->name, running->name);
                        timeslice_zero->proc=running;
                        timeslice_zero->next=NULL;
                        current_node->next=timeslice_zero;
                        running=rq->proc;
                        rq=rq->next;
                    }
                    else{
                        printf("%s]",running->name);
                        printf("\n\ntimeslice: %d, rq timeslice: %d\n\n",running->time_slice,rq->proc->time_slice);
                        timeslice_zero->proc=running;
                        timeslice_zero->next=NULL;
                        current_node=timeslice_zero;
                        printf("c: %d, rq: %d",current_node->proc->time_slice,rq->proc->time_slice);
                        running=rq->proc;
                        rq=rq->next;
                        
                    }

                    printf("running: %s, rq: %s",running->name,current_node->proc->name);
                }
                else{ //timeslice = 0인데 다 못햇을 때, rq가 없을 때
                    running->time_slice=10;
                    printf("run %s, []",running->name);
                }
            }else {
                running->state=TERMINATED;
                //printf("run %s, [",current_node->proc->name);
                if(current_node!=NULL){
                    printf("run %s, [",current_node->proc->name);
                    if(current_node->next!=NULL){
                        current_node=current_node->next;
                        if(current_node!=NULL){
                            while(current_node->next!=NULL){
                                printf("%s, ",current_node->proc->name);
                                current_node=current_node->next;
                            }
                            printf("%s], terminated: %s",current_node->proc->name,running->name);
                            running=rq->proc;
                            rq=rq->next;
                        }
                    }         
                    else{
                        printf("], terminated %s",running->name);
                        running=current_node->proc;
                        current_node=NULL;
                        rq=NULL;
                    }
                    
       
                }
                else{
                    printf("end of execution, terminated: %s",running->name);
                    running=NULL;
                }
                // if(current_node!=NULL){

                // }
                // if(current_node->next!=NULL){
                //     current_node=current_node->next;
                //     if(current_node!=NULL){
                //         while(current_node->next!=NULL){
                //             printf("%s, ",current_node->proc->name);
                //             current_node=current_node->next;
                //         }
                //         printf("%s], terminated: %s",current_node->proc->name,running->name);
                //         running=rq->proc;
                //         rq=rq->next;
                //     }
                // }
                // else{
                //     running=current_node->proc;
                //     rq=rq->next;
                // }
                // if(current_node!=NULL){
                //     while(current_node->next!=NULL){
                //         printf("%s, ",current_node->proc->name);
                //         current_node=current_node->next;
                //     }
                //     printf("%s]",current_node->proc->name);
                //     printf(", terminated: %s",running->name);
                //     rq=rq->next;
                // }
                // else{
                //     printf("end of execution, terminated: %s",running->name);
                //     running=NULL;
                    
                // }
            }
        }
        else{
            if(running->size == running->exec_time){ //running :pppp, rq : p1
                running->state=TERMINATED;
                if(current_node!=NULL){
                    printf("run %s, [",current_node->proc->name);
                    if(current_node->next!=NULL){
                        current_node=current_node->next;
                        if(current_node!=NULL){
                            while(current_node->next!=NULL){
                                printf("%s, ",current_node->proc->name);
                                current_node=current_node->next;
                            }
                            printf("%s], terminated: %s",current_node->proc->name,running->name);
                            running=rq->proc;
                            rq=rq->next;
                        }
                    }         
                    else{
                        printf("], terminated %s",running->name);
                        running=current_node->proc;
                        current_node=NULL;
                        rq=NULL;
                    }
                    
       
                }
                else{
                    printf("end of execution, terminated: %s",running->name);
                    running=NULL;
                }
            }
            else{ //timeslice 가 남아있고 작동해야할 시간이 남아잇을때
                printf("run %s, [",running->name);
                if(current_node!=NULL){
                    while(current_node->next!=NULL){
                        printf("%s, ",current_node->proc->name);
                        current_node=current_node->next;
                    }
                    printf("%s]",current_node->proc->name);

                }
                
            }
            
        }
    }
    printf("\n");
    
}

int sched(int* time){
    //정보 프린트
    NODE* current=rq;
    if(running!=NULL){
       printf("running: %s, runtime: %d, timeslice : %d\n",running->name, running->exec_time,running->time_slice);

        if(rq!=NULL){
            printf("current: ");
            while(current->next!=NULL){
                printf("%s, ",current->proc->name);
                current=current->next;
            }
            printf("%s\n",current->proc->name);
        }

    }



    if(running==NULL){
        if(rq!=NULL){
            running=rq->proc;
            running->state=RUNNING;
            if(rq->next!=NULL) rq=rq->next;
            else rq=NULL;
            // if(rq==NULL) printf("null");
            // else printf("%s\n",rq->proc->name);
            print_running(time);
            running->exec_time++;
            running->time_slice--;
            (*time)++;
            //printf("\n1. time: %d, running: %s, extime: %d, timeslice: %d\n", *time, running->name, running->exec_time, running->time_slice);
        
        }
        else{
            return (running!=NULL||rq!=NULL);
        }
    }
    else{ //running != NULL 일 때,
        
        if(running->time_slice > 0){
            if(running->size > running->exec_time){
                // printf("\n2.time: %d, running: %s, extime: %d, timeslice: %d\n", *time, running->name, running->exec_time, running->time_slice);
                // printf("\nnext: %s",rq->proc->name);
                running->exec_time++;
                running->time_slice--;
                (*time)++;
            }
            else{
                if(rq!=NULL){
                    running->state=TERMINATED;
                    print_running(time);
                    //rq_change(time);
                    //printf("\nrunning:%s\n",running->name);
                    running->exec_time++;
                    running->time_slice--;
                    (*time)++;
                    //printf("\nhere? time: %d, running: %s, extime: %d, timeslice: %d\n", *time, running->name, running->exec_time, running->time_slice);
                }
                else{
                    print_running(time);
                    return (running!=NULL||rq!=NULL);
                }
            }
        }
        else{//time_slice가 없을 때
            
            if(running->size > running->exec_time){
                if(rq!=NULL){
                    print_running(time);
                    printf("\n\nrun:%s, time:%d",running->name,running->time_slice);
                    
                    running->exec_time++;
                    running->time_slice--;  
                    (*time)++;
                    //printf("\n there ? time: %d, running: %s, extime: %d, timeslice: %d\n", *time, running->name, running->exec_time, running->time_slice);

                }
                else{
                    //안해도딜거같은디
                }
                      
            }
            else{
                if(rq!=NULL){
                    //printf("here perfect");
                    print_running(time);
                    //rq_change(time);
                    running->exec_time++;
                    running->time_slice--;
                    (*time)++;
                }
                else{
                    return (running!=NULL||rq!=NULL);
                }
                
            }
            
        }
    }
    return (running!=NULL||rq!=NULL);
}

int main() {
    rq=NULL;
    running=NULL;
    proc_arr=init();
    int time=0;
    

    NODE* current = rq;
    printf("init: [");
    while(current->next != NULL){
        printf("%s ,",current->proc->name);
        current=current->next;
    }
    printf("%s]",current->proc->name);
    for(int i=1; i<131; i++){
        sched(&time);
    }
    


    //while(sched(&time));

    return 0;

}