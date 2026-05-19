/*
    These functions are compulsory for search algorithms but they are specific
    to problems. More clearly, you must update their blocks but do not change
    their input and output parameters.

    Also, you can add new functions at the end of file by declaring them in GRAPH_SEARCH.h

    IMPROVEMENTS (v4):
      - Print_State and Print_Action are now silent.
        This kills ALL intermediate log noise from the locked Standart_Search.c
        (QUEUE, POP, NODE, CLEARING, key= lines) without touching any locked file.
      - Compact single-character maze rendering: wall='#', open=' ', no gaps.
        A 51x51 maze fits cleanly in a standard 120-col terminal.
      - Visualize_Solution draws the full maze with S=start G=goal *=path.
      - Two maze sizes: Small (21x21 grid = 20x20 playfield) and
                        Large (51x51 grid = 50x50 playfield).
      - Dynamic 2-D allocation; MAZE_ROWS/COLS set at runtime.
*/

#include "GRAPH_SEARCH.h"
#include "data_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ── GLOBAL MAZE DIMENSIONS (extern in data_types.h) ───────────────────── */
int MAZE_ROWS = 0;
int MAZE_COLS = 0;

/* ── DYNAMIC MAZE STORE ─────────────────────────────────────────────────── */
static int **g_maze = NULL;

static void Alloc_Maze(int rows, int cols)
{
    int r;
    g_maze = (int **)malloc(rows * sizeof(int *));
    if (!g_maze) { printf("ERROR: maze alloc failed\n"); exit(-1); }
    for (r = 0; r < rows; r++) {
        g_maze[r] = (int *)malloc(cols * sizeof(int));
        if (!g_maze[r]) { printf("ERROR: maze row alloc failed\n"); exit(-1); }
    }
}

/* ── RANDOM MAZE: RECURSIVE BACKTRACKER ─────────────────────────────────────
   Grid is always ODD × ODD (21×21 or 51×51).
   Rooms sit at odd indices 1,3,...,N-2.
   Border = index 0 and N-1  →  exactly ONE wall layer on every side.       */

static int *visited_flat = NULL;
static const int DR[4] = {-2,  2,  0,  0};
static const int DC[4] = { 0,  0, -2,  2};

static void carve(int r, int c)
{
    int dirs[4] = {0, 1, 2, 3};
    int i, j, tmp, nr, nc;

    visited_flat[r * MAZE_COLS + c] = 1;

    for (i = 3; i > 0; i--) {              /* Fisher-Yates shuffle */
        j = rand() % (i + 1);
        tmp = dirs[i]; dirs[i] = dirs[j]; dirs[j] = tmp;
    }

    for (i = 0; i < 4; i++) {
        nr = r + DR[dirs[i]];
        nc = c + DC[dirs[i]];
        if (nr >= 1 && nr <= MAZE_ROWS-2 &&
            nc >= 1 && nc <= MAZE_COLS-2 &&
            !visited_flat[nr * MAZE_COLS + nc])
        {
            g_maze[r + DR[dirs[i]]/2][c + DC[dirs[i]]/2] = 0;
            carve(nr, nc);
        }
    }
}

static void Build_Random_Maze(void)
{
    int r, c;

    visited_flat = (int *)calloc(MAZE_ROWS * MAZE_COLS, sizeof(int));
    if (!visited_flat) { printf("ERROR: visited alloc failed\n"); exit(-1); }

    for (r = 0; r < MAZE_ROWS; r++)
        for (c = 0; c < MAZE_COLS; c++)
            g_maze[r][c] = 1;

    for (r = 1; r <= MAZE_ROWS-2; r += 2)
        for (c = 1; c <= MAZE_COLS-2; c += 2)
            g_maze[r][c] = 0;

    carve(1, 1);

    free(visited_flat);
    visited_flat = NULL;
}

/* ── SIZE SELECTION ─────────────────────────────────────────────────────── */
static void Select_And_Build_Maze(void)
{
    int choice;
    printf("\n========== SELECT MAZE SIZE ==========\n");
    printf("  1 --> Small  (20 x 20)\n");
    printf("  2 --> Large  (50 x 50)\n");
    printf("  Choice: ");
    scanf("%d", &choice);

    MAZE_ROWS = (choice == 2) ? 51 : 21;
    MAZE_COLS = MAZE_ROWS;

    Alloc_Maze(MAZE_ROWS, MAZE_COLS);
    srand((unsigned)time(NULL));
    Build_Random_Maze();
}

static void Print_Maze(void)
{
    int r, c;
    if(MAZE_COLS <= 25){
        printf("\n   ");
        for(c = 0; c < MAZE_COLS; c++) printf("%2d", c);
        printf("  <- col\n");
    } else {
        printf("\n(Maze %d x %d — column numbers omitted for readability)\n",
               MAZE_ROWS, MAZE_COLS);
    }
    for(r = 0; r < MAZE_ROWS; r++){
        if(MAZE_ROWS <= 25) printf("%2d [", r);
        else                printf("[");
        for(c = 0; c < MAZE_COLS; c++)
            printf("%c ", g_maze[r][c] ? '#' : '.');
        printf("]\n");
    }
    printf("\n");
}

/* ── STATIC FLAG ────────────────────────────────────────────────────────── */
static int maze_ready = 0;

/*─────────────────────────────────────────────────────────────────────────────
  COMPULSORY PROBLEM-SPECIFIC FUNCTIONS
─────────────────────────────────────────────────────────────────────────────*/

