/*
  A simple doubly linked list implementation
  
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

/*
 * Note that all of these functions run in constant time
 * with the exception of the clear / destroy functions.
 */

#ifndef _ADT_LIST_H_
#define _ADT_LIST_H_

typedef struct _list_t * list_t;
typedef struct _list_entry_t * list_entry_t;

typedef enum _list_error_t
  {
    LIST_OK = 0,
    LIST_MALLOC_FAILED,
    LIST_UNKNOWN
  } list_error_t;

/* Returns a new list structure */
list_t list_init (void);

/* Deletes all of the nodes in a list */
void list_clear (list_t list);

/* Frees data while deleteing all nodes in the list */
void list_clear_free (list_t list);

/* Destroys a list */
void list_destroy (list_t list);

/* Destroys and frees all data in a list */
void list_destroy_free (list_t list);

/* Inserts a node with data */
list_error_t list_insert_after (list_t list, list_entry_t entry, void * data);
list_error_t list_insert_before (list_t list, list_entry_t entry, void * data);
list_error_t list_insert_back (list_t list, void * data);
list_error_t list_insert_front (list_t list, void * data);

/* Retrieves the size of the list */
size_t list_size (list_t list);

/* Gets the node pointer as an iterator or NULL if none exists */
list_entry_t list_get_front (list_t list);
list_entry_t list_get_back (list_t list);

/* Iterate the node pointer or returns NULL if at the end of the list */
list_entry_t list_next (list_entry_t entry);
list_entry_t list_prev (list_entry_t entry);

/* Modify Node Data */
void * list_get_data (list_entry_t entry);
void list_set_data (list_entry_t entry, void * data);

/* Remove a node and invalidate the entry */
void list_remove (list_t list, list_entry_t entry);

/* Remove the node, free data, and invalidate the entry */
void list_remove_free (list_t list, list_entry_t entry);

#endif
