#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 8

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    size_t head;
    size_t tail;
    size_t count;
} CircBuff;

void cb_init(CircBuff *cb){
    memset(cb->buffer, 0, sizeof(cb->buffer));
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool cb_is_empty(const CircBuff *cb){
    return cb->count == 0;
}

bool cb_is_full(const CircBuff *cb){
    return cb->count == BUFFER_SIZE;
}

void cb_push(CircBuff *cb, uint8_t data){
    if(!cb_is_full(cb)){
        cb->buffer[cb->head] = data;
        cb->head = (cb->head + 1) % BUFFER_SIZE;
        cb->count++;
        return;
    }
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
}

bool cb_pop(CircBuff *cb){
    if(cb_is_empty(cb)){
        return false;
    }

    cb->buffer[cb->tail] = 0;
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count--;
    return true;
}


void print_row(const CircBuff *cb){
    printf("\r");
    for (size_t j = 0; j < BUFFER_SIZE; j++){
        printf("%4u", cb->buffer[j]);
    }
    printf("\033[K");
    fflush(stdout);
    sleep(1);
}

int main(void){
    CircBuff cb;
    cb_init(&cb);

    print_row(&cb);

    for(uint8_t i = 10; i <= 200; i += 10){
        cb_push(&cb, i);
        print_row(&cb);
    }

    while(!cb_is_empty(&cb)){
        cb_pop(&cb);
        print_row(&cb);
    }

    printf("\n");

    return 0;
}
