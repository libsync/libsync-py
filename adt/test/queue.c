/*
  Tests for the dynamic queue Implementation
  
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
#include <stdio.h>
#include <string.h>
#include "queue.h"
#include "stack.h"

void
balance (stack_t stack_in, stack_t stack_out)
{
  if (stack_size (stack_out) == 0)
    while (stack_size (stack_in) > 0)
      stack_push (stack_out, stack_pop (stack_in));
}

void *
peek (stack_t stack_in, stack_t stack_out)
{
  /* Balance the outstack */
  balance (stack_in, stack_out);

  return stack_peek (stack_out);
}

void *
deq (stack_t stack_in, stack_t stack_out)
{
  /* Balance the outstack */
  balance (stack_in, stack_out);

  return stack_pop (stack_out);
}

void
test_empty (queue_t queue)
{
  /* Test size empty*/
  if (queue_size (queue) != 0)
    {
      printf ("Empty Queue: Size Failed\n");
      exit (EXIT_FAILURE);
    }

  /* Test peek and deq */
  if (queue_peek (queue) != NULL ||
      queue_deq (queue) != NULL)
    {
      printf ("Empty Queue: Peek / Deq Failed\n");
      exit (EXIT_FAILURE);
    }
}

int
main (int argc, char ** argv)
{
  queue_t queue;
  size_t i, j, size;
  stack_t stack_in, stack_out;

  /* Create a new queue */
  queue = queue_init ();
  if (queue == NULL)
    {
      printf ("Failed to initialize the initial queue\n");
      return 99;
    }

  /* Test empty queue */
  test_empty (queue);

  /* Test a few inserts */
  size = 10;
  for (i = 0; i < size; i++)
    if (queue_enq (queue, (void*)i) == QUEUE_MALLOC_FAILED)
      {
	printf ("Insert: Malloc failed\n");
	return 99;
      }
  if (size != queue_size (queue))
    {
      printf ("Few Inserts: Size Failed\n");
      return EXIT_FAILURE;
    }
  for (i = 0; i < size; i++)
    if (queue_peek (queue) != (void*)i ||
	queue_deq (queue) != (void*)i)
      {
	printf ("Few Inserts: Integrity Failed\n");
	return EXIT_FAILURE;
      }

  /* Test Clear */
  queue_clear (queue);
  test_empty (queue);

  /* Test Many Inserts */
  size = 10000;
  stack_in = stack_init ();
  stack_out = stack_init ();
  if (stack_in == NULL || stack_out == NULL)
    {
      printf ("Failed to create stacks\n");
      return 99;
    }
  for (i = 0; i < size; i++)
    {
      /* Queue the elements */
      stack_push (stack_in, (void*)i);
      queue_enq (queue, (void*)i);

      /* Queue and dequeue many elements */
      for (j = 0; j < 100; j++)
	{
	  stack_push (stack_in, (void*)i);
	  queue_enq (queue, (void*)i);

	  /* Check the queue */
	  if (queue_peek (queue) != peek (stack_in, stack_out) ||
	      queue_deq (queue) != deq (stack_in, stack_out))
	    {
	      printf ("Many Inserts: Integrity Failed\n");
	      return EXIT_FAILURE;
	    }
	}
    }
  if (size != queue_size (queue))
    {
      printf ("Many Inserts: Size Failure\n");
      return EXIT_FAILURE;
    }
  for (i = 0; i < size; i++)
    if (queue_peek (queue) != peek (stack_in, stack_out) ||
	queue_deq (queue) != deq (stack_in, stack_out))
      {
	printf ("Many Inserts: Integrity Failed\n");
	return EXIT_FAILURE;
      }
  stack_destroy (stack_in);
  stack_destroy (stack_out);

  /* Destroy the queue */
  queue_destroy (queue);

  return EXIT_SUCCESS;
}
