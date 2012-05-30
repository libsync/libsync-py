/*
  A simple, fast dynamic queue
  
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
#include "queue.h"

/*
  IMPLEMENTATION NOTES:

  Uses arrays within linked lists to store the queue data.
  As soon as the array is filled another node is created.
  As soon as a link is dequeued it is freed from the list.
  Uses a doubly linked list to achieve this.
*/

#define BLOCK_SIZE 16

typedef struct _queue_node_t * queue_node_t;

struct _queue_t
{
  queue_node_t head, tail, unused;
  size_t size;
};

struct _queue_node_t
{
  void ** data;
  size_t deq_pos, len, size;
  queue_node_t next, prev;
};

queue_t
queue_init (void)
{
  queue_t queue;

  /* Allocate memory for the queue */
  queue = (queue_t) malloc (sizeof (struct _queue_t));

  /* Check for bad malloc */
  if (queue == NULL)
    return queue;

  /* Initializes the queue */
  queue->head = NULL;
  queue->tail = NULL;
  queue->unused = NULL;
  queue->size = 0;

  return queue;
}

static void
queue_eob (queue_t queue)
{
  if (queue->head->deq_pos == queue->head->size)
    {
      /* Free the already existing unused block */
      if (queue->unused != NULL)
	free (queue->unused);
      queue->unused = queue->head;
      queue->head = queue->head->next;
      queue->head->prev = NULL;
    }
}

void *
queue_peek (queue_t queue)
{
  /* Check for empty queue */
  if (queue->size == 0)
    return NULL;

  /* Check for the end of the block */
  queue_eob (queue);

  return queue->head->data[queue->head->deq_pos];
}

void *
queue_deq (queue_t queue)
{
  /* Check for empty queue */
  if (queue->size == 0)
    return NULL;

  /* Check for the end of the block */
  queue_eob (queue);

  /* Update Queue Size */
  queue->size--;

  return queue->head->data[queue->head->deq_pos++];
}

size_t
queue_size (queue_t queue)
{
  return queue->size;
}

queue_error_t
queue_enq (queue_t queue, void * data)
{
  size_t size;

  /* Empty List Check */
  if (queue->tail == NULL ||
      queue->tail->size == queue->tail->len)
    {
      /* Allocate a New Node if we don't have one */
      if (queue->unused == NULL)
	{
	  /* Determine the size of the new node */
	  if (queue->size <= BLOCK_SIZE)
	    size = BLOCK_SIZE;
	  else
	    size = queue->size;

	  /* Allocate memory for the node and it's data segment */
	  queue->unused = (queue_node_t) malloc (sizeof (struct _queue_node_t) + 
						 size * sizeof (void *));

	  /* Check for malloc error */
	  if (queue->unused == NULL)
	    return QUEUE_MALLOC_FAILED;

	  /* Initialize the array */
	  queue->unused->size = size;
	  queue->unused->data = (void **)((uint8_t*)queue->unused + sizeof (struct _queue_node_t));
	}
  
      /* Initialize the unused queue node */
      queue->unused->prev = queue->tail;
      queue->unused->next = NULL;
      queue->unused->len = 0;
      queue->unused->deq_pos = 0;

      /* Fix the old list if it exists */
      if (queue->tail != NULL)
	queue->tail->next = queue->unused;
      
      /* Inject the node */
      else
	queue->head = queue->unused;
      queue->tail = queue->unused;
      queue->unused = NULL;
    }

  /* Add the element to the list */
  queue->tail->data[queue->tail->len++] = data;

  /* Increase the size*/
  queue->size++;

  return QUEUE_OK;
}

void
queue_clear (queue_t queue)
{
  queue_node_t tmp;

  /* Check to see if we can store an unused block */
  if (queue->unused == NULL && queue->tail != NULL)
    {
      queue->unused = queue->tail;
      if (queue->head == queue->tail)
	queue->head = NULL;
      else
	queue->tail->prev->next = NULL;
    }

  /* Iterate over the list freeing it */
  while (queue->head != NULL)
    {
      tmp = queue->head;
      queue->head = queue->head->next;
      free (tmp);
    }

  /* Cleanup queue */
  queue->tail = NULL;
  queue->size = 0;
}

void
queue_destroy (queue_t queue)
{
  /* Clears the queue */
  queue_clear (queue);

  /* Free the unused node */
  if (queue->unused != NULL)
    free (queue->unused);

  /* Free the queue structure */
  free (queue);
}
