#include "./../include/order.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <stdio.h>

struct order{
    char* client;
    unsigned int muffin;
};

order* order_create(char* client, unsigned int muffin){
    order* new_order = malloc(sizeof(order));
    if(new_order == NULL)
        return NULL;
    new_order->client = malloc(sizeof(char)*(strlen(client)+1));
    strncpy(new_order->client,client,strlen(client));
    new_order->client[strlen(client)]='\0';
    
    new_order->muffin = muffin;
    
    //printf("order %s \t %u erstellt\n", client, muffin);
    return new_order;
}

char* get_client(order* o){
    if(o == NULL )
        return NULL;
    return o->client;
}

unsigned int get_muffin(order* o){
    if(o == NULL) return 1;
    return o->muffin;
}

void order_destroy(order* o){
    if(o == NULL) return;
    free(o->client);
    free(o);
}



struct order_list{
    //TODO
    pthread_mutex_t count_m;
    pthread_cond_t order_came; 
    unsigned int count;
    unsigned int head;
    unsigned int tail;
    order** orders;
};

//returns the number of orders in the list
unsigned int get_count(order_list* ol){
    return ol->count;
}

//FUNCTIONS CALLED BY MANAGEMENT
order_list* order_list_create(){
    order_list* new_order_list = malloc(sizeof(order_list));    
    if(new_order_list == NULL)
        return NULL;
    
    new_order_list->orders = malloc(MAX_ORDERS * sizeof(order*));    
    if(new_order_list->orders == NULL)
        return NULL;
    //TODO
    /*------------------ Mutex und Condvar init ---------------------*/
    pthread_mutex_init(&new_order_list->count_m,NULL);
    pthread_cond_init(&new_order_list->order_came,NULL);
    /*---------------------------------------------------------------*/
    new_order_list->count = 0;
    new_order_list->head = 0;
    new_order_list->tail = 0;
    
    return  new_order_list;
    
}


//puts the order o in the order list ol
void deposit_order(order_list* ol, order* o){
    //TODO
    pthread_mutex_lock(&ol->count_m);
    ol->orders[ol->head] = o;
    ol->head = (ol->head+1)%MAX_ORDERS;
    ol->count++;
    pthread_cond_signal(&ol->order_came);
    pthread_mutex_unlock(&ol->count_m);
}   

//return the next order from order_list ol
order* get_order(order_list* ol){
    //TODO
    order* o;
    pthread_mutex_lock(&ol->count_m); 
    order_list* list = ol;
    while (get_count(list) == 0)
    {   
        pthread_cond_wait(&ol->order_came,&ol->count_m);
    }
    o = ol->orders[ol->tail];
    ol->tail = (ol->tail+1)%MAX_ORDERS;
    ol->count--;
    pthread_mutex_unlock(&ol->count_m);
    return o;
}

void order_list_destroy(order_list* ol){
    //TODO
    pthread_mutex_destroy(&ol->count_m);
    pthread_cond_destroy(&ol->order_came);
    free(ol->orders);
    free(ol);
}
