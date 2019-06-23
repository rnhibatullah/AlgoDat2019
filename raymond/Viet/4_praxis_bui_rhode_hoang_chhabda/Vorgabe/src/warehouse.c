#include "./../include/warehouse.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "./../include/worker.h"

struct warehouse{
    unsigned int flour, sugar, choclate;
    int locked;
    
    unsigned int sugar_taken, flour_taken, choclate_taken;
    //TODO
    pthread_mutex_t flour_m;
    pthread_mutex_t sugar_m;
    pthread_mutex_t chocolate_m;
    /*There are enough Ingredients for at least one Muffin  */
    pthread_cond_t enough_flour;
    pthread_cond_t enough_sugar;
    pthread_cond_t enough_chocolate;
    /*Supplier wait when Warehouse is full*/
    pthread_cond_t flour_is_taken;
    pthread_cond_t sugar_is_taken;
    pthread_cond_t chocolate_is_taken;

    pthread_mutex_t lock_m;
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
    /*------------------ Mutex und Condvar init ---------------------*/
    pthread_mutex_init(&new_warehouse->flour_m,NULL);
    pthread_mutex_init(&new_warehouse->sugar_m,NULL);
    pthread_mutex_init(&new_warehouse->chocolate_m,NULL);

    pthread_cond_init(&new_warehouse->enough_flour,NULL);
    pthread_cond_init(&new_warehouse->enough_sugar,NULL);
    pthread_cond_init(&new_warehouse->enough_chocolate,NULL);

    pthread_cond_init(&new_warehouse->flour_is_taken,NULL);
    pthread_cond_init(&new_warehouse->sugar_is_taken,NULL);
    pthread_cond_init(&new_warehouse->chocolate_is_taken,NULL);

    pthread_mutex_init(&new_warehouse->lock_m,NULL);

    /*---------------------------------------------------------------*/
    new_warehouse->choclate_taken = 0;
    new_warehouse->flour_taken = 0;
    new_warehouse->sugar_taken = 0;
    return new_warehouse;
}

//FUNCTIONS CALLED BY WORKER THREADS
//@param: amount of flour/sugar/choclate to be taken from warehouse wh

unsigned int get_choclate(unsigned int amount, warehouse* wh){
    
    //TODO
    pthread_mutex_lock(&wh->chocolate_m);
    while(wh->choclate < CHOCLATE_FOR_ONE_MUFFIN) {
        pthread_cond_wait(&wh->enough_chocolate,&wh->chocolate_m);
    }
    wh->choclate_taken += amount;
    wh->choclate -= amount;
    pthread_cond_signal(&wh->chocolate_is_taken);
    pthread_mutex_unlock(&wh->chocolate_m);
    return amount;
}

unsigned int get_flour(unsigned int amount, warehouse* wh){
    
    //TODO
    pthread_mutex_lock(&wh->flour_m);
    while(wh->flour < FLOUR_FOR_ONE_MUFFIN) {
        pthread_cond_wait(&wh->enough_flour,&wh->flour_m);
    } 
    wh->flour_taken += amount;
    wh->flour -= amount;
    pthread_cond_signal(&wh->flour_is_taken);
    pthread_mutex_unlock(&wh->flour_m);
    return amount;
}

unsigned int get_sugar(unsigned int amount, warehouse* wh){
    
    //TODO
    pthread_mutex_lock(&wh->sugar_m);
    while(wh->sugar < SUGAR_FOR_ONE_MUFFIN) {
        pthread_cond_wait(&wh->enough_sugar,&wh->sugar_m);
    }
    wh->sugar_taken += amount;
    wh->sugar -= amount;
    pthread_cond_signal(&wh->sugar_is_taken);
    pthread_mutex_unlock(&wh->sugar_m);
    return amount;
}


