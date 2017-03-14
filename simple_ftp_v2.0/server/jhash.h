/* jhash.h: Jenkins hash support.
 *
 * Copyright (C) 2006. Bob Jenkins (bob_jenkins@burtleburtle.net)
 *
 * http://burtleburtle.net/bob/hash/
 *
 * These are the credits from Bob's sources:
 *
 * lookup3.c, by Bob Jenkins, May 2006, Public Domain.
 *
 * These are functions for producing 32-bit hashes for hash table lookup.
 * hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final()
 * are externally useful functions.  Routines to test the hash are included
 * if SELF_TEST is defined.  You can use this free for any purpose.  It's in
 * the public domain.  It has no warranty.
 *
 * Copyright (C) 2009-2010 Jozsef Kadlecsik (kadlec@blackhole.kfki.hu)
 *
 * I've modified Bob's hash to be useful in the Linux kernel, and
 * any bugs present are my fault.
 * Jozsef
 *
 * Copyright (C) 2012 Tilera Corporation.
 *
 * We have modified Jozsef's hash to be useful in Tilera Netlib, and
 * any bugs present are our fault.
 */

#ifndef __JHASH_H__
#define __JHASH_H__

#ifdef __cplusplus
extern "C" {
#endif 

/**
 * rol32 - rotate a 32-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline unsigned int rol32(unsigned int word, unsigned int shift)
{
	return (word << shift) | (word >> (32 - shift));
}

/* Best hash sizes are of power of two */
#define jhash_size(n)   ((unsigned int)1<<(n))
/* Mask the hash value, i.e (value & jhash_mask(n)) instead of (value % n) */
#define jhash_mask(n)   (jhash_size(n)-1)

/* __jhash_mix -- mix 3 32-bit values reversibly. */
#define __jhash_mix(a, b, c)			\
{						\
	a -= c;  a ^= rol32(c, 4);  c += b;	\
	b -= a;  b ^= rol32(a, 6);  a += c;	\
	c -= b;  c ^= rol32(b, 8);  b += a;	\
	a -= c;  a ^= rol32(c, 16); c += b;	\
	b -= a;  b ^= rol32(a, 19); a += c;	\
	c -= b;  c ^= rol32(b, 4);  b += a;	\
}

/* __jhash_final - final mixing of 3 32-bit values (a,b,c) into c */
#define __jhash_final(a, b, c)			\
{						\
	c ^= b; c -= rol32(b, 14);		\
	a ^= c; a -= rol32(c, 11);		\
	b ^= a; b -= rol32(a, 25);		\
	c ^= b; c -= rol32(b, 16);		\
	a ^= c; a -= rol32(c, 4);		\
	b ^= a; b -= rol32(a, 14);		\
	c ^= b; c -= rol32(b, 24);		\
}

/* An arbitrary initial parameter */
#define JHASH_INITVAL		0xdeadbeef

/* jhash_3words - hash exactly 3, 2 or 1 word(s) */
static inline unsigned int jhash_3words(unsigned int a, unsigned int b, unsigned int c,
                                    unsigned int initval)
{
  a += JHASH_INITVAL;
  b += JHASH_INITVAL;
  c += initval;

  __jhash_final(a, b, c);

  return c;
}

static inline unsigned int jhash_2words(unsigned int a, unsigned int b, unsigned int initval)
{
  return jhash_3words(a, b, 0, initval);
}

static inline unsigned int jhash_1word(unsigned int a, unsigned int initval)
{
  return __insn_crc32_32(a, 0x12345678);
 // return jhash_3words(a, 0, 0, initval);
}

#ifdef __cplusplus
}
#endif  // cplusplus 

#endif  // __NETLIB_JHASH_H__ 
