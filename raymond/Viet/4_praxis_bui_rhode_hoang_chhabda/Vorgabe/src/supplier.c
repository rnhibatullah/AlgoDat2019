#include"./../include/supplier.h"
#include"./../include/warehouse.h"
#include<time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct forwarding_agent{
    supplier* sugar_supplier;
    supplier* flour_supplier;
    supplier* choclate_supplier;
    
    int stop_suppliers;
    //TODO
    pthread_mutex_t stop;
    pthread_cond_t stop_signal;
};

//FUNCTIONS CALLED BY BAKERY
forwarding_agent* forwarding_agent_create(warehouse* wh){    
    forwarding_agent* new_fa = malloc(sizeof(forwarding_agent));
    if(new_fa == NULL)
        return NULL;
    
    new_fa->sugar_supplier =supplier_create(wh);
    new_fa->flour_supplier = supplier_create(wh);
    new_fa->choclate_supplier = supplier_create(wh);
    
    new_fa->stop_suppliers = 0;
    //TODO
    /*------------------ Mutex und Condvar init ---------------------*/
    pthread_mutex_init(&new_fa->stop,NULL);
    pthread_cond_init(&new_fa->stop_signal,NULL);
    /*---------------------------------------------------------------*/
    return new_fa;
    
}

//THREAD FUNCTION
void* forwarding_agent_working(void* forward_agent){
    forwarding_agent* fa = (forwarding_agent*)forward_agent;
    //TODO
    pthread_t sugar_supplier_thread;
    pthread_t flour_supplier_thread;
    pthread_t choclate_supplier_thread;
    
    pthread_create(&sugar_supplier_thread,NULL,deliver_sugar,fa->sugar_supplier);
    pthread_create(&flour_supplier_thread,NULL,deliver_flour,fa->flour_supplier);
    pthread_create(&choclate_supplier_thread,NULL,deliver_choclate,fa->choclate_supplier);
    
    pthread_mutex_lock(&fa->stop);
    pthread_cond_wait(&fa->stop_signal,&fa->stop);
    pthread_mutex_unlock(&fa->stop);
    
    end_job(fa->sugar_supplier);
    end_job(fa->flour_supplier);
    end_job(fa->choclate_supplier);
        
    pthread_join(choclate_supplier_thread,NULL);
    pthread_join(sugar_supplier_thread,NULL);
    pthread_join(flour_supplier_thread,NULL);
    
    supplier_destroy(fa->choclate_supplier);
    supplier_destroy(fa->sugar_supplier);
    supplier_destroy(fa->flour_supplier);
    return NULL;
}

//FUNCTIONS CALLED BY MANAGEMENT
void stop_forwarding_agent(forwarding_agent* fa){
    //TODO
    pthread_mutex_lock(&fa->stop);
    fa->stop_suppliers = 1;
    pthread_cond_signal(&fa->stop_signal);
    pthread_mutex_unlock(&fa->stop);
}

void forwarding_agent_destroy(forwarding_agent* fa){
    //TODO
    pthread_mutex_destroy(&fa->stop);
    pthread_cond_destroy(&fa->stop_signal);    
    free(fa);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


struct supplier{
    int working;
    warehouse* wh;
    //TODO
    pthread_mutex_t working_m;
};

supplier* supplier_create(warehouse* wh){
    
    supplier* new_supplier = malloc(sizeof(supplier));
    if(new_supplier == NULL)
        return NULL;
    
    new_supplier->working=1;
    new_supplier->wh = wh;
    //TODO
    pthread_mutex_init(&new_supplier->working_m,NULL);
    
    return new_supplier;
}

//THREAD FUNCTIONS
void* deliver_choclate(void* sup){
    //TODO
    supplier* s = (supplier*)sup;
    srand(time(0));
    pthread_mutex_lock(&s->working_m);
    while(s->working){
        pthread_mutex_unlock(&s->working_m);
        unsigned int amount = rand()%2+1;
        deposit_choclate(amount, s->wh);
        pthread_mutex_lock(&s->working_m);
    }
    pthread_mutex_unlock(&s->working_m);
    return NULL;
}

void* deliver_sugar(void* sup){
    //TODO
    supplier* s = (supplier*)sup;
    srand(time(0));
    pthread_mutex_lock(&s->working_m);
    while(s->working){
        pthread_mutex_unlock(&s->working_m);
        unsigned int amount = rand()%3+1;
        deposit_sugar(amount, s->wh);
        pthread_mutex_lock(&s->working_m);
    }
    pthread_mutex_unlock(&s->working_m);
    return NULL;
}

void* deliver_flour(void* sup){
    //TODO
    supplier* s = (supplier*)sup;
    srand(time(0));
    pthread_mutex_lock(&s->working_m);
    while(s->working){
        pthread_mutex_unlock(&s->working_m);
        unsigned int amount = rand()%6+1;
        deposit_flour(amount, s->wh);
        pthread_mutex_lock(&s->working_m);
    }
    pthread_mutex_unlock(&s->working_m);
    return NULL;
}

//FUNCTIONS CALLED BY FORWARDING_AGENT
void end_job(supplier* s){
    //TODO
    pthread_mutex_lock(&s->working_m);
    s->working=0; 
    pthread_mutex_unlock(&s->working_m);
}

void supplier_destroy(supplier* s){
    //TODO
    pthread_mutex_destroy(&s->working_m);
    free(s);
}
