#pragma once

typedef struct llst LLST;

LLST *
llst_init (void);

void
llst_free (LLST *top);

void
llst_add_node (LLST *head, int value);

/*
 * Delete node from linked list.
 * If top of the list is passed, 
 * llst_free() called instead.
*/
LLST *
llst_delete_node (LLST *node);

/*
 * Extract node and replace it
 * as a tail node of linked list.
*/
void
llst_move_node_to_tail (LLST *node);

void
llst_swap (LLST *src, LLST *dst);

LLST *
llst_get_prev (LLST *node);

LLST *
llst_get_next (LLST *node);

int
llst_get_val (LLST *node);

LLST *
llst_set_val (LLST *node, int val);

LLST *
llst_get_top (LLST *node);

LLST *
llst_get_tail (LLST *node);

/*
 * Convert linked list to the array.
 * Array is allocated on the heap and 
 * have to be feed by caller.
 * */
int *
llst_convert_to_arr (LLST *top);

void
llst_print (LLST *top);

void
llst_print_node(LLST *node);

int
llst_get_len (LLST *top);

