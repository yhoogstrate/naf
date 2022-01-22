
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
        printf("example 1\n");
        char *data_in = (char *) malloc_or_die(255 * sizeof(char));
        data_in = "test\0";
        int data_in_size = 4;
        
        char *data_out = (char *) malloc_or_die(255 * 255 * sizeof(char));
        int data_out_size = 0;
        
        for(int i = 0 ; i < 64; i++){
            data_out[i] = '\0';
            assert(data_out[i] == '\0');
        }

        { // scope for compressing
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
            
            
            ZSTD_inBuffer input = { data_in, data_in_size, 0 }; // [start of input buffer, size, pos] uncompressed data?
            ZSTD_outBuffer output = { data_out, 255*255, 0 };
            
            size_t toRead = ZSTD_seekable_compressStream(s, &output, &input);
            if (ZSTD_isError(toRead)) {
                fputs("ZSTD_seekable_compressStream @ example 1 ", stderr);
                exit(1);
            }
            
            size_t const remainingToFlush = ZSTD_seekable_endStream(s, &output);
            if (remainingToFlush != 0) { die("can't end zstd stream\n"); }
            
            /* for(int i = 0; i < 48; i++) {
                printf("[%i] %i\n", i , (int) data_out[i]);
            } */
            
            data_out_size = output.pos;
            printf("example 1: toRead = %li , compressed data size: %li \n",  toRead,  data_out_size);
            
            
            // export data
            FILE *file = fopen("temp/example-01__zstd__test_string.zst", "wb+");
            if(file == NULL) { die("can't create temporary file \n"); }
            fwrite(data_out, 1, output.pos, file);
            fclose(file);
            
            ZSTD_seekable_freeCStream(s);
            
            //free(data_in);
            //free(data_out);
            //free(s);
        }
        
        printf(" *** decompression ***\n");
        

        int out_buffer_size = 0;
        out_buffer_size = ZSTD_DStreamOutSize();
        static char *out_buffer = NULL;
        out_buffer = (char *) malloc_or_die(out_buffer_size);

        
        { // scope for decompressing
            static ZSTD_DStream *input_decompression_stream = NULL;
            static ZSTD_inBuffer zstd_file_in_buffer;


            input_decompression_stream = ZSTD_createDStream();
            size_t bytes_to_read = ZSTD_initDStream(input_decompression_stream);
            printf(" - bytes_to_read = %i\n", (int) bytes_to_read);
            
            ZSTD_TRY(ZSTD_DCtx_setParameter(input_decompression_stream, ZSTD_d_windowLogMax, ZSTD_WINDOWLOG_MAX));// what's this?
            
            ZSTD_inBuffer in = { data_out, data_out_size, 0 };
            ZSTD_outBuffer out = { out_buffer, 1024, 0 };
            
            bytes_to_read = ZSTD_decompressStream(input_decompression_stream, &out, &in);
            out_buffer_size = out.pos;
            out_buffer[out_buffer_size] = '\0';
        }
        
        printf(" - decompressed size: %i\n", out_buffer_size);
        printf(" - decompressed: [%s]\n", out_buffer);
        
        printf(" - checkpoint\n");

        //FREE(input_decompression_stream );
    }
    
    printf("\n\n");
    
    // example 2: empty string
    {
        printf("example 2\n");
        char *data_in = (char *) malloc_or_die(255 * sizeof(char));
        data_in = "\0";
        int data_in_size = 0;
        
        char *data_out = (char *) malloc_or_die(255 * 255 * sizeof(char));
        int data_out_size = 0;
        
        for(int i = 0 ; i < 64; i++){
            data_out[i] = '\0';
            assert(data_out[i] == '\0');
        }

        { // scope for compressing
            ZSTD_seekable_CStream *s = ZSTD_seekable_createCStream();
            if (s == NULL) {
                fputs("wrong alloc @ example 2 ", stderr);
                exit(1);
            }
            
            size_t const initResult = ZSTD_seekable_initCStream(s, level, 1, 1024 * 1024);//1024 * 1024 = framesize
            if (ZSTD_isError(initResult)) {
                fputs("ZSTD_seekable_initCStream err @ example 2 ", stderr);
                exit(1);
            }
            
            
            ZSTD_inBuffer input = { data_in, data_in_size, 0 }; // [start of input buffer, size, pos] uncompressed data?
            ZSTD_outBuffer output = { data_out, 255*255, 0 };
            
            size_t toRead = ZSTD_seekable_compressStream(s, &output, &input);
            if (ZSTD_isError(toRead)) {
                fputs("ZSTD_seekable_compressStream @ example 2 ", stderr);
                exit(1);
            }
            
            size_t const remainingToFlush = ZSTD_seekable_endStream(s, &output);
            if (remainingToFlush != 0) { die("can't end zstd stream\n"); }
            
            /* for(int i = 0; i < 48; i++) {
                printf("[%i] %i\n", i , (int) data_out[i]);
            } */
            
            data_out_size = output.pos;
            printf("example 2: toRead = %li , compressed data size: %li \n",  toRead,  data_out_size);
            
            
            // export data
            FILE *file = fopen("temp/example-02__zstd__empty_string.zst", "wb+");
            if(file == NULL) { die("can't create temporary file \n"); }
            fwrite(data_out, 1, output.pos, file);
            fclose(file);
            
            ZSTD_seekable_freeCStream(s);
            
            //free(data_in);
            //free(data_out);
            //free(s);
        }
        
        printf(" *** decompression ***\n");
        

        int out_buffer_size = 0;
        out_buffer_size = ZSTD_DStreamOutSize();
        static char *out_buffer = NULL;
        out_buffer = (char *) malloc_or_die(out_buffer_size);

        
        { // scope for decompressing
            static ZSTD_DStream *input_decompression_stream = NULL;
            static ZSTD_inBuffer zstd_file_in_buffer;


            input_decompression_stream = ZSTD_createDStream();
            size_t bytes_to_read = ZSTD_initDStream(input_decompression_stream);
            printf(" - bytes_to_read = %i\n", (int) bytes_to_read);
            
            ZSTD_TRY(ZSTD_DCtx_setParameter(input_decompression_stream, ZSTD_d_windowLogMax, ZSTD_WINDOWLOG_MAX));// what's this?
            
            ZSTD_inBuffer in = { data_out, data_out_size, 0 };
            ZSTD_outBuffer out = { out_buffer, 1024, 0 };
            
            bytes_to_read = ZSTD_decompressStream(input_decompression_stream, &out, &in);
            out_buffer_size = out.pos;
            out_buffer[out_buffer_size] = '\0';
        }
        
        printf(" - decompressed size: %i\n", out_buffer_size);
        printf(" - decompressed: [%s]\n", out_buffer);
        
        printf(" - checkpoint\n");

        //FREE(input_decompression_stream );
    }
    
    
    printf("\n\n");
    
    
    // [ ---- compress using ZSTD [classic] --- ]
    // example 3: w/ a string
    { // compression scope
        printf("example 3\n");
        
        char *data_in = (char *) malloc_or_die(255 * sizeof(char));
        data_in = "test\0";
        int data_in_size = 4;
        
        char *data_out = (char *) malloc_or_die(255 * 255 * sizeof(char));
        int data_out_size = 0;
        
        for(int i = 0 ; i < 64; i++){
            data_out[i] = '\0';
            assert(data_out[i] == '\0');
        }

        {
           ZSTD_CStream *s = ZSTD_createCStream();
            if (s == NULL) {
                fputs("wrong alloc @ example 3 ", stderr);
                exit(1);
            }
            
            size_t const initResult = ZSTD_initCStream(s, level);//1024 * 1024 = framesize
            if (ZSTD_isError(initResult)) {
                fputs("ZSTD_initCStream err @ example 3 ", stderr);
                exit(1);
            }
            
            ZSTD_inBuffer input = { data_in, data_in_size, 0 }; // [start of input buffer, size, pos] uncompressed data?
            ZSTD_outBuffer output = { data_out, 255*255, 0 };
            
            size_t toRead = ZSTD_compressStream(s, &output, &input);
            if (ZSTD_isError(toRead)) {
                fputs("ZSTD_compressStream @ example 3 ", stderr);
                exit(1);
            }
            
            size_t const remainingToFlush = ZSTD_endStream(s, &output);
            if (remainingToFlush != 0) { die("can't end zstd stream\n"); }
            
            data_out_size = output.pos;
            printf("example 3: toRead = %li , compressed data size? : %li \n",  toRead,  data_out_size);
            
            
            // export data
            FILE *file = fopen("temp/example-03__zstd__test_string.zst", "wb+");
            if(file == NULL) { die("can't create temporary file \n"); }
            fwrite(data_out, 1, output.pos, file);
            fclose(file);
            
            ZSTD_freeCStream(s);
            
            //free(data_in);
            //free(data_out);
            //free(s);
        }
        
        printf(" *** decompression ***\n");
        

        int out_buffer_size = 0;
        out_buffer_size = ZSTD_DStreamOutSize();
        static char *out_buffer = NULL;
        out_buffer = (char *) malloc_or_die(out_buffer_size);

        
        { // scope for decompressing
            static ZSTD_DStream *input_decompression_stream = NULL;
            static ZSTD_inBuffer zstd_file_in_buffer;


            input_decompression_stream = ZSTD_createDStream();
            size_t bytes_to_read = ZSTD_initDStream(input_decompression_stream);
            printf(" - bytes_to_read = %i\n", (int) bytes_to_read);
            printf(" - size compressed data: %i\n", (int) data_out_size);
            
            ZSTD_TRY(ZSTD_DCtx_setParameter(input_decompression_stream, ZSTD_d_windowLogMax, ZSTD_WINDOWLOG_MAX));// what's this?
            
            ZSTD_inBuffer in = { data_out, data_out_size, 0 };
            ZSTD_outBuffer out = { out_buffer, 1024, 0 };
            
            bytes_to_read = ZSTD_decompressStream(input_decompression_stream, &out, &in);
            out_buffer_size = out.pos;
            out_buffer[out_buffer_size] = '\0';
        }
        
        printf(" - decompressed size: %i\n", out_buffer_size);
        printf(" - decompressed: [%s]\n", out_buffer);
        
        printf(" - checkpoint\n");

        //FREE(input_decompression_stream );
    }
    
    printf("\n\n");
    
    // example 4: empty string
    {
        printf("example 4\n");


       ZSTD_CStream *s = ZSTD_createCStream();
        if (s == NULL) {
            fputs("wrong alloc @ example 4 ", stderr);
            exit(1);
        }
        
        size_t const initResult = ZSTD_initCStream(s, level);//1024 * 1024 = framesize
        if (ZSTD_isError(initResult)) {
            fputs("ZSTD_initCStream err @ example 4 ", stderr);
            exit(1);
        }
        
        char *data_in = (char *) malloc_or_die(255 * sizeof(char));
        data_in = "\0";
        char *data_out = (char *) malloc_or_die(255 * 255 * sizeof(char));
        for(int i = 0 ; i < 64; i++){
            data_out[i] = '\0';
            assert(data_out[i] == '\0');
        }
        
        ZSTD_inBuffer input = { data_in, 0, 0 }; // [start of input buffer, size, pos] uncompressed data?
        ZSTD_outBuffer output = { data_out, 255*255, 0 };
        
        size_t toRead = ZSTD_compressStream(s, &output, &input);
        if (ZSTD_isError(toRead)) {
            fputs("ZSTD_compressStream @ example 4 ", stderr);
            exit(1);
        }
        
        size_t const remainingToFlush = ZSTD_endStream(s, &output);
        if (remainingToFlush != 0) { die("can't end zstd stream\n"); }
        
        /* for(int i = 0; i < 48; i++) {
            printf("[%i] %i\n", i , (int) data_out[i]);
        } */
        
        printf("example 4: toRead = %li , compressed data size? : %li \n",  toRead,  output.pos);
        
        
        // export data
        FILE *file = fopen("temp/example-04__zstd__empty_string.zst", "wb+");
        if(file == NULL) { die("can't create temporary file \n"); }
        fwrite(data_out, 1, output.pos, file);
        fclose(file);
        
        ZSTD_freeCStream(s);
        
        //free(data_in);
        //free(data_out);
        //free(s);
        
        printf("\n\n");
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

