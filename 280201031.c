#include <stdio.h>
#include <stdlib.h>

typedef struct linked_list_node_t {
    void* data;
    struct linked_list_node_t* next;
} linked_list_node_t;

void create_array_of_linked_list_ptrs(linked_list_node_t*** destination, int size) {
    *destination = (linked_list_node_t**) malloc(size * sizeof(linked_list_node_t*));
    // when the program initializes an array in create_array_of_linked_list_ptrs, assigning NULL to each element.
    for(int i = 0; i < size; i++){
        (*destination)[i] = NULL;
    }
}

void get_element_of_array_of_linked_list_ptrs(linked_list_node_t** array, int index, linked_list_node_t** destination){
    *destination = array[index];
}

void set_element_of_array_of_linked_list_ptrs(linked_list_node_t** array, int index, linked_list_node_t* head) {
    array[index] = head;
}

void destroy_array_of_linked_list_ptrs(linked_list_node_t** array) {
    free(array);
}

void resize_array_of_linked_list_ptrs(linked_list_node_t*** destination, linked_list_node_t** array, int size, int new_size){
    *destination = (linked_list_node_t**) realloc(array, new_size * sizeof(linked_list_node_t*));
    // when the program expands the array size using resize_array_of_linked_list_ptrs, assigning NULL to each newly created element.
    for(int i = size; i < new_size; i++){
        (*destination)[i] = NULL;
    }
}

void create_link_list(linked_list_node_t** destination){
    *destination = (linked_list_node_t*) malloc(sizeof(linked_list_node_t));
    // when the program initializes a linked list in create_link_list, assigning NULL to the fields	
    (*(*destination)).data = NULL;
    (*(*destination)).next = NULL;
}

void destroy_linked_list(linked_list_node_t* head) {
    linked_list_node_t* temporary_node;
    while (head != NULL){
       temporary_node = head;
       head = (*head).next;
       free(temporary_node);
    }
}

void add_to_linked_list(linked_list_node_t* head, const void* data) {
    if((*head).data == NULL){
        (*head).data = (void*)data;
    } else {
        linked_list_node_t* new_node = (linked_list_node_t*) malloc(sizeof(linked_list_node_t));
        (*new_node).data = (void*)data;
        (*new_node).next = NULL;

        linked_list_node_t* current = head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_node;
    }
}

void remove_from_linked_list(linked_list_node_t** destination, linked_list_node_t* head, const void* data) {
    linked_list_node_t* temp = head;
    linked_list_node_t* prev = NULL;

    if (temp != NULL && (*temp).data == data){
        *destination = (*temp).next;   
        free(temp);               
        return;
    }
    while (temp != NULL && (*temp).data != data){
        prev = temp;
        temp = (*temp).next;
    }
    if (temp == NULL){
        return;
    } 
    (*prev).next = (*temp).next;
    free(temp);
}

typedef struct song_t {
    const char* name;
    float duration;
} song_t;

void create_song(song_t* destination, const char* name, float duration) {
    (*destination).name = name;     //destination->name = name;
    (*destination).duration = duration;     //destination->duration = duration;
}

linked_list_node_t** array_of_playlist_ptrs = NULL;

int main(void){
    // initializing array_of_playlist_ptrs by using create_array_of_linked_list_ptrs with an initial size of 5.
    create_array_of_linked_list_ptrs(&array_of_playlist_ptrs, 5);

    // creating 5 playlists by using create_link_list and 
    // inserting them into array_of_playlist_ptrs by using set_element_of_array_of_linked_list_ptrs. 
    for(int i = 0; i < 5; i++){
        linked_list_node_t* new_playlist;
        create_link_list(&new_playlist);
        set_element_of_array_of_linked_list_ptrs(array_of_playlist_ptrs, i, new_playlist);
    }

    // resizing array_of_playlist_ptrs to a size of 10 by using resize_array_of_linked_list_ptrs and 
    // creating five more playlists, and inserting them into available areas in array_of_playlist_ptrs
    resize_array_of_linked_list_ptrs(&array_of_playlist_ptrs, array_of_playlist_ptrs, 5, 10);
    for(int i = 5; i < 10; i++){
        linked_list_node_t* new_playlist;
        create_link_list(&new_playlist);
        set_element_of_array_of_linked_list_ptrs(array_of_playlist_ptrs, i, new_playlist);
    }

    // creating four songs for each playlist by using create_song and 
    // inserting them into playlists by using add_to_linked_list
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 4; j++){
            song_t* new_song = (song_t*) malloc(sizeof(song_t));
            char* song_name = (char*) malloc(10 * sizeof(char));
            sprintf(song_name, "Song #%d", j+1); 
            float song_duration = (float)(rand() % 10000000) / 1000000.0; 
            create_song(new_song, song_name, song_duration);
            add_to_linked_list(array_of_playlist_ptrs[i], new_song);
        }
    }

    // removing the second song of each playlist by using remove_from_linked_list and 
    // destroying them :
        // In the part where we remove the second song of each playlist, we free the node with the line "free(temp);" in the remove_from_linked_list function. 
        // If we try to free this node again in the main function with "(free(third_song_node);)" 
        // This causes a double release error. And we get this error:
		// free(): double free detected in cache 2
		// Aborted (core dumped)
    for(int i = 0; i < 10; i++){
        linked_list_node_t* second_song_node = array_of_playlist_ptrs[i]->next;
        remove_from_linked_list(&array_of_playlist_ptrs[i], array_of_playlist_ptrs[i], (*second_song_node).data);
    }

    // printing all playlists and their contents by using get_element_of_array_of_linked_list_ptrs.
    for(int i = 0; i < 10; i++){
        printf("Playlist #%d\n", i);
        linked_list_node_t* current_node = array_of_playlist_ptrs[i];
        while(current_node != NULL){
            if((*current_node).data != NULL){
                song_t* current_song = (song_t*)(*current_node).data;
                printf("\tName: %s, Duration: %.6f\n", (*current_song).name, (*current_song).duration);
            }
            current_node = (*current_node).next;
        }
    }

    // destroying each playlist by using destroy_linked_list.
    for(int i = 0; i < 10; i++){
        destroy_linked_list(array_of_playlist_ptrs[i]);
    }

    // destroying array_of_playlist_ptrs by using destroy_array_of_linked_list_ptrs.
    destroy_array_of_linked_list_ptrs(array_of_playlist_ptrs);
    return 0;
}