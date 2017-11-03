/*
 *     simlines.c
 *     by Jia Wen Goh (jgoh01) and Rifat Ralfi Salhon (rsalho01), 09/07/2017
 *
 *     simlines reads any number of files and detect lines in the files 
 *     that are similar to each other.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <readaline.h>
#include <wordCleanup.h>
#include <list.h>
#include <atom.h>
#include <table.h>
#include <assert.h>

/* --- File Processing Functions --- */
void  process_all_files   (int num_files, char **datapp, 
                           char *argv[], Table_T *sim_table);
void  process_file        (FILE *fp, char **datapp, char *filename, 
                           Table_T *sim_table);

/* --- Information Storing Functions --- */
void  push_to_table       (Table_T *sim_table, char *line_location, 
                           const char *line_contents);

/* --- Printings Functions --- */
void  print_table         (Table_T *sim_table);
void  print_table_contents(const void *key, void **value, void *line_num);
void  print_list          (void **x, void *cl);

/* --- String Cleanup Functions --- */
char* get_location        (char *filename, size_t name_length, size_t line_num);

/*
 * [Name]:              main
 * [Purpose]:           main call; creates a Table that will contain all lines
 *                      and their locations, and note similarities. Will read 
 *                      data from given files
 * [Parameters]:        1 int (argc), 1 char* (argv[])
 * [Returns]:           int
 */
int main(int argc, char *argv[])
{
        int num_files = argc - 1;

        char **datapp = malloc(sizeof(datapp));
        assert(datapp != NULL);

        Table_T sim_table = Table_new(0, NULL, NULL);

        process_all_files(num_files, datapp, argv, &sim_table);

        Table_free(&sim_table);
        free(datapp);

        return 0;
}

/*----------------------------------------------------------------*
 |                  Processing Functions                          |
 *----------------------------------------------------------------*/
/*
 * [Name]:              process_all_files
 * [Purpose]:           Given the files in argv, this function will process 
 *                      them (i.e. store the line_contents & line_locations\
 *                      in the given Table);
 *                      calls process_file o process individual files
 * [Parameters]:        1 int (number of Files), 1 char** (datapp, 
 *                      line_contents) 1 char* (argv[], filename), 1 Table_T
 *                      (Table to store information)
 * [Returns]:           void
 */
void process_all_files(int num_files, char **datapp, 
                       char *argv[], Table_T *sim_table) 
{
        for (int i = 0; i < num_files; i++) {
                FILE *fp;
                fp = fopen(argv[i + 1], "r");
                assert(fp != NULL);

                process_file(fp, datapp, argv[i + 1], sim_table);

                fclose(fp);
        }
        print_table(sim_table);
}

/*
 * [Name]:              process_file
 * [Purpose]:           Process individual files by:
 *                      1. Gets line_location and stores it in a char*
 *                      2. Gets line_contents (from **datapp) and stores it in 
 *                         an Atom
 *                      3. Pushes line_location and line_contents into the Table
 *                      then frees heap-allocated memory that is created from
 *                      processing individual files.
 * [Parameters]:        1 int (number of Files), 1 char** (datapp, 
 *                      line_contents), 1 char* (specific filename), 
 *                      1 Table_T (Table to store information)
 * [Returns]:           void
 */
void process_file(FILE *fp, char **datapp, char *filename, Table_T *sim_table) 
{
        size_t line_num = 0;
        size_t fname_length = strlen(filename);
        size_t bytes = 0;

        while(!feof(fp)) {
                line_num++;

                /* FIRST: Store filename & line_num in string */
                char *line_location = NULL; 
                line_location = get_location(filename, fname_length, line_num);

                /* SECOND: Store line contents in string */
                bytes = readaline(fp, datapp);
                wordCleanup(bytes, datapp);

                /* THIRD: Store line_contents & line_location in Table */
                if (bytes > 0) {
                        const char* line_contents = Atom_string(*datapp);

                        if (line_contents[0] != '\0')
                                push_to_table(sim_table, line_location, 
                                              line_contents);
                        else 
                                free(line_location);
                } else 
                        free(line_location);

                free(*datapp);
        }
}

