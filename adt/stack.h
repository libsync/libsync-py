/*
  A simple, fast dynamic stack
  
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

#ifndef _ADT_STACK_H_
#define _ADT_STACK_H_

/* NOTE: All stack operations run in ~O(1) time except clear / destroy */

typedef struct _stack_t * stack_t;

typedef enum _stack_error_t
  {
    STACK_OK = 0,
    STACK_MALLOC_FAILED,
    STACK_UNKNOWN
  } stack_error_t;

/* Creates a new stack object or returns NULL on failure */
stack_t stack_init (void);

/* Looks at the top of the stack and optionally removes an element
   Returns NULL on failure or if the stack is empty */
void * stack_peek (stack_t stack);
void * stack_pop (stack_t stack);

/* Returns the number of elements on the stack */
size_t stack_size (stack_t stack);

/* Pushes data onto the stack, returning any errors that occur */
stack_error_t stack_push (stack_t stack, void * data);

/* Clears the stack without freeing inserted elements */
void stack_clear (stack_t stack);

/* Destroys the stack object but doesn't free the inserted elements */
void stack_destroy (stack_t stack);

#endif
