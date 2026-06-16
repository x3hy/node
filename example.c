#include <stdio.h> // for printf
#include <malloc.h> // for malloc_usable_size
#include "node2.h"

int main()
{
	// Define a starting node
	linked_list *table = NULL;

	char *msg = node_malloc(&table, 20);
	node_h_memcpy(msg, "test123", 7);
	msg[7] = '\0';

	printf("%s\n", msg);

	// alternatively you can strdup
	char *msg2 = nstrdup(&table, "Hello World!");
	printf ("%s\n", msg2);

	// only free the second message
	// (would lead to a memory leak 2 alloc 1 free)
	node_free(table, msg2);

	// realloc time
	printf("Size before realloc: %zu\n", malloc_usable_size(msg));
	msg = node_realloc(&table, msg, 30);
	printf("Size after realloc : %zu\n", malloc_usable_size(msg));

	printf("total size used: %d", total_mem_used(&table));

	// at this point, if we exit it would be 3 allocs 1 free.
	node_destroy(table);

	// entire table has been freed now
	return 0;
}
