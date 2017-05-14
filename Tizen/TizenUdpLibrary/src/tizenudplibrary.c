/**
 * This file contains the exported symbol.
 */
#include "tizenudplibrary.h"

// This is an example of an exported method.
int
tizentizenudplibrary(void)
{
	return 10;
}

int tulSendAsync(char * token, char * message, int(* callBack)(int ,int))
{
	return 0;
}
