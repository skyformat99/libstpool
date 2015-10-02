#ifndef __OBJPOOL_H__
#define __OBJPOOL_H__

#include "sm_cache.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 *  COPYRIGHT (C) 2014 - 2020, piggy_xrh
 * 
 *	  Stpool is portable and efficient tasks pool library, it can work on diferent 
 * platforms such as Windows, linux, unix and ARM.  
 *
 *    If you have any troubles or questions on using the library, contact me.
 *
 * 	  (Email: piggy_xrh@163.com  QQ: 1169732280)
 */

/**********************************A fast object pool*****************************/
typedef struct obj_block {
	/* The address of the objects array */
	char *begin, *end;
	
	/* The free queue of the objects */
	smlink_q_t putq;
} obj_block_t;

typedef struct objpool {
	/* Block records array */
	obj_block_t *blocks;
	int iblocks, ialloc;
	
	/* The memory size for each object block */
	size_t block_size;

	/* The limited object number that one block can store */
	size_t block_nobjs;
	
	/* The length of the object */
	int objlen;

	/* The object cache */
	smcache_t smc;

	/* The allocated (free) task objects */
	int ntotal, nput;
} objpool_t;


/* Object pool constructor */
int  objpool_ctor2(objpool_t *p,      /* the instance of the Object pool */
				   const char *name,  /* A const string to describe this object pool */
				   int objlen,        /* The length of object producted by the pool */
				   int nreserved,     /* The reserved number of objects */
				   int nlimite_cache, /* The limited numbef of the cached objects */
				   OSPX_pthread_mutex_t *cache_lock /* The lock who is used to synchronize the datas */
				  );

static inline int  objpool_ctor(objpool_t *p, const char *name, int objlen, int nreserved) {
	return objpool_ctor2(p, name, objlen, nreserved, 0 /* default */, NULL);
}

/* Object pool destructor */
void objpool_dtor(objpool_t *p);


/* Retreive the underly cache of the object pool */
static inline smcache_t *
objpool_get_cache(objpool_t *p) {
	return &p->smc;
}

/* Retreive the pool name passed to the constructor */
static inline const char *objpool_name(objpool_t *p) {
	return smcache_name(&p->smc);
}

#ifdef __cplusplus
}
#endif

#endif
