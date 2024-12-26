# Memory Management

## Allocation

Use the `alloc` function to allocate memory in bytes. This function does not perform implicit checks; it is the programmer's responsibility to ensure the size passed is valid.

```c++
// Allocate 128 bytes of memory
void* ptr = alloc(128);
```

## Size Calculation

Use the `sizeof(type)` function to calculate the size of a type in bytes. Combine it with `alloc` to allocate memory for specific data structures.

```c++
// Allocate memory for an array of 10 integers
int* array = alloc(sizeof(int) * 10);
[int, 10] array = alloc(sizeof(int) * 10);
```

## Deallocation

Use the `free` function to manually deallocate memory. The programmer is responsible for ensuring that memory is only freed once.

```c++
// Free the allocated memory
free(array);
```

## Invalid Memory Access

Any invalid memory access will result in a runtime panic. This includes:
- Accessing unallocated memory.
- Double freeing memory.

This behavior ensures that memory-related errors are caught during runtime, preventing undefined behavior.

## Example Usage

```c++
program memory_example

() -> void
main {
    // Allocate memory for a string
    char* str = alloc(sizeof(char) * 100);

    // Perform operations on the allocated memory
    str[0] = 'H';
    str[1] = 'i';
    str[2] = '\0';

    print(str); // Output: Hi

    // Free the allocated memory
    free(str);
}
```

## Notes

- `alloc` and `free` are part of the standard library and must be explicitly imported if not globally available.
- The `sizeof(type)` function is also part of the standard library and calculates sizes in bytes.
- Be cautious with manual memory management to avoid memory leaks or segmentation faults.
