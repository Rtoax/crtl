#ifndef __CRTL_BITS_BITS_FIND_H 
#define __CRTL_BITS_BITS_FIND_H 1


/**
 * DOC: bitmap bitops
 *
 *  crtl_set_bit(bit, addr)                  *addr |= bit
 *  crtl_clear_bit(bit, addr)                *addr &= ~bit
 *  crtl_change_bit(bit, addr)               *addr ^= bit
 *  crtl_test_bit(bit, addr)                 Is bit set in *addr?
 *  crtl_test_and_set_bit(bit, addr)         Set bit and return old value
 *  crtl_test_and_clear_bit(bit, addr)       Clear bit and return old value
 *  crtl_test_and_change_bit(bit, addr)      Change bit and return old value
 *  crtl_find_first_zero_bit(addr, nbits)    Position first zero bit in *addr
 *  crtl_find_first_bit(addr, nbits)         Position first set bit in *addr
 *  crtl_find_next_zero_bit(addr, nbits, bit)
 *                                      Position next zero bit in *addr >= bit
 *  crtl_find_next_bit(addr, nbits, bit)     Position next set bit in *addr >= bit
 *  crtl_find_next_and_bit(addr1, addr2, nbits, bit)
 *                                      Same as crtl_find_next_bit, but in
 *                                      (*addr1 & *addr2)
 *
 */
unsigned long crtl_find_next_bit(const unsigned long *addr, unsigned long size,
                                unsigned long offset);

unsigned long crtl_find_first_bit(const unsigned long *addr, unsigned long size);

unsigned long crtl_find_first_zero_bit(const unsigned long *addr, unsigned long size);


unsigned long crtl_find_next_zero_bit(const unsigned long *addr, unsigned long size,
				                    unsigned long offset);

unsigned long crtl_find_next_and_bit(const unsigned long *addr1,
                                  const unsigned long *addr2, unsigned long size,
                                  unsigned long offset);

unsigned long crtl_find_last_bit(const unsigned long *addr, unsigned long size);


/**
 * crtl_find_next_clump8 - find next 8-bit clump with set bits in a memory region
 * @clump: location to store copy of found clump
 * @addr: address to base the search on
 * @size: bitmap size in number of bits
 * @offset: bit offset at which to start searching
 *
 * Returns the bit offset for the next set clump; the found clump value is
 * copied to the location pointed by @clump. If no bits are set, returns @size.
 */
extern unsigned long crtl_find_next_clump8(unsigned long *clump,
				      const unsigned long *addr,
				      unsigned long size, unsigned long offset);

#define crtl_find_first_clump8(clump, bits, size) crtl_find_next_clump8((clump), (bits), (size), 0)




#endif /*<__CRTL_BITS_BITS_FIND_H>*/
