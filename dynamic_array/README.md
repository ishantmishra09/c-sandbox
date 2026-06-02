# dynarr

A dynamic array implementation for storing `int` values in C.

---

## Project Structure

```text
dynarr/
├── include/
│   └── dynarr.h        # public API and dynarr_t definition
├── src/
│   └── dynarr.c        # implementation
└── examples/
    └── main.c          # usage example
```

---

## Requirements

- GCC (C11 or later)
- Any POSIX-compatible OS (Linux/macOS) or Windows with MinGW

---

## Building

```bash
make
```

---

## Usage

```c
#include "dynarr.h"

dynarr_t arr;
dynarr_init(&arr);

dynarr_push(&arr, 42);
dynarr_push(&arr, 13);
dynarr_push(&arr, 7);

dynarr_fmt(&arr);   /* [42, 13, 7] */

dynarr_free(&arr);
```

---

## Configuration

Constants at the top of `src/dynarr.c` control allocation behaviour:

| Constant              | Default | Description                                                          |
| --------------------- | ------- | -------------------------------------------------------------------- |
| `DYNARR_MIN_CAPACITY` | `4`     | Minimum allocated capacity; the array never shrinks below this value |

Shrink heuristic: capacity is halved when `size <= capacity / 4`, keeping memory usage proportional to the number of stored elements.

---

## How It Works

1. `dynarr_init` allocates storage for `DYNARR_MIN_CAPACITY` integers.
2. `dynarr_push` appends values to the end of the array.
3. When the array becomes full, capacity is doubled using `realloc`.
4. `dynarr_get`, `dynarr_set`, and `dynarr_pop` provide indexed access and removal operations.
5. When the array becomes sparsely populated, capacity may be reduced to reclaim unused memory.
6. `dynarr_free` releases allocated memory and resets the structure to a safe empty state.

---

## Error Handling

All public functions validate their arguments and report errors to `stderr` without terminating the program. The caller is responsible for deciding how to handle failures.

```c
int value;

if (!dynarr_get(&arr, 99, &value)) {
    /* dynarr_get: index 99 out of bounds */
}
```

---

## Limitations

- Stores `int` values only.
- Not thread-safe; external synchronization is required for concurrent access.
- No iterator API.
- No search, sort, or capacity reservation APIs yet.

---
