#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unordered_map>
#include <string>

#define die(...) do {fprintf(stderr, __VA_ARGS__); exit(1);} while(0)

int main()
{
    int f = open("s.log", O_RDONLY);
    if (f == -1)
        die("Can't open s.log: %m\n");
    uint64_t *m = (uint64_t*)mmap(nullptr, 8UL*1024*1048576, PROT_READ, MAP_SHARED, f, 0);
    if (m == MAP_FAILED)
        die("Can't mmap: %m\n");

    std::unordered_map<std::string, int> traces;
    int nt = 0;

    while (*m)
    {
        uint64_t len = m[2];
        const std::string stack((char*)(m+3), 8*len);
        if (!traces.count(stack))
            traces.insert(std::pair<std::string, int>(stack, ++nt));
        printf("%zu %zu %zu %u\n", m[0], m[1], m[2], traces[stack]);
        m += len + 3;
    }

    return 0;
}
