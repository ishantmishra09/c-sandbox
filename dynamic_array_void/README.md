# dynarr

A type-generic dynamic array written in C. It stores any element type using a `void *` buffer and a fixed `elem_size`.

---

## Project Structure

```
dynarr/
├── include/
│   └── dynarr.h        # public API and dynarr_t struct
├── src/
│   └── dynarr.c        # implementation
└── examples/
    └── main.c          # usage examples: float, struct
```

---

## Requirements

- GCC (C11 or later)
- Any POSIX-compatible OS (Linux / macOS) or Windows with MinGW

---

## Building

```bash
make
```

---

## Usage

Initialise an array by passing the size of the element type, then push pointers to values:

```c
#include "include/dynarr.h"

static void fmt_int(const void *e) { printf("%d", *(const int *)e); }

dynarr_t arr;
dynarr_init(&arr, sizeof(int));

int x = 42;
dynarr_push(&arr, &x);
dynarr_fmt(&arr, fmt_int);   /* [42] */

dynarr_free(&arr);
```

Works identically with any type — primitives, structs, or nested types:

```c
typedef struct { char name[32]; float score; } player_t;

static void fmt_player(const void *e) {
    const player_t *p = (const player_t *)e;
    printf("{%s: %.1f}", p->name, p->score);
}

dynarr_t players;
dynarr_init(&players, sizeof(player_t));

player_t p = {"Arjun", 98.0f};
dynarr_push(&players, &p);
dynarr_fmt(&players, fmt_player);   /* [{Arjun: 98.0}] */

dynarr_free(&players);
```

---

## API

All functions return `bool` — `true` on success, `false` on failure (with a diagnostic printed to `stderr`).

| Function                        | Description                                             |
| ------------------------------- | ------------------------------------------------------- |
| `dynarr_init(arr, elem_size)`   | Initialise array for elements of `elem_size` bytes      |
| `dynarr_free(arr)`              | Release storage and zero the struct                     |
| `dynarr_push(arr, value)`       | Append a copy of the object pointed to by `value`       |
| `dynarr_pop(arr, out)`          | Remove the last element; copy it into `out` if non-NULL |
| `dynarr_get(arr, index, out)`   | Copy element at `index` into `out`                      |
| `dynarr_set(arr, index, value)` | Overwrite element at `index` with a copy of `value`     |
| `dynarr_fmt(arr, fmt_elem)`     | Print `[e0, e1, ...]` using a caller-supplied formatter |

---

## Configuration

Constants at the top of `src/dynarr.c` control growth and shrink behaviour:

| Constant               | Default | Description                                          |
| ---------------------- | ------- | ---------------------------------------------------- |
| `DYNARR_MIN_CAPACITY`  | `4`     | Minimum allocated capacity; never shrinks below this |
| `DYNARR_GROWTH_FACTOR` | `2`     | Capacity multiplier when the array is full           |

Shrink heuristic: capacity is halved when `size <= capacity / 4`, keeping memory use proportional to the number of elements.

---

## How It Works

1. `dynarr_init` allocates a raw byte buffer of `DYNARR_MIN_CAPACITY * elem_size` bytes.
2. `dynarr_push` doubles capacity via `realloc` when `size >= capacity`. On failure the array is left unchanged and `false` is returned.
3. `dynarr_get`, `dynarr_set`, and `dynarr_pop` all use `memcpy` to read and write elements — no casting, no type assumptions.
4. `dynarr_fmt` walks the buffer in `elem_size` strides and calls the formatter once per element.
5. `dynarr_free` calls `free` and zeroes all fields so the struct is safe to inspect after release.

---

## Error Handling

Every function validates its arguments and reports failures to `stderr` without calling `exit`. The caller decides how to handle errors:

```c
int val;
if (!dynarr_get(&arr, 99, &val)) {
    /* dynarr_get: index 99 out of bounds (size=4) */
    /* handle or propagate */
}
```

## Limitations

- Elements are copied by value (`memcpy`). If an element contains owning pointers (e.g. a `char *` to heap memory), the caller is responsible for managing the lifetime of the pointed-to data before calling `dynarr_free`.
- Not thread-safe. External locking is required for concurrent access.
- No iterator or search API — not yet implemented.
