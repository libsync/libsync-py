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

#ifndef _ADT_AVL_H_
#define _ADT_AVL_H_

typedef struct _avl_t * avl_t;

typedef enum _avl_error_t
  {
    AVL_OK = 0,
    AVL_MALLOC_FAILED,
    AVL_KEY_EXISTS,
    AVL_KEY_INVALID,
    AVL_UNKNOWN
  } avl_error_t;

/* Initalizes a new AVL Tree using a default key freeing function
   compare is a pointer to a comparison function for internal sorting based on keys */
avl_t avl_init (int (*compare) (void *, void *));

/* Initializes a new AVL Tree with a key freeing function
   compare is the same as the previous
   free_func takes a key and frees all associated memory */
avl_t avl_init_free (int (*compare) (void *, void *),
		     void (*free_func) (void *));

/* Clears the AVL Tree optionally freeing all of the elements
   Note a running time of O(n) */
void avl_clear (avl_t avl);
void avl_clear_free (avl_t avl);

/* Destroys the AVL Tree optionally freeing all of the elements
   Note a running time of O(n) */
void avl_destroy (avl_t avl);
void avl_destroy_free (avl_t avl);

/* Retrieves the size of the AVL */
size_t avl_size (avl_t avl);

/* Inserts a key value pair into the AVL Tree
   If the key already exists it returns an AVL_KEY_EXISTS */
avl_error_t avl_insert (avl_t avl, void * key, void * data);

/* Inserts a key value pair but overwrites if the key already exists */
avl_error_t avl_insert_rep (avl_t avl, void * key, void * data);

/* Replaces the key value pair with a new data value
   Returns AVL_KEY_INVALID if the key does not exist */
avl_error_t avl_replace (avl_t avl, void * key, void * data);

/* Retrieves the data for the given key from the AVL and return NULL on error */
void * avl_get (avl_t avl, void * key);

/* Removes the given key value pair from the AVL
   Returns AVL_KEY_INVALID if the key does not exist */
avl_error_t avl_remove (avl_t avl, void * key);

#endif
