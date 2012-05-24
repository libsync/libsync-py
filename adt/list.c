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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "list.h"

/*
  IMPLEMENTATION NOTES:

  The list is implemented using head and tail sentinels.

  The sentinels are allocated in the same chunk as the list,
  DO NOT free the sentinels separately.
  
  list_t and list_entry_t are pointers to the respective structures.

  All functions should be optimized for constant time access.
*/

struct _list_t
{
  list_entry_t head, tail;
  size_t size;
};

struct _list_entry_t
{
  list_entry_t prev, next;
  void *data;
};

list_t
list_init (void)
{
  list_t list;

  /* Allocate Memory for the list */
  list = (list_t) malloc (sizeof (struct _list_t) + (sizeof (struct _list_entry_t) << 1));

  /* Check for failed malloc */
  if (list == NULL)
    return list;

  /* Initialize List Members */
  list->size = 0;

  /* Add head and tail sentinels */
  list->head = (list_entry_t)((int8_t *)list + sizeof (struct _list_t));
  list->tail = (list_entry_t)((int8_t *)list->head + sizeof (struct _list_entry_t));

  /* Initialize sentinels */
  list->head->next = list->tail;
  list->head->prev = NULL;

  list->tail->next = NULL;
  list->tail->prev = list->head;

  return list;
}

static void
list_clear_intern (list_t list, int free_data)
{
  list_entry_t tmp, old;

  /* Iterate over each node deleting it */
  tmp = list->head->next;
  while (tmp != list->tail)
    {
      old = tmp;
      tmp = tmp->next;
      if (free_data && old->data != NULL)
	free (old->data);
      free (old);
    }

  /* Fix sentinel pointers */
  list->head->next = list->tail;
  list->tail->prev = list->head;

  /* Fix size */
  list->size = 0;
}

void
list_clear (list_t list)
{
  list_clear_intern (list, 0);
}

void
list_clear_free (list_t list)
{
  list_clear_intern (list, 1);
}

static void
list_destroy_intern (list_t list, int free_data)
{
  /* Delete all of the nodes in the list */
  list_clear_intern (list, free_data);

  /* Delete the list */
  free (list);
}

void
list_destroy (list_t list)
{
  list_destroy_intern (list, 0);
}

void
list_destroy_free (list_t list)
{
  list_destroy_intern (list, 1);
}

list_error_t
list_insert_after (list_t list, list_entry_t entry, void * data)
{
  list_entry_t new_entry;

  /* Initialize New Entry */
  new_entry = (list_entry_t) malloc (sizeof (struct _list_entry_t));

  /* Check for bad malloc */
  if (new_entry == NULL)
    return LIST_MALLOC_FAILED;

  new_entry->data = data;

  /* Insert the entry */
  new_entry->next = entry->next;
  new_entry->prev = entry;
  entry->next = new_entry;
  new_entry->next->prev = new_entry;

  /* Fix list size */
  list->size++;

  return LIST_OK;
}

list_error_t
list_insert_before (list_t list, list_entry_t entry, void * data)
{
  list_entry_t new_entry;

  /* Initialize New Entry */
  new_entry = (list_entry_t) malloc (sizeof (struct _list_entry_t));

  /* Check for bad malloc */
  if (new_entry == NULL)
    return LIST_MALLOC_FAILED;

  new_entry->data = data;

  /* Insert the entry */
  new_entry->next = entry;
  new_entry->prev = entry->prev;
  entry->prev = new_entry;
  new_entry->prev->next = new_entry;

  /* Fix list size */
  list->size++;

  return LIST_OK;
}

list_error_t
list_insert_back (list_t list, void * data)
{
  return list_insert_before (list, list->tail, data);
}

list_error_t
list_insert_front (list_t list, void * data)
{
  return list_insert_after (list, list->head, data);
}

size_t
list_size (list_t list)
{
  return list->size;
}

list_entry_t
list_get_front (list_t list)
{
  /* Sanity Check */
  if (list->size == 0)
    return NULL;

  return list->head->next;
}

list_entry_t
list_get_back (list_t list)
{
  /* Sanity Check */
  if (list->size == 0)
    return NULL;

  return list->tail->prev;
}

list_entry_t
list_next (list_entry_t entry)
{
  /* Don't return the sentinel */
  if (entry->next->next == NULL)
    return NULL;

  return entry->next;
}

list_entry_t
list_prev (list_entry_t entry)
{
  /* Don't return the sentinel */
  if (entry->prev->prev == NULL)
    return NULL;
  
  return entry->prev;
}

void *
list_get_data (list_entry_t entry)
{
  return entry->data;
}

void
list_set_data (list_entry_t entry, void * data)
{
  entry->data = data;
}

static void
list_remove_intern (list_t list, list_entry_t entry, int free_data)
{
  /* Fix list pointers */
  entry->next->prev = entry->prev;
  entry->prev->next = entry->next;

  /* Delete the entry */
  if (free_data)
    free (entry->data);
  free (entry);

  /* Fix the list size */
  list->size--;
}

void
list_remove (list_t list, list_entry_t entry)
{
  list_remove_intern (list, entry, 0);
}

void
list_remove_free (list_t list, list_entry_t entry)
{
  list_remove_intern (list, entry, 1);
}