/*----------------------------------------------------------------*
 |                  Info Storing Functions                        |
 *----------------------------------------------------------------*/
/*
 * [Name]:              push_to_table
 * [Purpose]:           Pushes the given char* and Atom into the Table; tracks 
 *                      similar lines by checking for existing value given the
 *                      key (the line_contents); if there's a similar line,
 *                      append line_location behind existing line information.
 * [Parameters]:        1 Table_T (Table to store info), 1 char* (line_location)
 *                      1 Atom (line_contents)
 * [Returns]:           void
 */
void push_to_table(Table_T *sim_table, char *line_location, 
                 const char *line_contents)
{
        List_T *buff = NULL;
        buff = Table_get(*sim_table, line_contents);

        if (buff == NULL) {
                List_T newList;
                newList = List_list(line_location, NULL);
                Table_put(*sim_table, (const void *)line_contents, newList); 
        } else {
                List_T newList;
                newList = List_list(line_location, NULL);
                *buff = List_append(*buff, newList);
                Table_put(*sim_table, (const void *)line_contents, 
                          (void *)buff);
        }    
}

/*----------------------------------------------------------------*
 |                  Printing Functions                            |
 *----------------------------------------------------------------*/
/* ------ TABLE PRINTING ------*/
/*
 * [Name]:              print_table
 * [Purpose]:           Prints the contents of the entire Table; calls on 
 *                      Table_map to print content of each 'cell'.
 * [Parameters]:        1 Table_T* (Table to print)
 * [Returns]:           void
 */
void print_table(Table_T *sim_table)
{
        int *line_num = malloc(8);
        assert(line_num != NULL);
        *line_num = 0;   

        Table_map(*sim_table, print_table_contents, line_num);

        free(line_num);
}

/*
 * [Name]:              print_table_contents
 * [Purpose]:           Prints the contents in each 'cell' of the Table
 *                      (i.e. any similar lines with their corresponding
 *                      locations); called by Table_map in print_table.
 * [Parameters]:        1 Atom (key/line_contents), 1 void** (line_location),
 *                      1 void* (checks if the line printed is the first line;
 *                      used for formatting)
 * [Returns]:           void
 */
void print_table_contents(const void *key, void **value, void *line_num)
  {
        List_T lines = (List_T) *value;
        if (List_length(lines) > 1) {
                if (*((int *)line_num) != 0)
                        printf("\n");

                *((int *)line_num) += 1;

                const char *line_contents = (const char *) key;
                printf("%s\n", line_contents);

                List_map(lines, print_list, NULL);
        }

        while (List_length(lines) > 0) {
                char *top = NULL;
                lines = List_pop(lines, (void *) &top);
                free(top);
        }
}

/* ------ LIST PRINTING ------*/
/*
 * [Name]:              print_list 
 * [Purpose]:           Prints the values in the list (i.e. line_ locations); 
 *                      called by List_map in print_table_contents
 * [Parameters]:        1 void ** (value), 1 void* (cl; not used  )
 * [Returns]:           void
 */
void print_list(void **value, void *cl) 
{
        (void) cl;
        char **str = (char **)value;
        if(*str != NULL)
                printf("%s\n", *str);
}

/*----------------------------------------------------------------*
 |                  String Cleanup Functions                      |
 *----------------------------------------------------------------*/
/*
 * [Name]:              get_location 
 * [Purpose]:           Formats the line_location in the given format:
 *                      if filename_length < 20,
 *                      - |<filename>padded to 20chars|| ||right-justified int| 
 *                      if longer than 20, then remove the padding, i.e.:
 *                      - |<filename>|| ||right-justified int|
 * [Parameters]:        1 char* (filename), 2 size_t (name_length & line_num)
 * [Returns]:           char* (formatted line)
 */
char* get_location(char *filename, size_t name_length, size_t line_num)
{
        char *file_location = NULL;

        if (name_length < 20) {
                file_location = calloc(29, 1);
                assert(file_location != NULL);
                sprintf(file_location, "%-20s %7zu", filename, line_num);
        } else {
                file_location = calloc(name_length + 9, 1);
                assert(file_location != NULL);
                sprintf(file_location, "%-20s %7zu", filename, line_num);
        }
        return file_location;
}