struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

Dir* create_home();
Dir* create_dir(Dir* parent, char* name);
File* create_file(Dir* parent, char* name);
File* add_last_file(Dir* parent, char* name);
Dir* add_last_dir(Dir* parent, char* name);
File* delete_file(Dir* parent, char *name);
void delete_dirs(Dir* head);
void delete_files(File* head);
void free_file(File* f);
void free_dir(Dir* d);
Dir* add_last_dir_special(Dir* parent, char* name, Dir* p);