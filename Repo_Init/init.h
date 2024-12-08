#ifndef INIT_H
#define INIT_H

int create_directory(const char *dir_path);
int create_file(const char *file_path, const char *content);
void generate_repo_id(char *repo_id);
void create_initial_commit();
void create_config_file();
int initialize_repository();

#endif
