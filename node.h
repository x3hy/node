// vim: colorcolumn=55
#ifndef NODE_H
#define NODE_H
#if __cplusplus
extern "C" {
#endif // __cplusplus

	#ifdef NODE_H_DEBUG
		#include <stdio.h>
	#else // NODE_H_DEBUG
		#ifndef NULL
			#define NULL (void *)0
		#endif // NULL
	#endif //!NODE_H_DEBUG

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
		cur->next = (struct node *)calloc(1, sizeof (struct node));
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
			if (cur->ptr == ptr)
			  {
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
		while (cur)
		  {
			struct node *next = cur->next;
			free(cur->ptr);
			free(cur);
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
