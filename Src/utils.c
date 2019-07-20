#include "utils.h"

unsigned int busy_wait(unsigned int delay)
{
	unsigned int d = 0;
	while ( d < delay )
		++d;
	return d;
}