/*
  Test Cases for the Dynamic Array Implementation
  
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
#include "../arr.h"

int
main (int argc, char ** argv)
{
  arr_t arr;
  size_t i, size;

  /* Initialize the array */
  arr = arr_init ();
  if (arr == NULL)
    return 99;

  /* Test and empty array */
  size = arr_size (arr);
  if (size != 0)
    {
      printf ("Initial Array is invalid\n");
      return EXIT_FAILURE;
    }
  if (arr_get (arr, 0))
    {
      printf ("Initial Array is invalid\n");
      return EXIT_FAILURE;
    }

  /* Test small inserts */
  size = 10;
  for (i = 0; i < size; i++)
    if (arr_insert_back(arr, (void *)i) == ARR_MALLOC_FAILED)
      return 99;
  if (arr_size(arr) != size)
    {
      printf ("Small Inserts: Failed Size Check\n");
      return EXIT_FAILURE;
    }
  for (i = 0; i < size; i++)
    if (arr_get (arr, i) != (void *)i)
      {
	printf ("Small Inserts: Failed Integrity Check\n");
	return EXIT_FAILURE;
      }

  /* Test Clear */
  arr_clear (arr);
  if (arr_size (arr) != 0)
    {
      printf ("Clear Test Failed\n");
      return EXIT_FAILURE;
    }

  /* Test Insert Before */
  if (arr_insert_before (arr, 0, (void *)0) != ARR_INVALID_IDX)
    {
      printf ("Insert Before: Failed on Invalid Index\n");
      return EXIT_FAILURE;
    }
  if (arr_insert_back (arr, (void *)6) == ARR_MALLOC_FAILED ||
      arr_insert_back (arr, (void *)3) == ARR_MALLOC_FAILED ||
      arr_insert_before (arr, 0, (void *)0) == ARR_MALLOC_FAILED ||
      arr_insert_before (arr, 1, (void *)1) == ARR_MALLOC_FAILED ||
      arr_insert_back (arr, (void *)4) == ARR_MALLOC_FAILED)
    return 99;
  arr_set (arr, 2, (void *)2);
  if (arr_size(arr) != 5)
    {
      printf ("Random Modifications: Failed Size Test\n");
      return EXIT_FAILURE;
    }
  for (i = 0; i < 5; i++)
    if (arr_get (arr, i) != (void *)i)
      {
	printf ("Random Modifications: Failed Integrity Test\n");
	return EXIT_FAILURE;
      }
  arr_clear (arr);

  /* Test Many Inserts */
  size = 100000;
  for (i = 0; i < size; i++)
    arr_insert_back (arr, (void *)i);
  if (arr_size (arr) != size)
    {
      printf ("Many Inserts: Failed Size Check\n");
      return EXIT_FAILURE;
    }
  for (i = 0; i < size; i++)
    if (arr_get (arr, i) != (void *)i)
      {
	printf ("Many Inserts: Failed Integrity Check\n");
	return EXIT_FAILURE;
      }
  arr_clear (arr);

  /* Test clear free */
  arr_insert_back (arr, malloc (16));
  arr_insert_back (arr, malloc (16));
  arr_insert_back (arr, NULL);
  arr_insert_back (arr, malloc (16));
  arr_clear_free (arr);

  if (arr_size (arr) != 0)
    {
      printf ("Clear Free Test Failed\n");
      return EXIT_FAILURE;
    }

  /* Destroy the array */
  arr_destroy (arr);

  return EXIT_SUCCESS;
}
