#ifndef SCL_RING_BUFFER_
#define SCL_RING_BUFFER_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ringbuffer_t ringbuffer_t;

// Known issue: The size must be the pow of two.
ringbuffer_t * ringbuffer_create(unsigned int size);
void ringbuffer_destroy(ringbuffer_t *ring_buffer);

int ringbuffer_push(ringbuffer_t *ring_buffer, void *data, unsigned int data_size);
int ringbuffer_pop(ringbuffer_t *ring_buffer, void *data, unsigned int data_size);

#ifdef __cplusplus
}
#endif

#endif  // SCL_RING_BUFFER_