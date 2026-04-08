// Copyright (c) 2024 ACM Class, SJTU

namespace sjtu {

class BuddyAllocator {
private:
    int ram_size;
    int min_block_size;
    int* max_free;
    int total_nodes;

    int max(int a, int b) {
        return a > b ? a : b;
    }

    void update(int u, int node_size) {
        int L = 2 * u, R = 2 * u + 1;
        if (max_free[L] == node_size / 2 && max_free[R] == node_size / 2) {
            max_free[u] = node_size;
        } else {
            max_free[u] = max(max_free[L], max_free[R]);
        }
    }

    void build(int u, int node_size) {
        max_free[u] = node_size;
        if (node_size == min_block_size) return;
        build(2 * u, node_size / 2);
        build(2 * u + 1, node_size / 2);
    }

    int _malloc(int u, int node_addr, int node_size, int S) {
        if (max_free[u] < S) return -1;
        if (node_size == S) {
            max_free[u] = 0;
            return node_addr;
        }
        int L = 2 * u, R = 2 * u + 1;
        int mid = node_addr + node_size / 2;
        int res = -1;
        if (max_free[L] >= S) {
            res = _malloc(L, node_addr, node_size / 2, S);
        } else {
            res = _malloc(R, mid, node_size / 2, S);
        }
        update(u, node_size);
        return res;
    }

    int _malloc_at(int u, int node_addr, int node_size, int addr, int S) {
        if (max_free[u] < S) return -1;
        if (node_size == S) {
            max_free[u] = 0;
            return addr;
        }
        int L = 2 * u, R = 2 * u + 1;
        int mid = node_addr + node_size / 2;
        int res = -1;
        if (addr < mid) {
            res = _malloc_at(L, node_addr, node_size / 2, addr, S);
        } else {
            res = _malloc_at(R, mid, node_size / 2, addr, S);
        }
        if (res != -1) {
            update(u, node_size);
        }
        return res;
    }

    void _free_at(int u, int node_addr, int node_size, int addr, int S) {
        if (node_size == S) {
            max_free[u] = S;
            return;
        }
        int L = 2 * u, R = 2 * u + 1;
        int mid = node_addr + node_size / 2;
        if (addr < mid) {
            _free_at(L, node_addr, node_size / 2, addr, S);
        } else {
            _free_at(R, mid, node_size / 2, addr, S);
        }
        update(u, node_size);
    }

public:
    BuddyAllocator(int ram_size, int min_block_size) 
        : ram_size(ram_size), min_block_size(min_block_size) {
        int N = ram_size / min_block_size;
        total_nodes = 2 * N;
        max_free = new int[total_nodes];
        build(1, ram_size);
    }

    ~BuddyAllocator() {
        delete[] max_free;
    }

    int malloc(int size) {
        return _malloc(1, 0, ram_size, size);
    }

    int malloc_at(int addr, int size) {
        return _malloc_at(1, 0, ram_size, addr, size);
    }

    void free_at(int addr, int size) {
        _free_at(1, 0, ram_size, addr, size);
    }
};

} // namespace sjtu
