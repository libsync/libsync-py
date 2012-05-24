/*
  A simple dynamic array implementation
  
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
#include "arr.h"

/*
  IMPLEMENTATION NOTES:
  internally stores an array of void pointers
  resizes by 2 times the previous size
  attempts to optimize operations on the tail end of the array
*/

#define ARR_INITIAL_SIZE 16

struct _arr_t
{
  void ** arr;
  size_t len, size;
};

arr_t
arr_init (void)
{
  arr_t arr;

  /* Allocate memory for the array structure */
  arr = (arr_t) malloc (sizeof (struct _arr_t));

  /* Check for failed malloc */
  if (arr == NULL)
    return arr;

  /* Initialize the array */
  arr->arr = (void **) malloc (ARR_INITIAL_SIZE * sizeof (void *));

  /* Check for failed malloc */
  if (arr->arr == NULL)
    {
      free (arr);
      return NULL;
    }

  /* Finish Initialization */
  arr->size = ARR_INITIAL_SIZE;
  arr->len = 0;

  return arr;
}

void
arr_clear (arr_t arr)
{
  arr->len = 0;
}

void
arr_clear_free (arr_t arr)
{
  size_t i;

  /* Iterate over each element freeing it */
  for (i = 0; i < arr->len; i++)
    if (arr->arr[i] != NULL)
      free (arr->arr[i]);

  /* Reset Length */
  arr->len = 0;
}

void
arr_destroy (arr_t arr)
{
  free (arr->arr);
  free (arr);
}

void
arr_destroy_free (arr_t arr)
{
  /* Free all of the elements */
  arr_clear_free (arr);

  /* Free the structures */
  arr_destroy (arr);
}

size_t
arr_size (arr_t arr)
{
  return arr->len;
}

arr_error_t
arr_reserve (arr_t arr, size_t added)
{
  void **tmp;

  /* Resize the array when space is low */
  if (arr->len + added > arr->size)
    {
      while (arr->len + added > arr->size)
	arr->size <<= 1;
      tmp = (void **) realloc (arr->arr, arr->size * sizeof (void *));
      
      /* Make sure malloc succeeds */
      if (tmp == NULL)
	return ARR_MALLOC_FAILED;

      arr->arr = tmp;
    }
  return ARR_OK;  
}

void *
arr_get (arr_t arr, size_t idx)
{
  /* Sanity Check */
  if (idx >= arr->len)
    return NULL;

  return arr->arr[idx];
}

arr_error_t
arr_set (arr_t arr, size_t idx, void * data)
{
  /* Sanity Check */
  if (idx >= arr->len)
    return ARR_INVALID_IDX;

  arr->arr[idx] = data;
  return ARR_OK;
}

arr_error_t
arr_insert_back (arr_t arr, void * data)
{
  /* Resize the array when space is low */
  if (arr_reserve (arr, 1) != ARR_OK)
    return ARR_MALLOC_FAILED;

  /* Insert the new element */
  arr->arr[arr->len] = data;
  arr->len++;

  return ARR_OK;
}

arr_error_t
arr_insert_before (arr_t arr, size_t idx, void * data)
{
  size_t i;

  /* Sanity Check */
  if (idx >= arr->len)
    return ARR_INVALID_IDX;

  /* Resize the array when space is low */
  if (arr_reserve (arr, 1) != ARR_OK)
    return ARR_MALLOC_FAILED;

  /* Move the old elements forward */
  for (i = arr->len; i > idx; i--)
    arr->arr[i] = arr->arr[i-1];

  /* Insert the new element */
  arr->arr[idx] = data;
  arr->len++;

  return ARR_OK;
}

arr_error_t
arr_remove (arr_t arr, size_t idx)
{
  size_t i;

  /* Sanity Check */
  if (idx >= arr->len)
    return ARR_INVALID_IDX;

  /* Shift elements into place */
  for (i = idx; i < arr->len; i++)
    arr->arr[i] = arr->arr[i+1];

  /* Fix array length */
  arr->len--;

  return ARR_UNKNOWN;
}
