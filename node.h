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

	static void *
	node_point(struct node **ref, void *point, int ptr_size)
	{
		// Allocate node
		const unsigned char *ptr = (unsigned char *)point;
		int ptr_rel_size = 0;
		while (ptr[ptr_rel_size])
			ptr_rel_size++;

		// Copy ptr over to node
		unsigned char *d = (unsigned char *)node_malloc(ref, ptr_size) ;
		for (int i = 0; i < ptr_rel_size; i++)
			d[i] = ptr[i];

		return (void *)d;
	}	

	static void *
	node_realloc(struct node **ref, void* old_node, int new_size)
	{
		void *new_node = node_point(ref, old_node, new_size);
		node_free(*ref, old_node);

		return new_node;
	}
	
	static void *
	node_string(struct node **ref, char *str)
	{
		// Get size of pointer
		int ptr_size = 0;
		while (str[ptr_size])
			ptr_size++;

		// Write str to a node
		unsigned char *dest =
			(unsigned char *)node_point(ref, str, ptr_size+1);

		// Null terminate
		dest[ptr_size] = '\0';
		return (void *)dest;
	}

	#ifdef CREATE_EXAMPLE_NODE
		// A default node
		static struct node *__node = NULL;
		#define nalloc(size) node_malloc(&__node, size)
		#define nfree(ptr) node_free(__node, ptr)
		#define ndel() node_destroy(&__node)
		#define ncpy(ptr) node_point(&__node, ptr)
		#define nstr(str) node_string(&__node, str);
		#define nrealloc(ptr, nsize) node_realloc(&__node, ptr, nsize);
	#endif // CREATE_EXAMPLE_NODE

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !NODE_H
