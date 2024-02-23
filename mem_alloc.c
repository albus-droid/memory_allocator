#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

typedef char ALIGN[16];

union header {
	struct {
		size_t size;
		unsigned is_free;
		union header *next;
		union header *prev;
	} s;
	ALIGN stub;
};

typedef union header header_t;

header_t *head = NULL, *tail = NULL;
pthread_mutex_t global_malloc_lock;

void lock_mutex(pthread_mutex_t *lock) {
	pthread_mutex_lock(lock);
}

void unlock_mutex(pthread_mutex_t *lock) {
	pthread_mutex_unlock(lock);
}

header_t *get_free_block(size_t size) {
	header_t *curr = head;
	while(curr) {
		if (curr->s.is_free && curr->s.size >= size)
			return curr;
		curr  = curr->s.next;
	}
	return NULL;
}

void free(void *block) {
	if(!block)
		return;
	lock_mutex(&global_malloc_lock);

	header_t *header = (header_t*)block - 1;
	void *programbreak = sbrk(0);

	if ((char*)block + header->s.size == programbreak) {
		if  (head->s.prev)
			header->s.prev->s.next = NULL;
		else
			head = NULL;
		tail = header->s.prev;

		sbrk(0 - header->s.size - sizeof(header_t));
		unlock_mutex(&global_malloc_lock);
		return;
	}

	header->s.is_free = 1;
	unlock_mutex(&global_malloc_lock);
}

void *malloc(size_t size) {
	if (!size)
		return NULL;
	lock_mutex(&global_malloc_lock);

	header_t *header = get_free_block(size);
	if (header) {
		header->s.is_free = 0;
		unlock_mutex(&global_malloc_lock);
		return (void*)(header + 1);
	}

	size_t total_size = sizeof(header_t) + size;
	void *block = sbrk(total_size);
	if (block == (void*) -1) {
		unlock_mutex(&global_malloc_lock);
		return NULL;
	}

	header = block;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = NULL;
	header->s.prev = tail;
	if (tail)
		tail->s.next = header;
	tail = header;
	if (!head)
		head = header;

	unlock_mutex(&global_malloc_lock);
	return (void*)(header + 1);
}

void *calloc(size_t num, size_t nsize) {
	size_t size = num * nsize;
	if (nsize != size / num)
		return NULL;
	void *block = malloc(size);
	if (!block)
		return NULL;
	memset(block, 0, size);
	return block;
}

void *realloc(void *block, size_t size) {
	if (!block || !size)
		return malloc(size);

	header_t *header = (header_t*)block - 1;
	if (header->s.size >= size)
		return block;

	void *ret = malloc(size);
	if (ret) {
		memcpy(ret, block, header->s.size);
		free(block);
	}
	return ret;
}

void print_mem_list() {
	header_t *curr = head;
	printf("head = %p, tail - %p \n", (void*)head, (void*)tail);
	while(curr) {
		printf("addr = %p, size = %zu, is_free=%u, next=%p\n", (void*)curr, curr->s.size, curr->s.is_free, (void*)curr->s.next);
		curr = curr->s.next;
	}
}
