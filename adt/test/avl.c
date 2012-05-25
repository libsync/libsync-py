/*
  Tests for Balanced Binary Search Tree Implementation
  
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
#include "../avl.h"

int
compare (void * a, void * b)
{
  return ((int)a) - ((int)b);
}

void
free_int (void * ptr)
{
}

void
free_complex (void * ptr)
{
  free (*(void**)ptr);
  free (ptr);
}

int
main (int argc, char ** argv)
{
  avl_t avl;
  size_t size, i;
  void ** tmp;

  /* Default Constructor */
  avl = avl_init_free (compare, free_int);
  if (avl == NULL)
    return 99;

  /* Test Empty Size */
  if (avl_size (avl) != 0)
    {
      printf ("Invalid Initial Size\n");
      return EXIT_FAILURE;
    }

  /* Test get, remove, and replace on empty */
  if (avl_replace (avl, (void *)1, (void *)1) != AVL_KEY_INVALID)
    {
      printf ("Empty AVL Replace Invalid Key\n");
      return EXIT_FAILURE;
    }
  if (avl_get (avl, (void *)1) != NULL)
    {
      printf ("Empty AVL Get Invalid Key\n");
      return EXIT_FAILURE;
    }
  if (avl_remove (avl, (void *)1) != AVL_KEY_INVALID)
    {
      printf ("Empty AVL Remove Invalid Key\n");
      return EXIT_FAILURE;
    }

  /* Test 3 inserts */
  if (avl_insert (avl, (void *)0, (void *)0) == AVL_MALLOC_FAILED ||
      avl_insert (avl, (void *)1, (void *)3) == AVL_MALLOC_FAILED ||
      avl_insert (avl, (void *)2, (void *)2) == AVL_MALLOC_FAILED)
    return 99;
  if (avl_size (avl) != 3)
    {
      printf ("Three Inserts Size Failed\n");
      return EXIT_FAILURE;
    }
  if (avl_get (avl, (void *)0) != (void *)0 ||
      avl_get (avl, (void *)2) != (void *)2 ||
      avl_get (avl, (void *)1) != (void *)3)
    {
      printf ("Three Inserts Failed\n");
      return EXIT_FAILURE;
    }

  /* Test replace and insert_replace */
  if (avl_insert_rep (avl, (void *)3, (void *)10) != AVL_OK ||
      avl_replace (avl, (void *)3, (void *)3) != AVL_OK ||
      avl_insert_rep (avl, (void *)1, (void *)1) != AVL_OK)
    {
      printf ("Replaces and inserts failed\n");
      return EXIT_FAILURE;
    }
  if (avl_size (avl) != 4)
    {
      printf ("Replace and Inserts Size Failed\n");
      return EXIT_FAILURE;
    }
  if (avl_get (avl, (void *)0) != (void *)0 ||
      avl_get (avl, (void *)1) != (void *)1 ||
      avl_get (avl, (void *)2) != (void *)2 ||
      avl_get (avl, (void *)3) != (void *)3)
    {
      printf ("Replace and Inserts Failed\n");
      return EXIT_FAILURE;
    }

  /* Test Failed Insert and Failed Replace */
  if (avl_insert (avl, (void *)1, (void *)4) != AVL_KEY_EXISTS ||
      avl_replace (avl, (void *)10, (void *)1) != AVL_KEY_INVALID)
    {
      printf ("Passed when insert / replace it should fail\n");
      return EXIT_FAILURE;
    }
  if (avl_size (avl) != 4)
    {
      printf ("Invalid insert / replace Size Failed\n");
      return EXIT_FAILURE;
    }
  if (avl_get (avl, (void *)0) != (void *)0 ||
      avl_get (avl, (void *)1) != (void *)1 ||
      avl_get (avl, (void *)2) != (void *)2 ||
      avl_get (avl, (void *)3) != (void *)3)
    {
      printf ("Invalid insert / replace Failed\n");
      return EXIT_FAILURE;
    }

  /* Test Clear */
  avl_clear (avl);
  if (avl_size (avl) != 0 ||
      avl_get (avl, (void *)1) != NULL)
    {
      printf ("Clearing the AVL Failed\n");
      return EXIT_FAILURE;
    }

  /* Test Many Inserts */
  size = 1000;
  for (i = 0; i < size; i++)
    avl_insert (avl, (void *)i, (void *)i);
  if (avl_size (avl) != size)
    {
      printf ("Many Inserts Size Failed\n");
      return EXIT_FAILURE;;
    }
  for (i = 0; i < size; i++)
    if (avl_get (avl, (void *)i) != (void *)i)
      {
	printf ("Many Inserts Failed\n");
	return EXIT_FAILURE;
      }

  /* Default Destructor */
  avl_destroy (avl);

  /* Test more complex free routine */
  avl = avl_init_free (compare, free_complex);
  tmp = (void **) malloc (sizeof (void *));
  *tmp = malloc (16);
  avl_insert (avl, tmp, NULL);
  avl_destroy (avl);

  return EXIT_SUCCESS;;
}
