#pragma once

typedef struct list LIST;
typedef struct node LIST_NODE;
typedef struct node_data NODE_DATA;

LIST *
list_init (void);

void
list_free (LIST *l);


void
list_add_node (LIST *l, void *v, int vsz);

/*
 * Returns
 * 0 on success;
 * 1 if node doesn't belong to list
 * */
int
list_delete_node (LIST *l, LIST_NODE *n);

void
list_move_node_to_tail (LIST *l, LIST_NODE *n);

LIST_NODE *
list_node_find(LIST *l, LIST_NODE *n);


LIST_NODE *
llst_node_get_prev (LIST_NODE *n);

LIST_NODE *
list_node_get_next (LIST_NODE *node);

void *
list_get_val (LIST *node);

int
list_get_len (LIST *l);

NODE_DATA *
list_node_data_get_data (LIST_NODE *n);

LIST_NODE *
list_get_top_node (LIST *l);

LIST_NODE *
list_get_tail_node (LIST *l);


void **
list_convert_to_arr (LIST *l);

void
list_print (LIST *l, void (*node_printer) (NODE_DATA *d));
