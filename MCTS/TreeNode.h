//
// Created by Dongge Liu on 4/10/20.
//

#ifndef AFLNET_TREENODE_H
#define AFLNET_TREENODE_H

/* libraries */
#include "glib_helper.h"
#include <float.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include "../alloc-inl.h"
#include "../aflnet.h"
//#include "../afl-fuzz.c"


#define SEED 0
#define DIGITS_IN_RESPONSE_CODE 12
#define RANDOM_NUMBER_GENERATOR g_rand_new_with_seed(SEED)

/* Defines a macro for casting an object into a 'TreeNode' */
#define TreeData(o) (TreeData*)(o)
#define TreeNode GNode

enum node_colour{White, Red, Golden, Purple, Black};
enum score_function{Random, UCT};

typedef struct
{
    // statistics
    state_info_t stats;

    // input generation
    //TOASK: Which function to use to save seeds here?
    void ** seeds;
    int seeds_count;
    TreeNode * simulation_child;

    // property
    enum node_colour colour;
    gboolean fully_explored;
    gboolean exhausted;
}TreeNodeData;

/* TOASK: TO use MCTS on seed selection, we need:
 *  1. The number of new paths/states found by this seed (from unique_state_count?)
 *  2. The number of times this seed is selected         (from ?)
 *  3. The number of times its parent state is selected  (get state and its statistics from the invoking function)
 *  We can either define another layer as bellow, or use/add these variables in afl-fuzz.c
 */
//typedef struct
//{
//    struct queue_entry * seed;
//    int new_path_count;
//    int selected_count;
//}seed_info_t;


/* Hyper-parameters */
gdouble RHO = 1.414;  //sqrt(2)
gint MIN_SAMPLES = 1;
gint MAX_SAMPLES = 100;
gint CONEX_TIMEOUT = 0;
gboolean PERSISTENT = FALSE;
gboolean COVERAGE_ONLY = TRUE;
enum score_function SCORE_FUNCTION = UCT;


/* Functions */
/* ============================================== TreeNode Functions ============================================== */
TreeNodeData * new_tree_node_data (int response_code, enum node_colour colour);

TreeNode * new_tree_node(TreeNodeData * tree_data);

TreeNodeData * get_tree_node_data(TreeNode * tree_node);

double tree_node_exploitation_score(TreeNode * tree_node);

double tree_node_exploration_score(TreeNode * tree_node);

double tree_node_score(TreeNode * tree_node);

gboolean is_fully_explored(TreeNode *  tree_node);

gboolean is_leaf(TreeNode * tree_node);

gboolean fits_fish_bone_optimisation(TreeNode * tree_node);

TreeNode * best_child(TreeNode * tree_node);

char * mutate(TreeNode * tree_node);

TreeNode * exists_child(TreeNode *  tree_node, int target_response_code);

TreeNode * append_child(TreeNode * tree_node, int child_response_code, enum node_colour colour);

void print_reversed_path(TreeNode * tree_node);
void print_path(TreeNode * tree_node);

int colour_encoder(enum node_colour colour);

void tree_node_print (TreeNode * tree_node);

/* ============================================== TreeNode Functions ============================================== */
/* ================================================ MCTS Functions ================================================ */
TreeNode * select_tree_node(TreeNode * parent_tree_node);
struct queue_entry * select_seed(TreeNode * tree_node_selected);
TreeNode * Initialisation();
struct queue_entry * Selection(TreeNode * parent_tree_node);
char * Simulation(TreeNode * target);
TreeNode * Expansion(TreeNode * tree_node, int * response_codes, int len_codes, gboolean * is_new);
void Propagation(TreeNode * selection_leaf, TreeNode * execution_leaf, gboolean is_new);

/* ================================================ MCTS Functions ================================================ */

/* Precomputation */
//static TreeNode * ROOT = new_tree_node(new_tree_node_data(0,White,""));
extern TreeNode * ROOT;

#endif //AFLNET_TREENODE_H

