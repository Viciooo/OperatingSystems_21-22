#ifndef LIB_H
#define LIB_H

void create_table(int size);
void wc_files(char *files);
void delete_files();
void remove_report();
void file_to_array(char *file_name);
void remove_block(int idx);
void free_array();
void get_times(char *name);
void load_file(char **buffer, char *file_path);
#endif // LIB_H:
