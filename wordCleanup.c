/*
 *     wordCleanup.c
 *     by Jia Wen Goh (jgoh01) and Rifat Ralfi Salhon (rsalho01), 09/12/2017
 *
 *     Cleans up the word by removing any non-word chars, and forming a 'clean 
 *     line' (e.g. "Lorem1@#@ipsum" will become "Lorem ipsum")
 */ 

#include <stdio.h>
#include <wordCleanup.h>
#include <stdlib.h>

int  is_word_char      (char c);
void check_end_of_line (size_t *bytes, char **datapp, 
                        size_t *index, size_t *cleaned_line_index);

/*
 * [Name]:              wordCleanup
 * [Purpose]:           Cleans up the word by removing any non-word chars,
 *                      and forming a 'clean line' (e.g. "Lorem1@#@ipsum" 
 *                      will become "Lorem ipsum")
 * [Parameters]:        1 size_t (# of bytes of the line), 1 char** 
 *                      (**datapp; lineContents)
 * [Returns]:           void
 */
void wordCleanup(size_t bytes, char **datapp)
{
        size_t  index = 0;
        size_t  cleaned_line_index = 0;
        
        while (index < bytes) {
                /* While character is a non-word char, move on to next index */
                while (index < bytes && !is_word_char((*datapp)[index]))
                        index++;

                /* While character is a word-char, add to cleanedLine */
                while (index < bytes && is_word_char((*datapp)[index])) {
                        (*datapp)[cleaned_line_index] = (*datapp)[index];
                        cleaned_line_index++;
                        index++;
                }

                /* Decide what to do next - read more or end the line */
                check_end_of_line(&bytes, datapp, &index, &cleaned_line_index);
        }

        return;
}

/*
 * [Name]:              is_word_char
 * [Purpose]:           Checks if the character is a word-character (i.e.
 *                      - any of lowercase 'a' – 'z'; uppercase 'A' – 'Z'; 
 *                      - the digit characters '0' – '9', 
 *                      - and the underscore character '_')
 * [Parameters]:        1 char (char to be checked)
 * [Returns]:           int (1 for success, 0 if not)
 */
int is_word_char(char c) 
{
        if ((c >= 48 && c <= 57) || (c >= 65 && c <=90) || 
            (c == 95) || (c >= 97 && c <= 122))
                return 1;

        else
                return 0;
}

/*
 * [Name]:              check_end_of_line
 * [Purpose]:           Checks if the iterator is at the end of the line; if
 *                      so, terminate the 'cleaned line', else put a ' '.
 * [Parameters]:        3 size_t* (1 for # bytes, 2 for iterators), 1 char**
 *                      (**datapp; lineContents)
 * [Returns]:           void
 */
void check_end_of_line(size_t *bytes, char **datapp, 
                       size_t *index, size_t *cleaned_line_index)
{
        if (*cleaned_line_index <= *index) {
                
                if ((*cleaned_line_index != 0) && 
                    (*datapp)[*cleaned_line_index - 1] == ' ')
                        (*datapp)[*cleaned_line_index - 1] = '\0';
                else if (*index == *bytes)
                        (*datapp)[*cleaned_line_index] = '\0';
                else {  
                        (*datapp)[*cleaned_line_index] = ' ';
                        (*cleaned_line_index)++;
                }
                        
        } else
                (*datapp)[*cleaned_line_index] = ' ';
}