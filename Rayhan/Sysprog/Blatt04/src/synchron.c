#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <synchron.h>

void mutexes_init() {
    pthread_mutex_init(&taking_order,NULL);
    pthread_cond_init(&order_taked, &taking_order);
}  