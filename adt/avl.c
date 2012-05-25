/*
  Balanced Binary Search Tree Implementation
  
  Copyright (C) 2012 William Kennington
  
  This file is part of Libsync.
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "arr.h"

/*
  IMPLEMENTATION NOTES:
*/

typedef struct _avl_entry_t * avl_entry_t;

struct _avl_t
{
  avl_entry_t root;
  size_t size;
  int (*compare) (void *, void *);
  void (*free) (void *);
};

struct _avl_entry_t
{
  avl_entry_t left, right;
  char balance;
  void *data, *key;
};

avl_t
avl_init (int (*compare) (void *, void *))
{
  return avl_init_free (compare, free);
}

avl_t
avl_init_free (int (*compare) (void *, void *),
	       void (*free_func) (void *))
{
  avl_t avl;

  /* Allocate Memory for a new AVL Tree */
  avl = (avl_t) malloc (sizeof (struct _avl_t));

  /* Check for bad malloc */
  if (avl == NULL)
    return avl;

  /* Initialize the AVL */
  avl->root = NULL;
  avl->size = 0;
  avl->compare = compare;
  avl->free = free_func;

  return avl;
}

static void
avl_clear_intern (avl_t avl, int free_data)
{
  arr_t arr;
  avl_entry_t curr;
  size_t size;

  /* Create a new array to be used as a stack */
  arr = arr_init ();

  /* Iterate over every node */
  arr_insert_back (arr, avl->root);
  while ((size = arr_size (arr)) > 0)
    {
      /* Get the current element from the stack */
      curr = (avl_entry_t) arr_get (arr, size-1);
      arr_remove (arr, size-1);

      /* Check for invalid nodes */
      if (curr == NULL)
	continue;

      /* Push children onto the stack */
      arr_insert_back (arr, curr->left);
      arr_insert_back (arr, curr->right);

      /* Free parent */
      if (free_data && curr->data != NULL)
	free (curr->data);
      avl->free (curr->key);
      free (curr);
    }

  /* Cleanup the array */
  arr_destroy (arr);

  /* Update AVL Struct */
  avl->size = 0;
  avl->root = NULL;

  return;
}

void
avl_clear (avl_t avl)
{
  avl_clear_intern (avl, 0);
}

void
avl_clear_free (avl_t avl)
{
  avl_clear_intern (avl, 1);
}

static void
avl_destroy_intern (avl_t avl, int free_data)
{
  /* Removes internal nodes */
  avl_clear_intern (avl, free_data);

  /* Free Tree Struct */
  free (avl);
}

void
avl_destroy (avl_t avl)
{
  avl_destroy_intern (avl, 0);
}

void
avl_destroy_free (avl_t avl)
{
  avl_destroy_intern (avl, 1);
}

size_t
avl_size (avl_t avl)
{
  return avl->size;
}

static avl_entry_t
avl_get_intern (avl_t avl, void * key)
{
  avl_entry_t curr;
  int ret;

  /* Traverse the tree using the comparator */
  curr = avl->root;
  while (curr != NULL)
    {
      /* Compare the key */
      ret = avl->compare (key, curr->key);

      /* Pick the proper traversal direction */
      if (ret == 0)
	break;
      else if (ret < 0)
	curr = curr->left;
      else
	curr = curr->right;
    }

  return curr;
}

static avl_error_t
avl_insert_intern (avl_t avl, void * key, void * data, int replace)
{
  arr_t stack;
  size_t size;
  avl_entry_t *curr, tmp;
  int ret;

  /* Create a stack for saving the path */
  stack = arr_init ();
  if (stack == NULL)
    return AVL_MALLOC_FAILED;

  /* Iterate down the the insertion point and update the balance */
  curr = &avl->root;
  while (*curr != NULL)
    {
      /* Insert the node into the backlog */
      arr_insert_back (stack, curr);

      /* Determine the correct traveral direction */
      ret = avl->compare (key, (*curr)->key);
      if (ret == 0)
	{
	  arr_destroy (stack);
	  if (replace)
	    {
	      (*curr)->data = data;
	      return AVL_OK;
	    }
	  else
	    return AVL_KEY_EXISTS;
	}
      else if (ret < 0)
	{
	  (*curr)->balance--;
	  curr = &(*curr)->left;
	}
      else
	{
	  (*curr)->balance++;
	  curr = &(*curr)->right;
	}
    }

  /* Create the new entry */
  tmp = (avl_entry_t) malloc (sizeof (struct _avl_entry_t));
  if (tmp == NULL)
    return AVL_MALLOC_FAILED;
  tmp->left = NULL;
  tmp->right = NULL;
  tmp->balance = 0;
  tmp->key = key;
  tmp->data = data;

  /* Insert the entry */
  *curr = tmp;

  /* Rebalance the tree upward */
  while ((size = arr_size (stack)) > 0)
    {
      break;
    }

  /* Increase the Tree Size */
  avl->size++;

  /* Destroy the path stack */
  arr_destroy (stack);

  return AVL_OK;
}

avl_error_t
avl_insert (avl_t avl, void * key, void * data)
{
  return avl_insert_intern (avl, key, data, 0);
}

avl_error_t
avl_insert_rep (avl_t avl, void * key, void * data)
{
  return avl_insert_intern (avl, key, data, 1);
}

avl_error_t
avl_replace (avl_t avl, void * key, void * data)
{
  avl_entry_t entry;

  /* Get the entry containing key */
  entry = avl_get_intern (avl, key);
  if (entry == NULL)
    return AVL_KEY_INVALID;

  /* Replace the data with new data */
  entry->data = data;

  return AVL_OK;
}

void *
avl_get (avl_t avl, void * key)
{
  avl_entry_t entry;

  /* Get the entry containing the key */
  entry = avl_get_intern (avl, key);
  if (entry == NULL)
    return NULL;

  return entry->data;
}

avl_error_t
avl_remove (avl_t avl, void * key)
{
  avl_entry_t entry;

  /* Get the entry containing the key */
  entry = avl_get_intern (avl, key);
  if (entry == NULL)
    return AVL_KEY_INVALID;

  return AVL_UNKNOWN;
}
