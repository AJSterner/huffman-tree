#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct Bitstream_T *Bitstream_T;

Bitstream_T Bitstream_new(FILE *stream);
void Bitstream_free(Bitstream_T *bstream);

/*
 *      Puts bits number of least significant bits on bstream
 */
void Bitstream_put(Bitstream_T bstream, uint64_t word, uint8_t bits);

/*
 *      Put bit on stream
 */
void Bitstream_putb(Bitstream_T bstream, uint8_t bit);

/*
 *      If remaining bits on stream emits them with trailing zeros
 */
void Bitstream_flush(Bitstream_T bstream);

/*
 *      Returns next bits number of bits
 */
uint64_t Bitstream_get(Bitstream_T bstream, uint8_t bits);
