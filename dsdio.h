/**
* DSD Unpack - https://github.com/michaelburton/dsdunpack
*
* Copyright (c) 2014 by Michael Burton.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#ifndef DSDIO_H_INCLUDED
#define DSDIO_H_INCLUDED

#include <inttypes.h>
#include <stdio.h>
#include "endianess.h"


/* Supported file format markers */
#define DSD_FORMAT_DSDIFF MAKE_MARKER('F', 'R', 'M', '8')
#define DSD_FORMAT_DSF    MAKE_MARKER('D', 'S', 'D', ' ')


/* Reading */
struct dsd_reader_t;

typedef struct dsd_reader_funcs_t {
    int      (*open)      (FILE *fp, struct dsd_reader_t *reader);
    size_t   (*read)      (char *buf, size_t len, struct dsd_reader_t *reader);
    uint32_t (*next_chunk)(struct dsd_reader_t *reader);
    void     (*close)     (struct dsd_reader_t *reader);
} dsd_reader_funcs_t;

typedef struct dsd_reader_t {
    FILE               *input;

    uint8_t             channel_count;
    uint32_t            sample_rate;
    uint64_t            data_length;
    uint8_t             compressed;

    uint32_t            container_format;
    void               *private;
    dsd_reader_funcs_t *impl;
} dsd_reader_t;

extern int      dsd_reader_open(FILE *fp, dsd_reader_t *reader);
extern size_t   dsd_reader_read(char *buf, size_t len, dsd_reader_t *reader);
extern uint32_t dsd_reader_next_chunk(dsd_reader_t *reader);
extern void     dsd_reader_close(dsd_reader_t *reader);


/* Writing */
struct dsd_writer_t;

typedef struct dsd_writer_funcs_t {
    void (*open)      (struct dsd_writer_t *writer);
    void (*write)     (const char *buf, size_t len, struct dsd_writer_t *writer);
    int  (*next_chunk)(uint32_t chunk, struct dsd_writer_t *writer);
    void (*close)     (struct dsd_writer_t *writer);
} dsd_writer_funcs_t;

typedef struct dsd_writer_t {
    FILE               *output;

    uint8_t             channel_count;
    uint32_t            sample_rate;
    uint64_t            data_length;

    void               *private;
    dsd_writer_funcs_t *impl;
} dsd_writer_t;

extern int  dsd_writer_open(FILE *fp, uint32_t format, uint32_t sample_rate, uint8_t channel_count, dsd_writer_t *writer);
extern void dsd_writer_write(const char *buf, size_t len, dsd_writer_t *writer);
extern int  dsd_writer_next_chunk(uint32_t chunk, dsd_writer_t *writer);
extern void dsd_writer_close(dsd_writer_t *writer);

#endif /* DSDIO_H_INCLUDED */
