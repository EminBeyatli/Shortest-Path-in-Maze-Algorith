#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GRAPH_SEARCH.h"
#include "data_types.h"

/* Prints elapsed search time in a report-friendly format */
void Print_Search_Time(double elapsed_seconds)
{
    printf("\n========== SEARCH TIME ==========\n");
    if (elapsed_seconds < 1e-3)
        printf("  Elapsed time : %.3f ms (%.6f s)\n",
               elapsed_seconds * 1000.0, elapsed_seconds);
    else if (elapsed_seconds < 1.0)
        printf("  Elapsed time : %.2f ms\n", elapsed_seconds * 1000.0);
    else
        printf("  Elapsed time : %.4f s\n", elapsed_seconds);
    printf("=================================\n\n");
}

int main()
{	
    Node root, *goal;
    State *goal_state = NULL;
    enum METHODS method;
    int Max_Level, level;
    float alpha;
    clock_t t_start, t_end;
    double  elapsed;
   
    // This part must be updated if a new algorithm is added. 
    printf("1 --> Breast-First Search\n");
    printf("2 --> Uniform-Cost Search\n");
    printf("3 --> Depth-First Search\n");
    printf("4 --> Depth-Limited Search\n");
    printf("5 --> Iterative Deepening Search\n");
    printf("6 --> Greedy Search\n");
    printf("7 --> A* Search\n");
    printf("8 --> Generalized A* Search\n");
    printf("Select a method to solve the problem: ");
    scanf("%d", &method);
    if(method==DepthLimitedSearch){
        printf("Enter maximum level for depth-limited search : ");                         
        scanf("%d", &Max_Level);                  
    }  
    if(method==GeneralizedAStarSearch){
        printf("Enter value of alpha for Generalized A* Search : ");                         
        scanf("%f", &alpha);                  
    }   
    
    // Creating the root node ... 
    root.parent         = NULL;
    root.path_cost      = 0;
    root.action         = NO_ACTION; // The program will not use this part. (NO_ACTION-->0)
    root.Number_of_Child = 0;
    	
    printf("======== SELECTION OF INITIAL STATE =============== \n");
    root.state = *(Create_State());
    
    if(PREDETERMINED_GOAL_STATE)  // User will determine the goal state if it is true
    {
        printf("======== SELECTION OF GOAL STATE =============== \n"); 
        goal_state = Create_State();
    }
    
    if(method==GreedySearch || method==AStarSearch || method==GeneralizedAStarSearch){
        root.state.h_n = Compute_Heuristic_Function(&(root.state), goal_state);
        if(PREDETERMINED_GOAL_STATE)
            goal_state->h_n = 0;                 
    }  

    /* ── START TIMER ───────────────────────────────────────────────────── */
    t_start = clock();
    	
    switch(method) 
    {
        case BreastFirstSearch: 
        case GreedySearch:               
            goal = First_GoalTest_Search_TREE(method, &root, goal_state);  break; 
        case DepthFirstSearch: 	
        case DepthLimitedSearch: 
            goal = DepthType_Search_TREE(method, &root, goal_state, Max_Level);  break;  
        case IterativeDeepeningSearch:
            for(level=0; TRUE; level++){
                printf("\n--- IDS: trying depth limit %d ---\n", level);
                goal = DepthType_Search_TREE(method, &root, goal_state, level);
                if(goal!=FAILURE){
                    printf("The goal is found in level %d.\n", level); 
                    break;
                }		
            }
            break;     
        case UniformCostSearch: 
        case AStarSearch:   
        case GeneralizedAStarSearch:   
            goal = First_InsertFrontier_Search_TREE(method, &root, goal_state, alpha);  break;      
     
        default: 
            printf("ERROR: Unknown method.\n");  
            exit(-1);    
    }

    /* ── STOP TIMER ────────────────────────────────────────────────────── */
    t_end   = clock();
    elapsed = (double)(t_end - t_start) / CLOCKS_PER_SEC;

    Show_Solution_Path(goal);
    Print_Search_Time(elapsed);
    Visualize_Solution(goal, &root.state, goal_state);
  	
    return 0;
}
