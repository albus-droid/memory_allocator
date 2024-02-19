typedef char ALIGN[16];

void *malloc(size_t size)
{
	void *block;
	block = sbrk(size);
	if (block == (void*) -1)
		return NULL;
	return block;
}

union header {
	struct header_t {
		size_t size;
		unsigned is_free;
		struct header_t *next;
	} s;
	ALIGN stub;
};

typedef union header header_t;

header_t *head, *tail;

