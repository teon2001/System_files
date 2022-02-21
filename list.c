#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "list.h"

Dir* create_home() {
    Dir* home = malloc(sizeof(Dir));
    char* h = malloc(6 * sizeof(char));
    strcpy(h, "home");
    home->name = h;
    home->parent = NULL;
    home->head_children_files = NULL;
    home->head_children_dirs = NULL;
    home->next = NULL;
    return home;
}

Dir* create_dir(Dir* parent, char* name) {
    Dir* node = malloc(sizeof(Dir));
    node->name = strdup(name);
    node->parent = parent;
    node->head_children_files = NULL;
    node->head_children_dirs = NULL;
    node->next = NULL;
    return node;
}

File* create_file(Dir* parent, char* name) {
    File* node = malloc(sizeof(File));
    node->name = strdup(name);
    node->parent = parent;
    node->next = NULL;
    return node;
}

File* add_last_file(Dir* parent, char* name) {
    if (parent->head_children_files == NULL) {
        File* node = create_file(parent, name);
        node->parent->head_children_files = node;
        return node;
    }
    File* temp = parent->head_children_files;
    while (temp->next != NULL)
        temp = temp->next;
    
    File* n = create_file(parent, name);
    temp->next = n;
    return parent->head_children_files;
}

Dir* add_last_dir(Dir* parent, char* name) {
    if(parent == NULL)
        return NULL; 
    if (parent->head_children_dirs == NULL) {
        Dir* node = create_dir(parent, name);
        node->parent->head_children_dirs = node;
        return node;
    }
    Dir* temp = parent->head_children_dirs;
    while (temp->next != NULL)
        temp = temp->next;
    
    Dir* node = create_dir(parent, name);
    temp->next = node;
    node->parent = parent;
    return parent->head_children_dirs;
}

File* delete_file(Dir* parent, char *name) {
    if (strstr(parent->head_children_files->name, name)) {
        File* for_rm = parent->head_children_files;
        parent->head_children_files = parent->head_children_files->next;
        free_file(for_rm); 
        return parent->head_children_files;
    }

    File* prev = parent->head_children_files;
    File* element =  parent->head_children_files->next;
    
    for( ; element != NULL; element = element->next, prev = prev->next) {
        if(strstr(element->name, name)) {
            File* for_rm = element;
            prev->next = element->next;
            free_file(for_rm);
            return parent->head_children_files;
        }
    }
    return NULL;
}

void delete_dirs(Dir* head) {
    if(head == NULL) 
        return;
    if(head->head_children_dirs != NULL) {
        Dir* iter = head->head_children_dirs;
        Dir* p = iter->parent;
        for(; iter != NULL; ) {
            Dir* n = iter->next;
            delete_dirs(iter);
            iter = n;
        }
        p->head_children_dirs = NULL; 
    }
    if(head->head_children_files != NULL) {
        delete_files(head->head_children_files);
        head->head_children_files = NULL;
    }
    free_dir(head);
    return;
}

void delete_files(File* head) {
    if(head == NULL)
        return;
    delete_files(head->next);
    free_file(head);
    return;
}

void free_file(File* f) {
    free(f->name);
    free(f);
}

void free_dir(Dir* d) {
    free(d->name);
    free(d);
}

Dir* add_last_dir_special(Dir* parent, char* name, Dir* p) {
    // aceasta functie adauga la final un director, doar
    // ca primeste un nou parametru si este folosita pentru mv
    if(parent == NULL)
        return NULL; 
    if (parent->head_children_dirs == NULL) {
        Dir* node = create_dir(parent, name);
        node->parent->head_children_dirs = node;
        return node;
    }
    Dir* temp = parent->head_children_dirs;
    while (temp->next != NULL)
        temp = temp->next;

    Dir* node;
    if(p == NULL)
        node = create_dir(parent, name);
    else
        node = p;
    temp->next = node;
    node->parent = parent;
    return parent->head_children_dirs;
}
