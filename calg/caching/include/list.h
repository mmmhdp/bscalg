#pragma once

#include <stddef.h>

typedef struct list LIST;
typedef struct node LIST_NODE;
typedef struct node_data NODE_DATA;

LIST *list_init (void);

void list_free (LIST *l);

void list_free_by_caller (LIST *l, void (*free_val) (NODE_DATA *d));

void list_add_node (LIST *l, void *v, int vsz);

void list_add_node_by_caller (LIST *l, void *v, int vsz,
                              void (*val_copy) (void **dst, void *v,
                                                size_t v_sz));

/*
 * Returns
 * 0 on success;
 * 1 if node doesn't belong to list
 * */
int list_delete_node (LIST *l, LIST_NODE *n);

int list_delete_node_by_caller (LIST *l, LIST_NODE *n,
                                void (*free_val_f) (NODE_DATA *d));

void list_move_node_to_tail (LIST *l, LIST_NODE *n);

LIST_NODE *list_node_find (LIST *l, LIST_NODE *n);

LIST_NODE *list_node_get_prev (LIST_NODE *n);

LIST_NODE *list_node_get_next (LIST_NODE *node);

int list_get_len (LIST *l);

NODE_DATA *list_node_data_get_data (LIST_NODE *n);

void *list_node_data_get_value (NODE_DATA *d);

size_t list_node_data_get_value_sz (NODE_DATA *d);

LIST_NODE *list_get_top_node (LIST *l);

LIST_NODE *list_get_tail_node (LIST *l);

void **list_convert_to_arr (LIST *l);

void list_print (LIST *l,
                 void (*node_printer) (NODE_DATA *d, int is_top_node));