//FUNCTIONS CALLED BY SUPPLIER THREADS
//@param: amount of flour/sugar/choclate to be store in warehouse wh
void deposit_choclate(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->chocolate_m);
    if(wh->choclate + amount > MAX_STORE_CAPACITY){
        wh->choclate = MAX_STORE_CAPACITY;
        pthread_mutex_lock(&wh->lock_m);
        while (wh->choclate == MAX_STORE_CAPACITY && wh->locked == 0) {
            pthread_mutex_unlock(&wh->lock_m);
            pthread_cond_wait(&wh->chocolate_is_taken,&wh->chocolate_m);
            pthread_mutex_lock(&wh->lock_m);
        }
        pthread_mutex_unlock(&wh->lock_m);
    }else{
        wh->choclate += amount;
    }
    if(wh->choclate >= CHOCLATE_FOR_ONE_MUFFIN) {
        pthread_cond_signal(&wh->enough_chocolate);
    }
    pthread_mutex_unlock(&wh->chocolate_m);    
    return;
}

void deposit_sugar(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->sugar_m);
    if(wh->sugar + amount > MAX_STORE_CAPACITY){
        wh->sugar = MAX_STORE_CAPACITY;
        pthread_mutex_lock(&wh->lock_m);
        while (wh->sugar == MAX_STORE_CAPACITY && wh->locked == 0) {
            pthread_mutex_unlock(&wh->lock_m);
            pthread_cond_wait(&wh->sugar_is_taken,&wh->sugar_m);
            pthread_mutex_lock(&wh->lock_m);
        }
        pthread_mutex_unlock(&wh->lock_m);
    }else{
        wh->sugar += amount;
    }
    if(wh->sugar >= SUGAR_FOR_ONE_MUFFIN) {
        pthread_cond_signal(&wh->enough_sugar);
    }
    pthread_mutex_unlock(&wh->sugar_m);
    return;
}

void deposit_flour(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->flour_m);
    if(wh->flour + amount > MAX_STORE_CAPACITY){
        wh->flour = MAX_STORE_CAPACITY;
        pthread_mutex_lock(&wh->lock_m);
        while (wh->flour == MAX_STORE_CAPACITY && wh->locked == 0)
        {   
           pthread_mutex_unlock(&wh->lock_m); 
           pthread_cond_wait(&wh->flour_is_taken,&wh->flour_m);
           pthread_mutex_lock(&wh->lock_m);
        }
        pthread_mutex_unlock(&wh->lock_m);
    }else{
        wh->flour += amount;
    }
    if(wh->flour >= FLOUR_FOR_ONE_MUFFIN) {
        pthread_cond_signal(&wh->enough_flour);
    }
    pthread_mutex_unlock(&wh->flour_m); 
    return;
}


//FUNCTIONS CALLED BY BAKERY
void lock_warehouse(warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->lock_m);
    wh->locked=1;
    pthread_mutex_unlock(&wh->lock_m);

    pthread_mutex_lock(&wh->chocolate_m);
    pthread_cond_signal(&wh->chocolate_is_taken);
    pthread_mutex_unlock(&wh->chocolate_m);

    pthread_mutex_lock(&wh->flour_m);
    pthread_cond_signal(&wh->flour_is_taken);
    pthread_mutex_unlock(&wh->flour_m);
    
    pthread_mutex_lock(&wh->sugar_m);
    pthread_cond_signal(&wh->sugar_is_taken);
    pthread_mutex_unlock(&wh->sugar_m);
}

unsigned int get_used_resources(warehouse* wh, int type){
    if(type == SUGAR) return wh->sugar_taken;
    if(type == FLOUR) return wh->flour_taken;
    if(type == CHOCLATE) return wh->choclate_taken;
    return 0;
}


void warehouse_destroy(warehouse* wh){
    //TODO
    pthread_mutex_destroy(&wh->flour_m);
    pthread_mutex_destroy(&wh->sugar_m);
    pthread_mutex_destroy(&wh->chocolate_m);
    pthread_mutex_destroy(&wh->lock_m);

    pthread_cond_destroy(&wh->enough_flour);
    pthread_cond_destroy(&wh->enough_sugar);
    pthread_cond_destroy(&wh->enough_chocolate);
    pthread_cond_destroy(&wh->flour_is_taken);
    pthread_cond_destroy(&wh->sugar_is_taken);
    pthread_cond_destroy(&wh->chocolate_is_taken);
    
    free(wh);

    return;
}

