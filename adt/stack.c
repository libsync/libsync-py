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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stack.h"

/*
 IMPLEMENTATION NOTES:

 Uses dynamically allocated arrays for storage and expands in exponentially growing links on a linked list
*/

#define START_SIZE 32

typedef struct _stack_node_t * stack_node_t;

struct _stack_t
{
  stack_node_t head;
  stack_node_t unused;
  size_t size;
};

struct _stack_node_t
{
  void ** data;
  size_t size, len;
  stack_node_t next;
};

stack_t
stack_init (void)
{
  stack_t stack;

  /* Allocate memory for the stack, the first node, and the first node's array */
  stack = (stack_t) malloc (sizeof (struct _stack_t) +
			    sizeof (struct _stack_node_t) +
			    sizeof (void *) * START_SIZE);

  /* Check for a failed alloc */
  if (stack == NULL)
    return stack;

  /* Initialize the stack */
  stack->size = 0;
  stack->head = (stack_node_t)((char*)stack + sizeof (struct _stack_t));
  stack->unused = NULL;

  /* Initialize the first node */
  stack->head->len = 0;
  stack->head->size = START_SIZE;
  stack->head->next = NULL;
  stack->head->data = (void**)((char*)stack->head + sizeof (struct _stack_node_t));

  return stack;
}

void *
stack_peek (stack_t stack)
{
  /* Empty Stack Check */
  if (stack->size == 0)
    return NULL;

  /* Check for the end of the array and move the link into unused */
  if (stack->head->len == 0)
    {
      /* If there is already an unused node delete it */
      if (stack->unused != NULL)
	free (stack->unused);
      stack->unused = stack->head;
      stack->head = stack->head->next;
    }

  return stack->head->data[stack->head->len-1];
}

void *
stack_pop (stack_t stack)
{
  /* Empty Stack Check */
  if (stack->size == 0)
    return NULL;

  /* Check for the end of the array and move the link into unused */
  if (stack->head->len == 0)
    {
      /* If there is already an unused node delete it */
      if (stack->unused != NULL)
	free (stack->unused);
      stack->unused = stack->head;
      stack->head = stack->head->next;
    }

  /* Decrease the size of the stack */
  stack->size--;

  return stack->head->data[--stack->head->len];
}

size_t
stack_size (stack_t stack)
{
  return stack->size;
}

stack_error_t
stack_push (stack_t stack, void * data)
{
  /* Handle list overflow */
  if (stack->head->len == stack->head->size)
    {
      /* Add a new node if an empty one doesn't exist */
      if (stack->unused == NULL)
	{
	  /* Allocate a new node */
	  stack->unused = (stack_node_t) malloc (sizeof (struct _stack_node_t) +
						 sizeof (void *) * (stack->head->size << 1));

	  /* Check for malloc errors */
	  if (stack->unused == NULL)
	    return STACK_MALLOC_FAILED;
	
	  /* Initialize the unused node */
	  stack->unused->next = stack->head;
	  stack->unused->size = stack->head->size << 1;
	  stack->unused->len = 0;
	  stack->unused->data = (void**)((char*)stack->unused + sizeof (struct _stack_node_t));
	}
      
      stack->head = stack->unused;
      stack->unused = NULL;
    }

  /* Add Element to stack */
  stack->head->data[stack->head->len++] = data;

  /* Increase Stack Size */
  stack->size++;

  return STACK_OK;
}

void
stack_clear (stack_t stack)
{
  stack_node_t tmp;

  /* Iterate over each node removing it */
  while (stack->head->next != NULL)
    {
      tmp = stack->head;
      stack->head = stack->head->next;
      free (tmp);
    }

  /* Free Any unused nodes */
  if (stack->unused != NULL)
    {
      free (stack->unused);
      stack->unused = NULL;
    }

  /* Set the length of the last node to 0 */
  stack->head->len = 0;

  /* Reset the size of the stack */
  stack->size = 0;
}

void
stack_destroy (stack_t stack)
{
  /* Remove all of the nodes from the stack */
  stack_clear (stack);

  /* Free the stack */
  free (stack);
}
