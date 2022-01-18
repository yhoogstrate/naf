
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
        if (ZSTD_isError(initResult))
        {
            fputs("ZSTD_seekable_initCStream err @ example 1 ", stderr);
            exit(1);
        }
        
        char *data = (char *) malloc_or_die(255);
        data = "test\0";
        
        ZSTD_inBuffer input = { data, 4, 0 };
        
        
        // stream?
        //w->file = fopen(w->path, "wb+");
        //if (w->file == NULL) { die("can't create temporary file \"%s\"\n", w->path); }
        
        free(data);
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

