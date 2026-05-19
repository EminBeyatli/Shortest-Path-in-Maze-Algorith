#ifndef DATA_TYPES_H
#define DATA_TYPES_H

// ==================== WRITE YOUR OPTIONAL DATA TYPES IF YOU NEED =============

// Labirent boyutu artık runtime'da belirleniyor (Small=20x20, Large=50x50).
// Değerler SpecificToProblem.c içinde set edilip buradan extern olarak okunur.
extern int MAZE_ROWS;
extern int MAZE_COLS;


// ====== WRITE YOUR COMPULSORY (BUT SPECIFIC TO THE PROBLEM) DATA TYPES =======

// Labirentte 4 yön hareketi mümkündür.
enum ACTIONS
{
    Move_Up    = 0,   // Yukarı git  (row - 1)
    Move_Down  = 1,   // Aşağı git   (row + 1)
    Move_Left  = 2,   // Sola git    (col - 1)
    Move_Right = 3    // Sağa git    (col + 1)
};

// State artık bir şehir değil; labirentteki (satır, sütun) konumudur.
typedef struct State  
{
    int   row;   // Satır numarası (0 ... MAZE_ROWS-1)
    int   col;   // Sütun numarası (0 ... MAZE_COLS-1)
    float h_n;   // Heuristic function değeri (A* için)
} State;


// ================== YOU DO NOT NEED TO CHANGE THIS PART ======================

enum METHODS
{
    BreastFirstSearch      = 1,
    UniformCostSearch      = 2,
    DepthFirstSearch       = 3,
    DepthLimitedSearch     = 4,
    IterativeDeepeningSearch = 5,
    GreedySearch           = 6,
    AStarSearch            = 7,
    GeneralizedAStarSearch = 8
};

typedef struct Transition_Model
{
    State new_state;
    float step_cost;
} Transition_Model;

typedef struct Node
{
    State       state;
    float       path_cost;
    enum ACTIONS action;        // Bu düğüme ulaşmak için uygulanan aksiyon
    struct Node *parent;
    int          Number_of_Child; // DFS için gerekli
} Node;

typedef struct Queue  // Frontier (sınır) için kullanılan kuyruk
{
    Node         *node;
    struct Queue *next;
} Queue;

#endif
