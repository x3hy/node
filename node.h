// vim: colorcolumn=55
#ifndef NODE_H
#define NODE_H
#if __cplusplus
extern "C" {
#endif // __cplusplus
	// Ensure `NULL` is set
	#ifndef NULL
		#define NULL (void *)0
	#endif // NULL

	// Only dependency (`free()` and `calloc()`)
	#include <stdlib.h>

	// Main node
	typedef struct node {
		void *ptr;
		struct node *next;
	} Arena;

	// Allocate new memory
	static void *
	node_malloc (struct node **ref, int size)
	{
		struct node *cur = *ref;
		
		// Initialize the main node
		if (cur == NULL)
		  {
			cur = (struct node *)
				calloc(1, sizeof(struct node));
			*ref = cur;
		  }

		// Get the next available node
		while (cur)
		  {
			if (cur->ptr == NULL)
			  {
				cur->ptr = malloc (size);
				return cur->ptr;
			  }
			if (cur->next == NULL)
				break;

			cur = cur->next;
		  }

		// Allocate the next node
		cur->next =
			(struct node *)
			calloc(1, sizeof (struct node));

		// Recurse until the next node is available
		return node_malloc(ref, size);
	}

	// frees a given value
	static int
	node_free(struct node *ref, void *ptr)
	{
		if (ptr == NULL || ref == NULL)
			return -1;

		// Locate the pointer in the node list, when
		// its found it will be free'd
		struct node *cur = ref;
		while(cur)
		  {
			  // Compare the given pointer to the
			  // pointer of the current node
			if (cur->ptr == ptr)
			  {
				// If found then free that pointer
				free(cur->ptr);
				cur->ptr = NULL;
				return 1;
			  }
			cur = cur->next;
		  }
		return 0;
	}

	// Frees ALL memory and destroys nodes, the
	// node_free function just removes the pointer.
	static int
	node_destroy (struct node **ref)
	{
		struct node *cur = *ref;

		// Loop through every node
		while (cur)
		  {
			struct node *next = cur->next;

			// Free the nodes pointer and the node
			// itself
			free(cur->ptr);
			free(cur);

			// Move to the next node
			cur = next;
		  }
		*ref = NULL;
		return 0;
	}
	
	// A default node
	static struct node *__node = NULL;
	#define nalloc(size) node_malloc(&__node, size)
	#define nfree(str) node_free(__node, str)
	#define ndel() node_destroy(&__node)

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !NODE_H
