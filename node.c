#include <stdio.h>
#include <string.h>
#include "node.h"

int main(){

	// Allocate data to a node
	char *msg = nalloc(20);
	strcpy(msg, "test123");
	printf("%s\n", msg);

	// Free that node
	nfree(msg);

	// Delete ALL nodes
	ndel();

	// Allocate more data
	char *msg2 = nalloc(20);
	strcpy(msg2, "test123");
	printf("%s\n", msg2);

	// Don't free the node
	//nfree(msg2);

	// Delete ALL nodes
	ndel();
	return 0;
}
