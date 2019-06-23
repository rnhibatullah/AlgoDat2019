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

    //mutex for number of flour, chocolate, and sugar in warehouse
    pthread_mutex_t flour_m, sugar_m, choclate_m;

    //signal from get function to letting dep funtion in supplier know, that 
    //warehouse is ready
    pthread_cond_t flour_get_sig, sugar_get_sig, choclate_get_sig;
    
    //Signal from dep function. Suppliers have to wait if they receive this signal
    pthread_cond_t flour_dep_sig, sugar_dep_sig, choclate_dep_sig;

    pthread_mutex_t locked_m;
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

    //Initialize all mutexes and signals

    //signals from dep funct
    pthread_cond_init(&new_warehouse->flour_dep_sig,NULL);
    pthread_cond_init(&new_warehouse->sugar_dep_sig,NULL);
    pthread_cond_init(&new_warehouse->choclate_dep_sig,NULL);

    //signals from get func.
    pthread_cond_init(&new_warehouse->flour_get_sig,NULL);
    pthread_cond_init(&new_warehouse->sugar_get_sig,NULL);
    pthread_cond_init(&new_warehouse->choclate_get_sig,NULL);


    //mutexes
    pthread_mutex_init(&new_warehouse->flour_m,NULL);
    pthread_mutex_init(&new_warehouse->sugar_m,NULL);
    pthread_mutex_init(&new_warehouse->choclate_m,NULL);
    pthread_mutex_init(&new_warehouse->locked_m,NULL);

    new_warehouse->choclate_taken = 0;
    new_warehouse->flour_taken = 0;
    new_warehouse->sugar_taken = 0;
    return new_warehouse;
}

//FUNCTIONS CALLED BY WORKER THREADS
//@param: amount of flour/sugar/choclate to be taken from warehouse wh

unsigned int get_choclate(unsigned int amount, warehouse* wh){
    
    //TODO
    pthread_mutex_lock(&wh->choclate_m);
    while(wh->choclate < CHOCLATE_FOR_ONE_MUFFIN) {
        //have to gain permission to access the warehouse. have to wait until dep func. send the signal
        //and the number of chocolate is not zero
        pthread_cond_wait(&wh->choclate_get_sig,&wh->choclate_m);
    }
    wh->choclate_taken += amount;
    wh->choclate -= amount;
    pthread_cond_signal(&wh->choclate_dep_sig);
    pthread_mutex_unlock(&wh->choclate_m);
    return amount;
}

unsigned int get_flour(unsigned int amount, warehouse* wh){
    
    //TODO
    pthread_mutex_lock(&wh->flour_m);
    while(wh->flour < FLOUR_FOR_ONE_MUFFIN) {
        //have to gain permission to access the warehouse. have to wait until dep func. send the signal
        //and the number of flour is not zero
        pthread_cond_wait(&wh->flour_get_sig,&wh->flour_m);
    } 
    wh->flour_taken += amount;
    wh->flour -= amount;
    pthread_cond_signal(&wh->flour_dep_sig);
    pthread_mutex_unlock(&wh->flour_m);
    return amount;
}

unsigned int get_sugar(unsigned int amount, warehouse* wh){
    
    //TODO
    pthread_mutex_lock(&wh->sugar_m);
    while(wh->sugar < SUGAR_FOR_ONE_MUFFIN) {
        //have to gain permission to access the warehouse. have to wait until dep func. send the signal
        //and the number of sugar is not zero
        pthread_cond_wait(&wh->sugar_get_sig,&wh->sugar_m);
    }
    wh->sugar_taken += amount;
    wh->sugar -= amount;
    pthread_cond_signal(&wh->sugar_get_sig);
    pthread_mutex_unlock(&wh->sugar_m);
    return amount;
}


