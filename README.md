# arena.h

Header-only memory arena for C/C++

## Usage

Copy `arena.h` to your project.

```c

#include "arena.h"

#include <stdio.h>

int main() {
    // Create memory arena with the size of 14 bytes
    Arena* arena = arena_new(14);
    // Allocate buffer for string "Hello, World!" + NULL-terminator
    char* buffer = arena_alloc(arena, 14);
    // Print string to buffer
    sprintf(buffer, "Hello, World!");
    // Print string to stdout
    printf("%s\n", buffer);
    // Reset arena to reuse memory
    arena_reset(arena);
    // Print another string to buffer
    sprintf(buffer, "What's up?");
    // Print string to stdout
    printf("%s\n", buffer);
    // Allocate more than arena size. It will interlally allocate new arena with bigger buffer.
    char* buffer2 = arena_alloc(arena, 25);
    // Print string to buffer
    sprintf(buffer, "Hello, World! What's up?");
    // Print new string to stdout
    printf("%s\n", buffer2);
    // Free memory arena
    arena_free(arena);
    return 0;
}
```

That's it :)
