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

#ifndef _ADT_QUEUE_H_
#define _ADT_QUEUE_H_

/* NOTE: All queue operations run in ~O(1) time except clear / destroy */

typedef struct _queue_t * queue_t;

typedef enum _queue_error_t
  {
    QUEUE_OK = 0,
    QUEUE_MALLOC_FAILED,
    QUEUE_UNKNOWN
  } queue_error_t;

/* Creates a new queue object or returns NULL on failure */
queue_t queue_init (void);

/* Looks at the end of the queue and optionally removes an element
   Returns NULL on failure or if the queue is empty */
void * queue_peek (queue_t queue);
void * queue_deq (queue_t queue);

/* Returns the number of elements on the queue */
size_t queue_size (queue_t queue);

/* Pushes data onto the queue, returning any errors that occur */
queue_error_t queue_enq (queue_t queue, void * data);

/* Clears the queue without freeing inserted elements */
void queue_clear (queue_t queue);

/* Destroys the queue object but doesn't free the inserted elements */
void queue_destroy (queue_t queue);

#endif
