/*
  A simple, fast dynamic stack implementation test suite
  
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
#include "stack.h"

void
test_empty (stack_t stack)
{
  /* Test Size */
  if (stack_size (stack) != 0)
    {
      printf ("Empty Stack: Size Failed\n");
      exit (EXIT_FAILURE);
    }

  /* Test peek and pop functions */
  if (stack_pop (stack) != NULL)
    {
      printf ("Empty Stack: Pop Failure\n");
      exit (EXIT_FAILURE);
    }
  if (stack_peek (stack) != NULL)
    {
      printf ("Empty Stack: Peek Failure\n");
      exit (EXIT_FAILURE);
    }
}

int
main (int argc, char ** argv)
{
  stack_t stack;
  size_t i, size, j;
  void * tmp;

  /* Create a new stack */
  stack = stack_init ();
  if (stack == NULL)
    {
      printf ("Initial Memory allocation failed\n");
      return 99;
    }

  /* Test an empty stack */
  test_empty (stack);

  /* Small Number Test */
  size = 10;
  for (i = 0; i < size; i++)
    stack_push (stack, (void*)i);
  if (size != stack_size (stack))
    {
      printf ("Small Number: Size Failure\n");
      return EXIT_FAILURE;
    }
  for (i = size; i > 0; i--)
    if (stack_peek (stack) != (void*)(i-1) ||
	stack_pop (stack) != (void*)(i-1))
      {
	printf ("Small Number: Integrity Failure\n");
	return EXIT_FAILURE;
      }

  /* Make sure stack is empty */
  test_empty (stack);

  /* Clear Test */
  size = 100;
  for (i = 0; i < size; i++)
    stack_push (stack, (void*)i);
  stack_clear (stack);
  test_empty (stack);

  /* Many data test */
  size = 10000;
  for (i = 0; i < size; i++)
    {
      stack_push (stack, (void*)i);
      for (j = 0; j < 32; j++)
	stack_push (stack, (void*)(i+j));
      for (j = 32; j > 0; j--)
	if ((tmp = stack_peek (stack)) != (void*)(j+i-1) ||
	    (tmp = stack_pop (stack)) != (void*)(j+i-1))
	  {
	    printf ("Many Data: Integrity Failure\nExpected: %p Got: %p\n", j+i-1, tmp);
	    return EXIT_FAILURE;
	  }
    }
  if (size != stack_size (stack))
    {
      printf ("Many Data: Size Failure\nExpected: %d Got: %d\n", size, stack_size (stack));
      return EXIT_FAILURE;
    }
  for (i = size; i > 0; i--)
    if (stack_peek (stack) != (void*)(i-1) ||
	stack_pop (stack) != (void*)(i-1))
      {
	printf ("Many Data: Integrity Failure\n");
	return EXIT_FAILURE;
      }
    

  /* Destroy the stack */
  stack_destroy (stack);

  return EXIT_SUCCESS;
}
