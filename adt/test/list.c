/*
  Test Cases for the Doubly Linked List Implementation
  
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
#include "../list.h"

void
test_empty (list_t list)
{
  size_t size;
  list_entry_t entry;

  entry = list_get_front (list);
  if (entry != NULL)
    {
      printf ("EMPTY TEST: Front Entry Wasn't NULL\n");
      exit (EXIT_FAILURE);
    }
  entry = list_get_back (list);
  if (entry != NULL)
    {
      printf ("EMPTY TEST: Back Entry Wasn't NULL\n");
      exit (EXIT_FAILURE);
    }
  size = list_size (list);  
  if (size != 0)
    {
      printf ("EMPTY TEST: List Size Wasn't 0!\n");
      exit (EXIT_FAILURE);
    }
}

int
main (int argc, char ** argv)
{
  list_t list;
  list_entry_t entry, entry2;
  size_t size, i;

  /* Initialize the list */
  list = list_init ();
  if (list == NULL)
    return 99;

  /* Try and access an empty list */
  test_empty (list);

  /* Two Element Test */
  list_insert_front (list, (void *)1);
  list_insert_back (list, (void *)3);

  /* Add two elements for 4 total */
  entry = list_get_front (list);
  entry = list_next (entry);
  list_insert_before (list, entry, (void *)2);
  entry = list_prev (entry);
  entry = list_next (entry);
  list_insert_after (list, entry, (void *)4);
  size = list_size (list);
  if (size != 4)
    {
      printf ("4 ELEMS: Invalid Size\n");
      return EXIT_FAILURE;
    }
  entry = list_get_front (list);
  entry2 = list_get_back (list);
  for(i = 0; i < size; i++)
    {
      if (list_get_data(entry) != (void *)(i + 1) ||
	  list_get_data(entry2) != (void *)(size - i))
	{
	  printf ("4 ELEMS: Invalid Element\n");
	  return EXIT_FAILURE;
	}
      entry = list_next (entry);
      entry2 = list_prev (entry2);
    }
  if (entry2 != NULL || entry != NULL)
    {
      printf ("4 ELEMS: Didn't hit the end of the array\n");
      return EXIT_FAILURE;
    }

  /* Test modifying data */
  entry = list_get_front (list);
  list_set_data (entry, (void *)5);
  if (list_get_data (entry) != (void *)5)
    {
      printf ("Failed to Modify Elem\n");
      return EXIT_FAILURE;
    }

  /* Test clearing the list */
  list_clear (list);
  list_clear (list);
  test_empty (list);

  /* Test freeing */
  list_insert_front (list, malloc (16));
  list_insert_front (list, NULL);
  list_insert_front (list, malloc (16));
  if (list_size (list) != 3)
    {
      printf ("Inserting 3 Elems fails test");
      return EXIT_FAILURE;
    }

  list_clear_free (list);
  if (list_size (list) != 0)
    {
      printf ("Clear Free Failed Size Test\n");
      return EXIT_FAILURE;
    }

  list_insert_front (list, malloc (16));
  list_insert_front (list, malloc (16));

  /* Destroy the list */
  list_destroy_free (list);

  return EXIT_SUCCESS;
}
