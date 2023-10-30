#include "utils.h"
#include <stdio.h>

void swap32s(uint32_t* buf, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		*buf = bswap_32(*buf);
		buf++;
	}
}
