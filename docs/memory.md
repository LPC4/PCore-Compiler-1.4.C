# Memory Management

## Allocation

Use the `alloc` function to allocate memory in bytes. This function does not perform implicit checks; it is the programmer's responsibility to ensure the size passed is valid.

### Syntax
```c++
// Allocate n bytes of memory
void* ptr = alloc(size_t n);
```

### Example
```c++
// Allocate 128 bytes of memory
void* ptr = alloc(128);
```

## Size Calculation

Use the `sizeof(type)` function to calculate the size of a type in bytes. Combine it with `alloc` to allocate memory for specific data structures.

### Syntax
```c++
// Calculate the size of a type in bytes
int size = sizeof(type);
```

### Example
```c++
// Allocate memory for an array of  10 integers
int* array = alloc(sizeof(int) * 10);
```

## Deallocation

Use the `free` function to manually deallocate memory. The programmer is responsible for ensuring that memory is only freed once.

### Example
```c++
// Free the allocated memory
free(array);
```

## Invalid Memory Access

Any invalid memory access will result in a runtime panic. This includes:
- Accessing unallocated memory.
- Double freeing memory.

This behavior ensures that memory-related errors are caught during runtime, preventing undefined behavior.

## Pointer Operations

### Dereferencing a Pointer
Dereferencing a pointer means accessing the value at the address stored in the pointer.

```c++
int x = 5;
int* a = &x; // a points to the address of x
int z = *a;  // z is assigned the value at the address a points to (which is 5)
```

### Assigning an Address to a Pointer
You can assign the address of a variable to a pointer using the address-of operator `&`.

```c++
int x = 5;
int* a = &x; // a now holds the address of x
```

### Changing the Value via a Pointer
You can change the value at the address stored in a pointer by dereferencing it and assigning a new value.

```c++
int x = 5;
int* a = &x; // a points to the address of x
*a = 10;     // the value of x is now 10
```

### Pointer to Pointer
A pointer to pointer stores the address of another pointer.

```c++
int x = 5;
int* a = &x; // a points to the address of x
int** b = &a; // b points to the address of a
```

### Pointer Arithmetic
Adding to a pointer moves it by multiples of the type's size.

```c++
int arr[3] = {1, 2, 3};
int* p = arr; // p points to the first element of arr
p++;          // p now points to the second element of arr
```

### Complete Example
```c++
program MemoryManagement;

() -> int
func main {
    int x = 5;
    int y = 15;

    int* a = &x;  // Pointer to x
    int z = *a;   // Dereference a to get the value of x and assign to z
    print("z = %d\n", z); // Output: 5

    *a = 10;      // Change the value of x via the pointer a
    print("x = %d\n", x); // Output: 10

    int** b = &a; // Pointer to pointer
    print("Value at address stored in b = %d\n", **b); // Output: 10

    int arr[3] = {1, 2, 3};
    int* p = arr; // p points to the first element of arr
    print("First element: %d\n", *p); // Output: 1

    p += 1; // Move to the next element
    printf("Second element: %d\n", *p); // Output: 2

    p -= 1; // Move back to the first element
    print("First element again: %d\n", *p); // Output: 1

    return 0;
}
```

## Notes

- **Size Calculation**: The `sizeof(type)` function is part of the standard library and calculates sizes in bytes.
- **Manual Memory Management**: Be cautious with manual memory management to avoid memory leaks or segmentation faults. Always ensure allocated memory is freed once it is no longer needed.
- **Runtime Panics**: Invalid memory accesses, such as accessing unallocated memory or double freeing memory, will result in a runtime panic, helping catch errors early.

## Best Practices

- **Initialize Pointers**: Always initialize pointers to `nullptr` after declaration.
- **Check Allocations**: Verify that memory allocation is successful by checking if the returned pointer is not `nullptr`.
- **Avoid Memory Leaks**: Ensure every `alloc` has a corresponding `free`.
- **Avoid Dangling Pointers**: After freeing memory, set the pointer to `nullptr` to avoid dangling pointers.

---