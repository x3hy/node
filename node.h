// vim: colorcolumn=55
#ifndef ARENA_H
#if __cplusplus
extern "C" {
#endif
	
	// Misc dependencies
	#define ARENA_H

	#ifndef NULL
	#define NULL (void *)0
	#endif	

	#include <stdlib.h> // malloc and free are the only
						// dependencies.

	// Main node
	typedef struct node {
		void *ptr;
		unsigned int size;
		unsigned int has_next;
		struct node *next;
	} Arena;

	#define __node_loop(start) \
		struct node *_cur = \
			(start->next) \
				? start->next \
				: start; \
		while (_cur)

	#define __node_loop_inc \
		_cur = _cur->next

	#include <stdio.h>

	// Allocate some new data
	static struct node *
	node_malloc (struct node *node, int size)
	{
		// Get the next available node
		__node_loop (node)
		  {
			if (_cur->size != 0)
				__node_loop_inc;

			else break;
		  }
		// Something went wrong
		if (!_cur){
			printf("failed to generate");
			return node;
		}

		// Assign the value and the size
		_cur->ptr = malloc (_cur->size = size);

		// Create a new node if needed.
		_cur->next = (_cur->next)
			? _cur->next
			: calloc (1, sizeof(struct node));
		
		// Return the pointer
		return _cur;
	}

	// frees a given node
	static void
	node_free (struct node *node)
	{
		free(node->ptr);
	}
	

	// frees ALL memory and destroys nodes, the
	// node_free function just removes the data.
	static void
	node_destroy (struct node *node)
	{
		unsigned int n_count = 0;
		__node_loop(node)
		  {
			n_count++;
			__node_loop_inc;
		  }

		for (int i = n_count; i > 0; i++)
		  {
			__node_loop(node)
				__node_loop_inc;

			free(_cur->ptr);
			free(_cur->next);
		  }
	}
	
	// A default node
	struct node *__node = {0};
	#define a_malloc(size) node_malloc(__node, size)
	#define a_free(ref) node_free(__node, ref)
	#define a_destroy() node_destroy(__node)

#ifdef __cplusplus
}
#endif
#endif // !ARENA_H
