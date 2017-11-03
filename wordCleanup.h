/*
 *     wordCleanup.h
 *     by Jia Wen Goh (jgoh01) and Rifat Ralfi Salhon (rsalho01), 09/12/2017
 *
 *     Cleans up the word by removing any non-word chars, and forming a 'clean 
 *     line' (e.g. "Lorem1@#@ipsum" will become "Lorem ipsum")
 */ 

#ifndef WORDCLEANUP_H_INCLUDED
#define WORDCLEANUP_H_INCLUDED

#include <stdio.h>

/*
 *               wordCleanup
 *
 *   Parameters:
 * 
 *     bytes:     the number of bytes in the line to be cleaned
 *     datapp:    readaline-updated *datapp to points to 1st char of the most
 *                recently-read line; this line is then 'cleaned-up'
 *   Examples:
 *              - Lorem   ipsum   dolor
 *              - Lorem      ipsum!@#;’dolor 
 *              - Lorem@ipsum!@#;’dolor 
 *              - Lorem   ipsum!@#;’dolor
 *              will all be cleaned up to "Lorem ipsum dolor". Additionally,
 *              lines filled with non-word characters (or a blank line) will
 *              result in a blank line.
 *              (note: word character is defined as:
 *                      - any of lowercase 'a' – 'z'; uppercase 'A' – 'Z'; 
 *                      - the digit characters '0' – '9', 
 *                      - and the underscore character '_'),
 *              and any non-word characters are characters that don't fill the
 *              above criteria.
 */
void wordCleanup(size_t bytes, char **datapp);

#endif