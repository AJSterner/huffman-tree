#include <assert.h>
#include "bitstream.h"


struct Bitstream_T {
        FILE *stream;
        int buffer;
        int currbit;
};

void emit(Bitstream_T bstream);
uint64_t left_shift(uint64_t word, unsigned shift);

/*
 *      return new bitstream
 */
Bitstream_T Bitstream_new(FILE *stream)
{
        assert(stream != NULL);
        Bitstream_T bstream = malloc(sizeof(*bstream));
        bstream->stream = stream;
        bstream->buffer = 0;
        bstream->currbit = 7;
        return bstream;
}

/*
 *      free memory associated with bitstream
 */
void Bitstream_free(Bitstream_T *bstream)
{
        assert(bstream != NULL && *bstream != NULL);
        free(*bstream);
        *bstream = NULL;
}

/*
 *      Puts bits number of least significant bits on bstream
 */
void Bitstream_put(Bitstream_T bstream, uint64_t word, uint8_t bits)
{
        assert(bstream != NULL);
        uint64_t mask;
        uint64_t emit_word;
        unsigned num_to_emit;

        while (bits > 0) {
                if (bits < bstream->currbit + 1) {
                        num_to_emit = bits;
                } else {
                        num_to_emit = bstream->currbit + 1;
                }

                mask = (uint64_t)(~0) << (64 - num_to_emit) >> (64 - bits);
                emit_word = (mask & word) >> (bits - bstream->currbit - 1);

                bstream->buffer |= emit_word;
                bstream->currbit -= num_to_emit;
                emit(bstream);

                bits -= num_to_emit;
        }
}

/*
 *      put single bit on stream
 */
void Bitstream_putb(Bitstream_T bstream, uint8_t bit)
{
        assert(bit <= 1);
        if (bit == 1) {
                bstream->buffer |= bit << bstream->currbit;
        }
        bstream->currbit--;
        emit(bstream);
}

/*
 *      If remaining bits on stream emits them with trailing zeros
 */
void Bitstream_flush(Bitstream_T bstream)
{
        assert(bstream != NULL);
        if (bstream->currbit < 7) {
                bstream->currbit = -1;
                emit(bstream);
        }
}

/*
 *      Gets next bit
 */
int Bitstream_getb(Bitstream_T bstream)
{
        assert(bstream != NULL);

        if (bstream->buffer == EOF) {
                return EOF;
        }

        return ((uint8_t)bstream->buffer) << 7 - bstream->currbit >> 7;
}


void ensure_buffer(Bitstream_T bstream)
{
        if (bstream->currbit == -1) {
                bstream->currbit = 7;
                bstream->buffer = fgetc(bstream->stream);
        }
}

void emit(Bitstream_T bstream)
{
        if (bstream->currbit == -1) {
                fputc(bstream->buffer, bstream->stream); //TODO: check error?
                bstream->buffer = 0;
                bstream->currbit = 7;
        }
}

uint64_t left_shift(uint64_t word, unsigned shift)
{
        if (shift >= 64) {
                return 0;
        }

        return word << shift;
}