#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define MAX_INPUT_LINE_SIZE 300

int is_file_exist(Dir* parent, char* name);

void touch (Dir* parent, char* name) {
	// pentru a crea un fisier am avut mai intai nevoie sa verific daca
	// exista deja numele fisierului, in caz contrat il adaug la sistemul
	// de fisiere deja existent
	if(is_file_exist(parent, name) != 1) {
		File* f = add_last_file(parent, name);
	} else {
		printf("File already exists\n");
	}
	return;
}

int is_dir_exist(Dir* parent, char* name);

void mkdir (Dir* parent, char* name) {
	// pentru a crea un director am avut mai intai nevoie sa verific daca
	// exista deja numele, in caz contrat il adaug la sistemul deja existent
	if(is_dir_exist(parent, name) != 1) {
		Dir* d = add_last_dir(parent, name);
	} else {
		printf("Directory already exists\n");
	}
	return;
}

void ls (Dir* parent) {
	for(Dir* iter = parent->head_children_dirs; iter != NULL; iter = iter->next) {
		printf("%s", iter->name);
	}
	for(File* iter = parent->head_children_files; iter != NULL; iter = iter->next) {
		printf("%s", iter->name);
	}
	return;
}

void rm (Dir* parent, char* name) {
	// cauta fisierul cu numele name si il sterge din lista
	if(is_file_exist(parent, name) == 1) {
		parent->head_children_files = delete_file(parent, name);
	} else {
		printf("Could not find the file\n");
	}	
}

// acesta functie f reface legaturie in sistemuul de fisiere si directorare
void f(Dir* for_rm);
Dir* pointer_to_dir(Dir* parent, char* name);

void rmdir (Dir* parent, char* name) {
	// verific daca exista directorul, il rup de sistemul de fisiere si directoare
	// dupa care il sterg recursiv
	if(is_dir_exist(parent, name) == 1) {
		Dir* for_rm = pointer_to_dir(parent, name);
		f(for_rm);
		delete_dirs(for_rm);
	} else {
		printf("Could not find the dir\n");
	}	
}

// spre deosebire de is_dir_exista, aceasta functie verifica daca
// exista directorul doar in dir_curent, nu in tot sistemul
int is_dir_exist_current(Dir* parent, char* name);
Dir* pointer_to_dir_current (Dir* parent, char* name);

void cd(Dir** target, char *name) {
	if(strstr(name, "..")) {
		if((*target)->parent == NULL) {
			return;
		} else {
			*target = (*target)->parent;
		} 
	} else {
		if(is_dir_exist_current(*target, name) == 1) {
			Dir* i = pointer_to_dir_current(*target, name);
			*target = i;
		} else { 
			printf("No directories found!\n");
		}
	}
}

// functia give primeste un cuvant si imi da cuvantul fara \n
char* give(char* cuv);
// add__v imi adauga un cuvant in vectorul v
char** add__v(char** v, char* cuv, int idx);

char *pwd (Dir* target) {
	char* res = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
	char* partial_res = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
	char** v = calloc(10, sizeof(char*));

	int idx = 0;
	//am parcurs invers, de la target la home, am pus name.urile in vector
	// dupa care le-am inversat 
	for(Dir* iter = target; iter != NULL; iter = iter->parent) {
		strcat(partial_res, "/");
		char* x = give(iter->name);
		v = add__v(v,x, idx++);
		strcat(partial_res, x);
	}
	for(int i = idx-1; i >= 0; i--) {
		strcat(res, "/");
		strcat(res, *(v+i));
	}

	free(partial_res);
	free(v);
	return res;
}

void stop (Dir* target) {
	return;
}

// get_space imi genereaza cate 4 spatii * level
void get_space(int nr);

void tree (Dir* target, int level) {
	if(target == NULL) 
		return;
	level++;
	// parcurg subdiectoarele_copii si le afisez recursiv
	for(Dir* iter = target->head_children_dirs; iter != NULL; iter = iter->next) {
		get_space(level-1);
		printf("%s", iter->name);
		tree(iter, level);
	}
	// dupa care, ma intreb daca are si fisiere
	if(target->head_children_files != NULL) {
		for(File* iter = target->head_children_files; iter != NULL; iter = iter->next) {
			get_space(level-1);
			printf("%s", iter->name);
		}
	}
	return;
}

// cu aceasta functie modific numele in cel nou, 
// adauga la final "noul director" si aplic
// functia f peste sistem pentru refacerea legaturilor
void change(Dir* parent, char* oldname, char *newname);

void mv(Dir* parent, char *oldname, char *newname) {
	// se verifica daca exista in sistem sau nu
	if(is_dir_exist_current(parent, oldname) == 1) {
		if(is_dir_exist_current(parent, newname) != 1 && is_file_exist(parent, newname) != 1) {
			change(parent, oldname, newname);
		} else {
			printf("File/Director already exists\n");
		}
	} else if (is_file_exist(parent, oldname) == 1) {
		if(is_file_exist(parent, newname) != 1 && is_dir_exist_current(parent, newname) != 1) {
			// in cazul fisierelor, ne putem folosi de functiile deja implementatte
			// deoarece nu este o stergere recursiva la un fisiere, deci
			// putem crea un fisier un numele nou si sa-l stergem pe cel vechi
			touch(parent, newname);
			rm(parent, oldname);
		} else {
			printf("File/Director already exists\n");
		}
	} else {
		printf("File/Director not found\n");
	}
}

char* aux_read(char* line);
// aux_read_mv_old este oractic aux_read, insa fara ultimul strtok
// deoarece aveam nevoie  la mv de 2 nume
char* aux_read_mv_old(char* line);
void free_ierarchy(Dir* parent);

