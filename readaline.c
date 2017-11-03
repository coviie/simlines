/*
 *     readaline.c
 *     by Jia Wen Goh (jgoh01) and Rifat Ralfi Salhon (rsalho01), 09/07/2017
 *
 *     Readaline reads a single line of input from file inputfd. Each 
 *     invocation of readaline retrieves the next unread line in the file. 
 *     The characters comprising the line are placed into a contiguous array 
 *     of bytes, and *datapp is set to the address of of the first byte. 
 *     readaline returns the number of bytes in the line.
 */ 

#include <stdio.h>
#include <readaline.h>
#include <stdlib.h>
#include <assert.h>

size_t get_bytes         (FILE *inputfd);
void   store_contents    (FILE *inputfd, size_t bytes, char **datapp);

/*
 * [Name]:              readaline
 * [Purpose]:           Read a single line of input from file inputfd, and 
                        returns the number of bytes in the line.
 * [Parameters]:        1 FILE* (file to be read), 1 char** (**datapp, points
                        to the address of the first byte of the line)
 * [Returns]:           size_t (# of bytes)
 */
size_t readaline(FILE *inputfd, char **datapp)
{
        assert(inputfd != NULL && datapp != NULL);

        size_t bytes = get_bytes(inputfd);
        store_contents(inputfd, bytes, datapp);

        return bytes;
}

/*
 * [Name]:              get_bytes
 * [Purpose]:           Determines the number of bytes in the line of the given
                        file.
 * [Parameters]:        1 FILE* (file to be read)
 * [Returns]:           size_t (# of bytes)
 */
size_t get_bytes(FILE *inputfd)
{
        fpos_t  startPos;
        fgetpos(inputfd, &startPos);

        size_t  bytes = 0;
        char    c;
        while ('\n' != (c = fgetc(inputfd)) && !feof(inputfd)) {
                assert(c >= 0);
                bytes++;
        }

        fsetpos(inputfd, &startPos);
        return bytes;
}

/*
 * [Name]:              store_contents
 * [Purpose]:           read a single line of input from file inputfd, and 
                        returns the number of bytes in the line.
 * [Parameters]:        1 FILE* (file to be read), 1 size_t (# of bytes in the
                        line), 1 char** (**datapp, points to the address of the 
                        first byte of the line)
 * [Returns]:           void
 */
void store_contents(FILE *inputfd, size_t bytes, char **datapp) 
{
        char *lineContents = malloc(bytes + 1);
        assert(lineContents != NULL);

        for (size_t i = 0; i < bytes; i++)
                lineContents[i] = fgetc(inputfd);

        if (!feof(inputfd))
                fgetc(inputfd);

        *datapp = lineContents;
}