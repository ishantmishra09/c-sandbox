# c-sandbox

A personal sandbox for learning and exploring C — built incrementally as I go deeper into the language. Each project is self-contained with its own README.

---

## Projects

### Data Structures

| Project | Description |
|---|---|
| [`data-structures/linked_list`](./data-structures/linked_list/) | Singly linked list — insert, delete, traverse |
| [`dynamic_array`](./dynamic_array/) | Dynamic array for `int` — resizable buffer with manual memory management |
| [`dynamic_array_void`](./dynamic_array_void/) | Generic dynamic array using `void *` — type-agnostic resizable buffer |

### Systems / Networking

| Project | Description |
|---|---|
| [`http-server`](./http-server/) | Minimal HTTP server from scratch using sockets |

---

## Goals

- Get comfortable with manual memory management (`malloc`, `realloc`, `free`)
- Understand pointers, pointer arithmetic, and `void *` generics
- Build foundational data structures from scratch
- Explore systems programming — sockets, I/O, processes
- Write clean, readable C without over-engineering

---

## Structure

```
c-sandbox/
├── dynamic_array/      # ( int ) dynamic array
├── dynamic_array_void/ # ( generic ) dynamic array
├── data-structures/
│   └── linked_list/    # singly linked list
├── http-server/        # minimal HTTP server
└── README.md           # you are here
```

Each subdirectory has its own `README.md` with build instructions, usage, and notes.

---

*This repo grows as I learn. Implementations may be rough — that's the point.*