/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */
#include <stdio.h>
#include "oshw.h"

/**
 * Host to Network byte order (i.e. to big endian).
 *
 * Note that Ethercat uses little endian byte order, except for the Ethernet
 * header which is big endian as usual.
 */
uint16 oshw_htons (uint16 data)
{
	uint16_t tmp = 0;
	tmp		=	(data & 0x00ff) << 8;
	tmp		|=	(data & 0xff00) >> 8;
	data	=	tmp;
	return data;
}

/**
 * Network (i.e. big endian) to Host byte order.
 *
 * Note that Ethercat uses little endian byte order, except for the Ethernet
 * header which is big endian as usual.
 */
uint16 oshw_ntohs (uint16 data)
{
	uint16_t tmp = 0;
	tmp		=	(data & 0x00ff) << 8;
	tmp		|=	(data & 0xff00) >> 8;
	data	=	tmp;
	return data;
}

/* Create list over available network adapters.
 * @return First element in linked list of adapters
 */
ec_adaptert * oshw_find_adapters (void)
{
    // Not implemented
    // because Arduino usually has only one LAN port
    ec_adaptert * ret_adapter = NULL;
    return ret_adapter;
}

/** Free memory allocated memory used by adapter collection.
 * @param[in] adapter = First element in linked list of adapters
 * EC_NOFRAME.
 */
void oshw_free_adapters (ec_adaptert * adapter)
{
    // Not implemented
    // because Arduino usually has only one LAN port
}
