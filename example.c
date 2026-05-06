#include <stdio.h> // for printf
#include <string.h> // for strcpy
#include <malloc.h>

#define CREATE_EXAMPLE_NODE
#include "node.h"

int main()
{
	// Allocate data to a node
	char *msg = nalloc(20);
	strcpy (msg, "Hello World!");
	printf ("%s\n", msg);

	// Free that particular node
	// nfree (msg);

	// Allocate another node
	char *msg2 = nstr("Foobar!");
	printf ("%s\n", msg2);
	
	// Allocate another node
	char *msg3 = nstr("test-123");
	printf ("%s\n", msg3);

	// Create more room in a node
	printf("size before: %zu\n", malloc_usable_size(msg));
	msg = nrealloc(msg, 30);
	printf("size after : %zu\n", malloc_usable_size(msg));

	// Delete ALL nodes
	ndel ();
	return 0;
}
