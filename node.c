#include <stdio.h>
#include "node.h"

int main(){
	struct node *msg = a_malloc(20);

	// cast to a char*
	char *my_msg = (char *)msg->ptr;

	printf("%s\n", my_msg);

	node_free(msg);
	a_destroy();
}
