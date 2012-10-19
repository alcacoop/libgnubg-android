/*
 * mt19937int.h
 *
 * $Id: mt19937ar.h,v 1.2 2004/05/03 13:17:35 thyssen Exp $
 */

#ifndef _MT19937AR_H_
#define _MT19937AR_H_

#define N 624

extern void init_genrand( unsigned long s, int *mti, unsigned long mt[ N ] );
extern unsigned long genrand_int32( int *mti, unsigned long mt[ N ] );
    
#endif