//FUNCTIONS CALLED BY SUPPLIER THREADS
//@param: amount of flour/sugar/choclate to be store in warehouse wh
void deposit_choclate(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->choclate_m);
    if(wh->choclate + amount > MAX_STORE_CAPACITY){
        wh->choclate = MAX_STORE_CAPACITY;
        pthread_mutex_lock(&wh->locked_m);
        while (wh->choclate == MAX_STORE_CAPACITY && wh->locked == 0) {
            pthread_mutex_unlock(&wh->locked_m);
            pthread_cond_wait(&wh->choclate_dep_sig,&wh->choclate_m);
            pthread_mutex_lock(&wh->locked_m);
        }
        pthread_mutex_unlock(&wh->locked_m);
    }else{
        wh->choclate += amount;
    }
    if(wh->choclate >= CHOCLATE_FOR_ONE_MUFFIN) {
        //send the signal for get func. to give permission to take the choclate from warehous
        pthread_cond_signal(&wh->choclate_get_sig);
    }
    pthread_mutex_unlock(&wh->choclate_m);    
    return;
}

void deposit_sugar(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->sugar_m);
    if(wh->sugar + amount > MAX_STORE_CAPACITY){
        wh->sugar = MAX_STORE_CAPACITY;
        pthread_mutex_lock(&wh->locked_m);
        while (wh->sugar == MAX_STORE_CAPACITY && wh->locked == 0) {
            pthread_mutex_unlock(&wh->locked_m);
            pthread_cond_wait(&wh->sugar_dep_sig,&wh->sugar_m);
            pthread_mutex_lock(&wh->locked_m);
        }
        pthread_mutex_unlock(&wh->locked_m);
    }else{
        wh->sugar += amount;
    }
    if(wh->sugar >= SUGAR_FOR_ONE_MUFFIN) {
        pthread_cond_signal(&wh->sugar_get_sig);
    }
    pthread_mutex_unlock(&wh->sugar_m);
    return;
}

void deposit_flour(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->flour_m);
    if(wh->flour + amount > MAX_STORE_CAPACITY){
        wh->flour = MAX_STORE_CAPACITY;
        pthread_mutex_lock(&wh->locked_m);
        while (wh->flour == MAX_STORE_CAPACITY && wh->locked == 0)
        {   
           pthread_mutex_unlock(&wh->locked_m); 
           pthread_cond_wait(&wh->flour_dep_sig,&wh->flour_m);
           pthread_mutex_lock(&wh->locked_m);
        }
        pthread_mutex_unlock(&wh->locked_m);
    }else{
        wh->flour += amount;
    }
    if(wh->flour >= FLOUR_FOR_ONE_MUFFIN) {
        pthread_cond_signal(&wh->flour_get_sig);
    }
    pthread_mutex_unlock(&wh->flour_m); 
    return;
}


//FUNCTIONS CALLED BY BAKERY
void lock_warehouse(warehouse* wh){
    //TODO
    pthread_mutex_lock(&wh->locked_m);
    wh->locked=1;
    pthread_mutex_unlock(&wh->locked_m);

    pthread_mutex_lock(&wh->choclate_m);
    pthread_cond_signal(&wh->choclate_dep_sig);
    pthread_mutex_unlock(&wh->choclate_m);

    pthread_mutex_lock(&wh->flour_m);
    pthread_cond_signal(&wh->flour_dep_sig);
    pthread_mutex_unlock(&wh->flour_m);
    
    pthread_mutex_lock(&wh->sugar_m);
    pthread_cond_signal(&wh->sugar_dep_sig);
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
    pthread_mutex_destroy(&wh->choclate_m);
    pthread_mutex_destroy(&wh->locked_m);

    pthread_cond_destroy(&wh->flour_get_sig);
    pthread_cond_destroy(&wh->sugar_get_sig);
    pthread_cond_destroy(&wh->choclate_get_sig);
    pthread_cond_destroy(&wh->flour_dep_sig);
    pthread_cond_destroy(&wh->sugar_dep_sig);
    pthread_cond_destroy(&wh->choclate_dep_sig);
    
    free(wh);

    return;
}

