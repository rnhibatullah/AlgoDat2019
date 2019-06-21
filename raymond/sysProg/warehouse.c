#include "./../include/warehouse.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct warehouse{
    unsigned int flour, sugar, choclate;
    int locked;
    
    unsigned int sugar_taken, flour_taken, choclate_taken;
    //TODO
    pthread_mutex_t anzahl_flour;
    pthread_mutex_t anzahl_sugar;
    pthread_mutex_t anzahl_chocolate;
    pthread_mutex_t anzahl_taken_flour;
    pthread_mutex_t anzahl_taken_sugar;
    pthread_mutex_t anzahl_taken_chocolate;
    pthread_mutex_t lock_warehouse;
    pthread_cond_t sig_dep_choc;
    pthread_cond_t sig_get_choc;
    pthread_cond_t sig_dep_flour;
    pthread_cond_t sig_get_flour;
    pthread_cond_t sig_dep_sugar;
    pthread_cond_t sig_get_sugar;
};

warehouse* warehouse_create(){
    warehouse* new_warehouse = (warehouse*) malloc(sizeof(warehouse));
    if(new_warehouse== NULL){
        printf("Unable to allocate memory in function %s()\n",__func__);
        return NULL;
    }
    new_warehouse->flour = 0;
    new_warehouse->sugar = 0;
    new_warehouse->choclate = 0;
    new_warehouse->locked = 0;
    
    //TODO
    
    new_warehouse->choclate_taken = 0;
    new_warehouse->flour_taken = 0;
    new_warehouse->sugar_taken = 0;

    pthread_mutex_init(&new_warehouse->anzahl_flour, NULL);
    pthread_mutex_init(&new_warehouse->anzahl_sugar, NULL);
    pthread_mutex_init(&new_warehouse->anzahl_chocolate, NULL);
    pthread_mutex_init(&new_warehouse->anzahl_taken_flour, NULL);
    pthread_mutex_init(&new_warehouse->anzahl_taken_sugar, NULL);
    pthread_mutex_init(&new_warehouse->anzahl_taken_chocolate, NULL);
    pthread_mutex_init(&new_warehouse->lock_warehouse, NULL);
    pthread_cond_init(&new_warehouse->sig_dep_choc, NULL);
    pthread_cond_init(&new_warehouse->sig_get_choc, NULL);
    pthread_cond_init(&new_warehouse->sig_dep_flour, NULL);
    pthread_cond_init(&new_warehouse->sig_get_flour, NULL);
    pthread_cond_init(&new_warehouse->sig_dep_sugar, NULL);
    pthread_cond_init(&new_warehouse->sig_get_sugar, NULL);
    return new_warehouse;
}

//FUNCTIONS CALLED BY WORKER THREADS
//@param: amount of flour/sugar/choclate to be taken from warehouse wh

unsigned int get_choclate(unsigned int amount, warehouse* wh){
    pthread_mutex_lock(&wh->anzahl_chocolate);
    while(wh->choclate == 0 && wh->locked){
        pthread_mutex_unlock(&wh->anzahl_chocolate);
        pthread_cond_wait(&wh->sig_get_choc, &wh->anzahl_chocolate);
        pthread_mutex_lock(&wh->anzahl_chocolate);
    }
    pthread_mutex_lock(&wh->anzahl_taken_chocolate);
    wh->choclate_taken += amount;
    pthread_mutex_unlock(&wh->anzahl_taken_chocolate);
    wh->choclate -= amount;
    pthread_cond_signal(&wh->sig_dep_choc);
    pthread_mutex_unlock(&wh->anzahl_chocolate);
    
    
    return amount;
}

unsigned int get_flour(unsigned int amount, warehouse* wh){
    
    //TODO
    pthread_mutex_lock(&wh->anzahl_flour);
    while(wh->flour == 0 && wh->locked){
        pthread_mutex_unlock(&wh->anzahl_flour);
        pthread_cond_wait(&wh->sig_get_flour, &wh->anzahl_flour);
        pthread_mutex_lock(&wh->anzahl_flour);
    }
    pthread_mutex_lock(&wh->anzahl_taken_flour);
    wh->flour_taken += amount;
    pthread_mutex_unlock(&wh->anzahl_taken_flour);
    wh->flour -= amount;
    pthread_cond_signal(&wh->sig_dep_flour);
    pthread_mutex_unlock(&wh->anzahl_flour);
    
    return amount;
}

