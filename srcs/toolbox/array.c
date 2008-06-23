/* 
 * Copyright (c) 2005-2008 by KoanLogic s.r.l. - All rights reserved.  
 */

static const char rcsid[] =
    "$Id: array.c,v 1.1 2008/06/23 16:27:35 tho Exp $";

#include <u/libu_conf.h>
#include <u/libu.h>
#include <toolbox/array.h>

struct u_array_s
{
    void *base;
    size_t sz, nelem;
};

int u_array_create (size_t sz, u_array_t **pa)
{
    u_array_t *a = NULL;

    dbg_return_if (pa == NULL, ~0); 

    a = u_zalloc(sizeof(u_array_t));
    warn_err_sif (a == NULL);

    /* if we've been requested to allocate some slot, do it now */
    if ((a->sz = sz) != 0)
        warn_err_sif ((a->base = u_zalloc(sz * sizeof(a->base))) == NULL);

    *pa = a;
    
    return 0;
err:
    return ~0;
}

void u_array_free (u_array_t *a)
{
    dbg_return_if (a == NULL, );

    U_FREE(a->base);
    u_free(a);

    return;
}

/* if realloc fails the memory at a->base is still valid */
int u_array_grow (u_array_t *a, size_t more)
{
    size_t new_sz;

    dbg_return_if (a == NULL, ~0);

    new_sz = (more == U_ARRAY_GROW_AUTO) ? a->sz * 2 : a->sz + more;

    a->base = u_realloc(a->base, new_sz);
    warn_err_sif (a->base == NULL);

    a->sz = new_sz;

    return 0; 
err:
    return ~0;
}

int u_array_add (u_array_t *a, void *elem)
{
    void *ptr;

    dbg_return_if (a == NULL, ~0);

    /* if no space is available, grow the array by the default factor */
    if (a->nelem == a->sz)
        warn_err_if (u_array_grow(a, U_ARRAY_GROW_AUTO));
    
    /* stick the supplied element on top and increment nelem counter */
    ptr = a->base + a->nelem;
    ptr = elem;
    a->nelem += 1;

    return 0;
err:
    return ~0;
}

void *u_array_get_n (u_array_t *a, size_t idx)
{
    dbg_return_if (a == NULL, NULL);

    warn_err_ifm (idx >= a->nelem, 
            "trying to get an element out of array boundaries");

    return a->base + idx;
err:
    return NULL;
}

size_t u_array_count (u_array_t *a)
{
    dbg_return_if (a == NULL, 0);

    return a->nelem;
}