int main () {
	char* line = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	char* line2 = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	char* line3 = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	char* name;

	Dir* home = create_home();
	Dir* parent = home;

	do
	{
		fgets(line, MAX_INPUT_LINE_SIZE, stdin);
		strcpy(line2, line);
		strcpy(line3, line);

		name = aux_read(line);
		if(strstr(line, "touch") != NULL) {
			touch(parent, name);
		} else if(strstr(line, "ls") != NULL) {
			ls(parent);
		} else if(strstr(line, "mkdir") != NULL) {
			mkdir(parent, name);
		} else if(strstr(line, "rmdir") != NULL) {
			rmdir(parent, name);
		} else if(strstr(line, "rm") != NULL) {
			rm(parent, name);
		} else if(strstr(line, "cd") != NULL) {
			cd(&parent, name);
		} else if(strstr(line, "tree") != NULL) {
			tree(parent, 0);
		} else if(strstr(line, "pwd") != NULL) {
			char* r = pwd(parent);
			printf("%s", r);
			free(r);
		} else if(strstr(line, "mv") != NULL) {
			char* n1= aux_read_mv_old(line3);
			mv(parent, n1, name);
			free(n1);
		} else if(strstr(line, "stop") != NULL) {
			stop(parent);
		}
		
		if(strstr(name, "home") == NULL) {
			free(name);
		}
	} while(strstr(line, "stop") == NULL);

	free(line);
	free_ierarchy(home);
	free(line2);
	free(line3);
	return 0;
}

char* aux_read(char* line) {
	char* p;
	char* file_name = malloc(MAX_INPUT_LINE_SIZE*sizeof(char));
	p = strtok(line, " ");
	while(p != NULL) {
		strcpy(file_name, p);
    	p = strtok (NULL, " ");
	}  
	return file_name;
}

char* aux_read_mv_old(char* line) {
	char* p;
	char* file_name = malloc(MAX_INPUT_LINE_SIZE*sizeof(char));
	p = strtok(line, " ");
	int idx = 0;
	// functia aceasta face acleasi lucru precu
	// sora ei aux_read, doar ca se opreste
	// inainte de ultimul nume de fisier/director
	while(p != NULL) {
		idx++;
		if(idx == 3)
			break;
		strcpy(file_name, p);
    	p = strtok (NULL, " ");
	}
	return file_name;
}

int is_file_exist(Dir* parent, char* name) {
	// 1-> exista fisierul in director curent
	// 0->nu exista fisierul in directorul curent
	File* i = parent->head_children_files;
	for( ; i != NULL; i = i->next) {
		if(strstr(i->name, name))
			return 1;
	}
	return 0;
}

int is_dir_exist(Dir* parent, char* name) {
	// 1-> exista directorul in sistem
	// 0->nu exista directorul in sistem
	if(parent == NULL)
		return 0;
	if(strstr(parent->name, name))
		return 1;

	Dir* i = parent->head_children_dirs;
	if(i == NULL)
		return 0;
	for( ; i != NULL; i = i->next) {
		int rez = is_dir_exist(i, name);
		if(rez) {
			return 1;
		}
	}
}

Dir* pointer_to_dir(Dir* parent, char* name) {
    if(parent == NULL) 
        return NULL;
    if(strstr(parent->name, name))
        return parent;
	Dir* iter = parent->head_children_dirs;
    if(iter == NULL)
        return NULL;
    
    for( ; iter != NULL; iter = iter->next) {
        Dir* rez = pointer_to_dir(iter, name);
		if(rez) {
			return rez;
		}
    }
    return NULL;

}

int is_dir_exist_current(Dir* parent, char* name) {
	// 1-> exista directorul in director curent
	// 0->nu exista directorul in directorul curent
	Dir* i = parent->head_children_dirs;
	for( ; i != NULL; i = i->next) {
		if(strstr(i->name, name))
			return 1;
	}
	return 0;
}

Dir* pointer_to_dir_current (Dir* parent, char* name) {
	// 1-> exista directorul in director curent
	// 0->nu exista directorul in directorul curent
	Dir* i = parent->head_children_dirs;
	for( ; i != NULL; i = i->next) {
		if(strstr(i->name, name)) {
			return i;
		}
	}
	return NULL;
}

void free_ierarchy(Dir* parent) {
	delete_dirs(parent);
}

void f(Dir* for_rm) {
	Dir* next = for_rm->next; 
	Dir* parent = for_rm->parent; 
	Dir* head_list = parent->head_children_dirs;
	if(head_list == NULL)
		return;
	Dir* prev = parent->head_children_dirs;
	if(head_list == for_rm) {
		parent->head_children_dirs = parent->head_children_dirs->next;
		if(next != NULL)
			next->parent = parent;
		for_rm->next = NULL;
	}
	for(Dir *i = head_list->next; i != NULL; i = i->next, prev = prev->next) {
		if(i == for_rm){
			prev->next = i->next;
		}
	}
	return;
}

char* give(char* cuv) {
	if(strstr(cuv, "\n")) {
		cuv[strlen(cuv)-1] = '\0';
	}
	return cuv;
}

char** add__v(char** v, char* cuv, int idx) {
	*(v+idx) = cuv;
	return v;
}

void get_space(int nr) {
	if(nr > 0) {
		nr--;
		get_space(nr);
		printf("    ");
	}
	return;
}

void change(Dir* parent, char* oldname, char *newname) {
	Dir * p = pointer_to_dir_current(parent, oldname);
	strcpy(p->name, newname);
	parent->head_children_dirs = add_last_dir_special(parent, p->name, p);
	f(p);
}