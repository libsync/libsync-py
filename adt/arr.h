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

#ifndef _ADT_ARR_H_
#define _ADT_ARR_H_

typedef struct _arr_t * arr_t;

typedef enum _arr_error_t
  {
    ARR_OK = 0,
    ARR_INVALID_IDX,
    ARR_MALLOC_FAILED,
    ARR_UNKNOWN
  } arr_error_t;

/* Creates a new array or NULL on failure */
arr_t arr_init (void);

/* Removes all of the elements in the array
   Note the running time of O(1) */
void arr_clear (arr_t arr);

/* Removes all of the elements in the array and frees them
   Note the running time of O(n) */
void arr_clear_free (arr_t arr);

/* Completely destroys the array
   Note the running time of O(1) */
void arr_destroy (arr_t arr);

/* Destroys the array and frees all of the elements 
   Note the running time of O(n) */
void arr_destroy_free (arr_t arr);

/* Returns the size of the array
   Note the running time of O(1) */
size_t arr_size (arr_t arr);

/* Allocates room for additional size inserts
   Note the running time of O(n) in cases where resizing is necessary */
arr_error_t arr_reserve (arr_t arr, size_t added);

/* Modifies an element at the given location
   Note the running time of O(1) */
void * arr_get (arr_t arr, size_t idx);
arr_error_t arr_set (arr_t arr, size_t idx, void * data);

/* Inserts an element at the end of the array
   Note the running time of ~O(1) due to resizing */
arr_error_t arr_insert_back (arr_t arr, void * data);

/* Inserts an element right before the specified index
   Note the running time increases to O(n) at index 0 */
arr_error_t arr_insert_before (arr_t arr, size_t idx, void * data);

/* Removes an element from the specified index
   Note the running time increases to O(n) at index 0 */
arr_error_t arr_remove (arr_t arr, size_t idx);

#endif
