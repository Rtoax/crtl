
#include "crtl/network/crtl_network_inet.h"

/*	file name: 		if.c
 *	author:			Rong Tao
 *	create time:	2018.11.14
 * 	
 */


#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
#include <malloc.h>
#include <string.h>

#include "crtl/crtl_log.h"

/**
 *	Show all if_nameindex in terminal
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
int crtl_if_showallnameindexs(void)
{
	t_if_nameindex *head, *ifni;
	ifni = t_if_nameindex();
    head = ifni;

	if (head == NULL) 
	{
        perror("t_if_getallnameindexs()");
        return -1;
    }

	while (ifni->if_index != 0) 
	{
        printf("Interface %d : %s\n", ifni->if_index, ifni->if_name);
        ifni++;
    } 
	
	t_if_freenameindex(head);
    head = NULL;
    ifni = NULL;
	return 0;
}

/**
 *	is if_index exist
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
int crtl_if_indexexist(unsigned int if_index)
{
	char *if_name = (char*)malloc(T_IFNAMSIZ*sizeof(char));
	char *name = t_if_indextoname(if_index, if_name);
	
    if (name == NULL) 
	{
        perror("t_if_indextoname()");
		free(if_name);
        return -1;
    }
	else
	{
		crtl_print_err("\033[31m%d:%s\033[0m\n", if_index, if_name);
		free(if_name);
		return 0;
	}
}


/**
 *	is if_name exist
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
int crtl_if_nameexist(const char *if_name)
{
	if(if_name == NULL)
	{
		return -1;
	}

	unsigned int if_index;
	t_if_nameindex *head, *ifni;
	ifni = t_if_nameindex();
    head = ifni;

	if (head == NULL) 
	{
        perror("t_if_nameexist()");
        return -1;
    }

	while (ifni->if_index != 0) 
	{
        if(strcmp(if_name, ifni->if_name) == 0)
    	{
    		if_index = ifni->if_index;
			t_if_freenameindex(head);
		    head = NULL;
		    ifni = NULL;
			return if_index;
    	}
        ifni++;
    } 
	
	t_if_freenameindex(head);
    head = NULL;
    ifni = NULL;
	return -1;
}


