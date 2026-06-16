#ifndef NODE_H
#define NODE_H

// node
typedef struct linked_list {
	struct linked_list *link; // link to next item
	unsigned int size; // size of pointer
	void *pointer;     // pointer memory
} linked_list;

#include <stdlib.h> // for malloc and free
#ifndef NULL
#define NULL (void *)0
#endif // NULL

// HELPER FUNCTIONS -------------------------------------------------
#define node_h_loop(ref) \
	linked_list *cur = ref; \
	while (cur != NULL)
#define node_h_loop_next \
	cur = cur->link
#define node_h_unset(node) \
	if (node->pointer) \
		free(node->pointer); \
	node->pointer = NULL; \
	node->size = 0

#define nstrdup(ref, ptr) node_string(ref, ptr)

/**
 * Gets the length of a given string
 **/
static int
node_h_strlen(const char *str)
{
	int str_s = 0;
	if (str != NULL)
		while (str[str_s])
		   str_s++;
	return str_s;
}

/**
 * Finds the next free node in memory
 **/
linked_list *
node_h_next_empty(struct linked_list *ref)
{
	node_h_loop(ref){
		if (cur->pointer == NULL
				|| cur->size == 0)
			return cur;
		node_h_loop_next;
	}

	// Could not find an empty node
	return NULL;
}

/**
 * Finds a node in the linked list based on a given value
 **/
linked_list *
node_h_find (linked_list *ref, void *pointer)
{
	node_h_loop(ref){
		if (cur->pointer == pointer)
			return cur;
		node_h_loop_next;
	}
	return NULL;
}


/**
 * Creates a new node at the end of the linked list
 **/
linked_list *
node_h_create (linked_list **ref)
{
	linked_list *new_node = calloc (1, sizeof (linked_list));
	if (new_node == NULL) return NULL;
	
	node_h_unset(new_node);
	new_node->link = NULL;
	
	if (*ref == NULL) {
		*ref = new_node;
	} else {
		linked_list *cur = *ref;
		while (cur->link != NULL) {
			cur = cur->link;
		}
		cur->link = new_node;
	}
	return new_node;
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

// FUNCTION DEFINITIONS ---------------------------------------------

/**
 * Allocates memory in the linked list
 **/
static void *
node_malloc(linked_list **ref, int size)
{
	linked_list *cur = node_h_next_empty(*ref);

	// Failed to locate next node
	if (cur == NULL)
		cur = node_h_create(ref);

	cur->pointer = malloc(size);

	// Failed to malloc
	if (cur->pointer == NULL)
		return NULL;

	cur->size = size;
	return cur->pointer;
}

/**
 * Frees memory in the linked list
 **/
static int
node_free(linked_list *ref, void *pointer)
{
	// find the node from the given pointer
	linked_list *cur = node_h_find(ref, pointer);
	if (cur == NULL)
		return 1;

	// free and abolish the rest of the data
	node_h_unset(cur);
	return 0;
}

/**
 * Removes all the items in the linked list
 **/
static int
node_destroy(linked_list *ref)
{
	node_h_loop(ref){
		linked_list *next = cur->link;
		node_h_unset(cur);
		
		// free the actual node
		free(cur);
		cur = next;
	}
	return 0;
}

/**
 * Allocates a buffer and copies data into it
 **/
static void *
node_dup(linked_list **ref, void *pointer, int pointer_s)
{
	void *new_buf = node_malloc(ref, pointer_s);
	if (new_buf == NULL) return NULL;
	
	node_h_memcpy(new_buf, pointer, pointer_s);
	return new_buf;
}

/**
 * Specifically handles null-terminated strings
 **/
static char *
node_string(linked_list **ref, char *str)
{
	if (str == NULL) return NULL;
	
	unsigned int str_s = node_h_strlen(str);
	
	// We need str_s + 1 to account for the null terminator '\0'
	char *out = (char *)node_malloc(ref, str_s + 1);
	if (out == NULL) return NULL;
	
	node_h_memcpy(out, str, str_s);
	out[str_s] = '\0'; // Manually terminate
	
	return out;
}

/**
 * Re-allocates data in a pointer
 **/
static void *
node_realloc(linked_list **ref, void *pointer, int new_size)
{
	linked_list *cur = node_h_find(*ref, pointer);
	
	// pointer does not exist
	if (cur == NULL)
		return NULL;
	
	void * new_data = realloc(cur->pointer, new_size);
	if (!new_data)
		// Failed to realloc
		return NULL;

	cur->pointer = new_data;
	cur->size = new_size;
	return new_data;
}

/**
 * Returns the total allocated memory
 **/
static int
total_mem_used(linked_list **ref){
	linked_list *cur = *ref;
	unsigned int total = 0;
	while (cur->link != NULL) {
		total += cur->size;
		cur = cur->link;
	}

	return total;
}

#endif // NODE_H