unsigned int get_sugar(unsigned int amount, warehouse* wh){
    
    //TODO
    pthread_mutex_lock(&wh->anzahl_sugar);
    while(wh->sugar == 0 && wh->locked){
        pthread_mutex_unlock(&wh->anzahl_sugar);
        pthread_cond_wait(&wh->sig_get_sugar, &wh->anzahl_sugar);
        pthread_mutex_lock(&wh->anzahl_sugar);
    }
    pthread_mutex_lock(&wh->anzahl_taken_sugar);
    wh->sugar_taken += amount;
    pthread_mutex_unlock(&wh->anzahl_taken_sugar);
    wh->sugar -= amount;
    pthread_cond_signal(&wh->sig_dep_sugar);
    pthread_mutex_unlock(&wh->anzahl_sugar);
        
    return amount;
}


//FUNCTIONS CALLED BY SUPPLIER THREADS
//@param: amount of flour/sugar/choclate to be store in warehouse wh
void deposit_choclate(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->anzahl_chocolate);
    while(wh->choclate + amount > MAX_STORE_CAPACITY && wh->locked){
        pthread_cond_wait(&wh->sig_dep_choc, &wh->anzahl_chocolate);
    }
    if(wh->choclate + amount > MAX_STORE_CAPACITY){
        wh->choclate = MAX_STORE_CAPACITY;
    }else{
        wh->choclate += amount;
    }
    pthread_cond_signal(&wh->sig_get_choc);
    pthread_mutex_unlock(&wh->anzahl_chocolate);
    
    return;
}

void deposit_sugar(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->anzahl_sugar);
    while(wh->sugar + amount > MAX_STORE_CAPACITY && wh->locked){
        pthread_cond_wait(&wh->sig_dep_sugar, &wh->anzahl_sugar);
    }
    if(wh->sugar + amount > MAX_STORE_CAPACITY){
        wh->sugar = MAX_STORE_CAPACITY;
    }else{
        wh->sugar += amount;
    }
    pthread_cond_signal(&wh->sig_get_sugar);
    pthread_mutex_unlock(&wh->anzahl_sugar);
    return;
}

void deposit_flour(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->anzahl_flour);
    while(wh->flour + amount > MAX_STORE_CAPACITY && wh->locked){
        pthread_cond_wait(&wh->sig_dep_flour, &wh->anzahl_flour);
    }
    if(wh->flour + amount > MAX_STORE_CAPACITY){
        wh->flour = MAX_STORE_CAPACITY;
    }else{
        wh->flour += amount;
    }
    pthread_cond_signal(&wh->sig_get_flour);
    pthread_mutex_unlock(&wh->anzahl_flour);
    return;
}


//FUNCTIONS CALLED BY BAKERY
void lock_warehouse(warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->lock_warehouse);
    wh->locked=1;
    //pthread_cond_signal(&wh->sig_dep_choc);
    //pthread_cond_signal(&wh->sig_get_choc);
    pthread_mutex_unlock(&wh->lock_warehouse);
}

unsigned int get_used_resources(warehouse* wh, int type){
    if(type == SUGAR) return wh->sugar_taken;
    if(type == FLOUR) return wh->flour_taken;
    if(type == CHOCLATE) return wh->choclate_taken;
    return 0;
}


void warehouse_destroy(warehouse* wh){
    //TODO
    pthread_mutex_destroy(&wh->anzahl_sugar);
    pthread_mutex_destroy(&wh->anzahl_chocolate);
    pthread_mutex_destroy(&wh->anzahl_flour);
    pthread_mutex_destroy(&wh->anzahl_taken_sugar);
    pthread_mutex_destroy(&wh->anzahl_taken_chocolate);
    pthread_mutex_destroy(&wh->anzahl_taken_flour);
    pthread_mutex_destroy(&wh->lock_warehouse);
    pthread_cond_destroy(&wh->sig_dep_choc);
    pthread_cond_destroy(&wh->sig_dep_flour);
    pthread_cond_destroy(&wh->sig_dep_sugar);
    pthread_cond_destroy(&wh->sig_get_choc);
    pthread_cond_destroy(&wh->sig_get_flour);
    pthread_cond_destroy(&wh->sig_get_sugar);

    free(wh);
    return;
}
