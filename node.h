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

	// DOCUMENTATION:

	/**
	 * node_malloc allocates a certain amount of memory.
	 * The same as malloc()
	 **/
	static void * node_malloc (struct node **ref, int size);

	/*
	 * node_free will free a value from a given node. This function will
	 * search for the given value within the linked list until it finds
	 * it. when it finds it it will free it. this could be optimised by
	 * just freeing the memory using free() and then calling node_destroy
	 * to remove any excess data.
	 **/
	static int node_free(struct node *ref, void *ptr);

	/**
	 * node_destroy will recursivly remove ALL allocated memory in a linked
	 * list.
	 **/
	static int node_destroy (struct node **ref);

	/**
	 * node_point allows you to allocate memory in a certain size, then also
	 * memcopy a given void* value to the new node before returning the mem.
	 **/
	static void * node_point(struct node **ref, void *point, int ptr_size);

	/**
	 * node_point allows you to allocate memory in a certain size, then also
	 * memcopy a given void* value to the new node before returning the mem.
	 **/
	static void * node_point(struct node **ref, void *point, int ptr_size);

	/**
	 * node_realloc takes in a node and a new size and will create a new
	 * node with the new size and the old nodes value and return it.
	 * this function is similar to realloc.
	 **/
	static void * node_realloc(struct node **ref, void* old_node, int new_size);
	
	/**
	 * node_string returns allocated memory for the given string (str).
	 * this is the same as strdup.
	 **/
	static void * node_string(struct node **ref, char *str);

	/**
	 * node_h_memcpy is the same as memcpy, it takes in two variables
	 * a destination and a source and it will move over as much of the
	 * source that will fit into the destination.
	 **/
	static void * node_h_memcpy(void * dest, const void *sorc, int sorc_len);

	/**
	 * node_h_strlen is a copy of strlen.
	 **/
	static int node_h_strlen(const char *str);
	// DEFINITION:
	
	static int
	node_h_strlen(const char *str)
	{
		int str_s = 0;
		while (str[str_s])
		   str_s++;
		return str_s;
	}
	
	static void *
	node_h_memcpy(void * dest, const void *sorc, int sorc_len)
	{
		if (dest == NULL || sorc == NULL)
			return NULL;

		unsigned char *u_dest = (unsigned char *)dest;
		unsigned char *u_sorc = (unsigned char *)sorc;
		
		// Copy the bytes over
		while (sorc_len--)
			*u_dest++ = *u_sorc++;

		return dest;
	}

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



	#ifndef NO_DEFAULT_NODE
		// A default node
		static struct node *__default_node = NULL;

		// Default node macros
		#define nmalloc(size) node_malloc(&__default_node, size)
		#define nfree(ptr) node_free(__default_node, ptr)
		#define ndelete() node_destroy(&__default_node)
		#define nmemcreate(ptr) node_point(&__default_node, ptr)
		#define nstrdup(str) node_string(&__default_node, str)
		#define nrealloc(ptr, nsize) node_realloc(&__default_node, ptr, nsize)
	#endif // CREATE_EXAMPLE_NODE

	// Extra feature macros
	#define nstrcpy(dest, src) node_h_memcpy(dest, src, node_h_strcpy(src))
	#define nmemcopy(dest, src, len) node_h_memcpy(dest, src, len)

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !NODE_H