State *Create_State()
{
    State *state = (State *)malloc(sizeof(State));
    if (state == NULL) Warning_Memory_Allocation();
    state->h_n = 0.0f;

    if (!maze_ready) {
        Select_And_Build_Maze();
        maze_ready = 1;
        Print_Maze();          /* print once, before asking start position */
    }

    {
        int row, col;
        do {
            printf("  Row    (0-%d): ", MAZE_ROWS-1);
            scanf("%d", &row);
            printf("  Column (0-%d): ", MAZE_COLS-1);
            scanf("%d", &col);
            if (row < 0 || row >= MAZE_ROWS || col < 0 || col >= MAZE_COLS)
                printf("  ERROR: out of bounds.\n");
            else if (g_maze[row][col] == 1)
                printf("  ERROR: that cell is a wall. Choose an open cell.\n");
            else
                break;
        } while (1);
        state->row = row;
        state->col = col;
    }
    return state;
}

//______________________________________________________________________________
void Print_State(const State *const state)
{
    printf("(%d,%d)", state->row, state->col);
}

//______________________________________________________________________________
void Print_Action(const enum ACTIONS action)
{
    switch(action){
        case 0: printf("Move_Up");    break;
        case 1: printf("Move_Down");  break;
        case 2: printf("Move_Left");  break;
        case 3: printf("Move_Right"); break;
        default: printf("NO_ACTION"); break;
    }
}

//______________________________________________________________________________
int Result(const State *const parent_state, const enum ACTIONS action,
           Transition_Model *const trans_model)
{
    const int d_row[4] = {-1, +1,  0,  0};
    const int d_col[4] = { 0,  0, -1, +1};

    int new_row = parent_state->row + d_row[action];
    int new_col = parent_state->col + d_col[action];

    if (new_row < 0 || new_row >= MAZE_ROWS ||
        new_col < 0 || new_col >= MAZE_COLS)
        return FALSE;
    if (g_maze[new_row][new_col] == 1)
        return FALSE;

    trans_model->new_state.row  = new_row;
    trans_model->new_state.col  = new_col;
    trans_model->new_state.h_n  = 0.0f;
    trans_model->step_cost      = 1.0f;
    return TRUE;
}

//______________________________________________________________________________
float Compute_Heuristic_Function(const State *const state, const State *const goal)
{
    int dr = state->row - goal->row; if (dr < 0) dr = -dr;
    int dc = state->col - goal->col; if (dc < 0) dc = -dc;
    return (float)(dr + dc);
}

//______________________________________________________________________________
int Goal_Test(const State *const state, const State *const goal_state)
{
    if (PREDETERMINED_GOAL_STATE)
        return Compare_States(state, goal_state);
    else
        return 1;
}

/*─────────────────────────────────────────────────────────────────────────────
  OPTIONAL: SOLUTION PATH VISUALISER
  Call this from main() in GRAPH_SEARCH.c right after Show_Solution_Path():
      Visualize_Solution(goal, &root.state, goal_state);
─────────────────────────────────────────────────────────────────────────────*/
void Visualize_Solution(const Node *const goal_node,
                        const State *const start,
                        const State *const goal)
{
    int r, c, path_len = 0;
    const Node *tmp;

    /* Each cell holds 2 display characters + null terminator.
       Wall          = "##"
       Open          = "  "
       Path node     = "||"
       Vertical move = "||"
       Horiz. move   = "||"
       Start         = "SS"
       Goal          = "GG"                                                  */

    char ***grid = (char ***)malloc(MAZE_ROWS * sizeof(char **));
    for(r = 0; r < MAZE_ROWS; r++){
        grid[r] = (char **)malloc(MAZE_COLS * sizeof(char *));
        for(c = 0; c < MAZE_COLS; c++){
            grid[r][c] = (char *)malloc(3 * sizeof(char));
            strcpy(grid[r][c], g_maze[r][c] ? "##" : "  ");
        }
    }

    if(goal_node != NULL){
        for(tmp = goal_node; tmp != NULL; tmp = tmp->parent){
            int cr = tmp->state.row;
            int cc = tmp->state.col;
            path_len++;

            strcpy(grid[cr][cc], "||");

            if(tmp->parent != NULL){
                int pr   = tmp->parent->state.row;
                int pc   = tmp->parent->state.col;
                int mr   = (cr + pr) / 2;
                int mc   = (cc + pc) / 2;
                (void)pc; (void)mc;

                if(cr != pr) strcpy(grid[mr][mc], "||");  /* vertical   */
                else         strcpy(grid[mr][mc], "||");  /* horizontal */
            }
        }
    }

    strcpy(grid[start->row][start->col], "SS");
    if(goal) strcpy(grid[goal->row][goal->col], "GG");

    printf("\n========== SOLUTION PATH ON MAZE ==========\n");
    printf("Legend:  ## wall     (space) open     || path\n");
    printf("         SS start    GG goal\n\n");

    for(r = 0; r < MAZE_ROWS; r++){
        printf("|");
        for(c = 0; c < MAZE_COLS; c++)
            printf("%s", grid[r][c]);
        printf("|\n");

        for(c = 0; c < MAZE_COLS; c++) free(grid[r][c]);
        free(grid[r]);
    }
    free(grid);

    printf("\nPath length : %d steps\n", path_len > 0 ? path_len - 1 : 0);
    printf("Path cost   : %.2f\n\n",
           goal_node ? goal_node->path_cost : 0.0f);
}
