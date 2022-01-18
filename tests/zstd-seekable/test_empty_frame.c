
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>


#include "zstd_seekable_utils.h"
#include "zstdseek_compress.c"
#include "zstdseek_utils.c"

#include "utils.c"

#include "common/xxhash.h"


const int level = 1;


int main() {
    // [ ---- compress using ZSTD Seekable --- ]
    // example 1: w/ a string
    {
        ZSTD_seekable_CStream *s = ZSTD_seekable_createCStream();
        if (s == NULL) {
            fputs("wrong alloc @ example 1 ", stderr);
            exit(1);
        }
        
        size_t const initResult = ZSTD_seekable_initCStream(s, level, 1, 1024 * 1024);//1024 * 1024 = framesize
        if (ZSTD_isError(initResult)) {
            fputs("ZSTD_seekable_initCStream err @ example 1 ", stderr);
            exit(1);
        }
        
        char *data_in = (char *) malloc_or_die(255 * sizeof(char));
        data_in = "test\0";
        char *data_out = (char *) malloc_or_die(255 * 255 * sizeof(char));
        
        ZSTD_inBuffer input = { data_in, 4, 0 }; // [start of input buffer, size, pos] uncompressed data?
        ZSTD_outBuffer output = { data_out, 255*255, 0 };
        
        size_t toRead = ZSTD_seekable_compressStream(s, &output, &input);
        if (ZSTD_isError(toRead)) {
            fputs("ZSTD_seekable_compressStream @ example 1 ", stderr);
            exit(1);
        }
        
        printf("example 1: toRead = %li\n",  toRead);
        
        size_t const remainingToFlush = ZSTD_seekable_endStream(s, &output);
        if (remainingToFlush != 0) { die("can't end zstd stream\n"); }
        
        // stream?
        //w->file = fopen(w->path, "wb+");
        //if (w->file == NULL) { die("can't create temporary file \"%s\"\n", w->path); }
        
        //free(data_in);
        //free(data_out);
        //free(s);
    }
    
    // example 2: empty string
    {
    }
    
    // [ ---- compress using ZSTD [classic] --- ]
    {
        // example 3: w/ a string
        //ZSTD_CStream *s = ZSTD_createCStream();
        //size_t const initResult = ZSTD_initCStream(s, level);
        
        // example 4: empty string
        
    }
    
    
    
    // decompress using ZSTD
    {
        // example 1
        // assert
        
        // example 2
        // assert
        
        // example 3
        // assert
        
        // example 4
        // assert
    }
    
    return 0;
}

