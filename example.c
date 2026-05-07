#include <stdio.h> // for printf
#include <malloc.h> // for malloc_usable_size
#include "node.h"

int main()
{
	// Allocate data to a node
	char *msg = nmalloc(20);
	node_h_memcpy (msg, "Hello World!", 20);
	printf ("%s\n", msg);

	// Free that particular node
	// nfree (msg);

	// Allocate another string node
	char *msg2 = nstrdup("Foobar!");
	printf ("%s\n", msg2);
	
	// Allocate another string node
	char *msg3 = nstrdup("test-123");
	printf ("%s\n", msg3);

	// Create more room in a node
	printf("size before realloc: %zu\n", malloc_usable_size(msg));
	msg = nrealloc(msg, 30);
	printf("size after realloc : %zu\n", malloc_usable_size(msg));

	// Delete ALL nodes
	ndelete ();
	return 0;
}
