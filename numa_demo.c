#include <stdio.h>
#include <stdlib.h>
#include <numa.h>
#include <numaif.h>
#include <errno.h>
#include <sys/mman.h>
#ifndef MPOL_F_STATIC_NODES
/* Bug in numaif.h, this should be defined in there. Definition copied
 * from linux/mempolicy.h.
 */
#define MPOL_F_STATIC_NODES     (1 << 15)
#endif

// #define STUDY_API

static void print_nodes() {
    int max_node = numa_max_node();
    int node_count = max_node + 1;
    long long fsz = 0;
    struct bitmask *current_mask = numa_get_membind();
    printf("┌─────────────────────────────────────┐\n");
    printf(" max num node: %d \n", max_node);
    printf(" current memory mask: 0x%lX \n", *current_mask->maskp);
    for (int i = 0; i < node_count; i++) {
        long long sz = numa_node_size64(i, &fsz);
        printf(" node: %d, size: %lldMB, free: %lldMB \n", i, sz >> 20, fsz >> 20);
    }
    printf("└─────────────────────────────────────┘\n");
    numa_bitmask_free(current_mask);
}

static void* alloc_memory_on_node(size_t size, int node) {
#ifdef STUDY_API
    char *mem = (char *) mmap(mem, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (NULL == mem) {
        printf("failed to malloc memory \n");
        return NULL;
    }
    int max_node = numa_max_node();
    struct bitmask* mask = numa_bitmask_alloc(max_node + 1);
    numa_bitmask_clearall(mask);
    numa_bitmask_setbit(mask, node);

    int mode = MPOL_F_STATIC_NODES | MPOL_BIND;
    printf("new memory mask: 0x%lX \n", *mask->maskp);
    int ret = mbind(mem, size, mode, mask->maskp, max_node, 0);
    if (ret < 0) {
        printf("mbind failed, error: %d \n", errno);
        free(mem);
        return NULL;
    }

    return mem;
#else
    return numa_alloc_onnode(size, node);
#endif
}

static void free_memory(void *mem, size_t size) {
#ifdef STUDY_API
    munmap(mem, size);
#else
    numa_free(mem, size);
#endif
}

int main(int argc, const char* argv[])
{
    if (numa_available() < 0) {
        printf("numa support not available \n");
        return 0;
    }

    print_nodes();

    // malloc 10GB on node 1
    size_t size = 10UL << 30;
    void *mem = alloc_memory_on_node(size, 1);
    memset(mem, 0x0, size);

    print_nodes();

    free_memory(mem, size);
err:
    return 0;
}
