# http-server

A simple multithreaded HTTP file server written in C. It serves static files from a `static/` directory using a thread pool to handle concurrent connections.

---

## Project Structure

```
http-server/
├── include/
│   ├── threadpool.h
│   ├── handler.h
│   ├── mime.h
│   └── file_utils.h
├── src/
│   ├── main.c          # socket setup and accept loop
│   ├── handler.c       # HTTP request parsing and response
│   ├── threadpool.c    # thread pool implementation
│   ├── mime.c          # file extension to MIME type
│   └── file_utils.c    # read file into buffer
├── static/             # put your HTML, CSS, JS files here
└── Makefile
```

---

## Requirements

- GCC
- POSIX-compatible OS (Linux / macOS)
- pthreads (included in glibc, no extra install needed)

---

## Building

```bash
make
```

This compiles everything into a `server` binary. To clean up build artifacts:

```bash
make clean
```

---

## Running

```bash
./server
```

The server starts on port `3000` by default. Drop your files into the `static/` folder and open `http://localhost:3000` in a browser. Requesting `/` serves `static/index.html`.

---

## Configuration

A few constants at the top of the relevant source files control behaviour:

| Constant     | File                   | Default    | Description                   |
| ------------ | ---------------------- | ---------- | ----------------------------- |
| `PORT`       | `src/main.c`           | `3000`     | Port the server listens on    |
| `BACKLOG`    | `src/main.c`           | `10`       | Max pending connections       |
| `N_THREADS`  | `include/threadpool.h` | `4`        | Number of worker threads      |
| `STATIC_DIR` | `src/handler.c`        | `"static"` | Directory to serve files from |

---

## Supported MIME Types

`.html`, `.css`, `.js`, `.json`, `.png`, `.jpg`, `.jpeg`, `.ico`, `.svg`

Anything else is served as `application/octet-stream`. Add more in `src/mime.c`.

---

## How It Works

1. `main.c` creates a TCP socket, binds it to `PORT`, and starts the thread pool.
2. The main loop calls `accept()` and hands each incoming connection to `tpool_add_task()`.
3. A worker thread picks up the task and calls `handle_client()`.
4. `handle_client()` reads the HTTP request, resolves the file path, and sends back the file with the correct headers — or a 403/404 if something is wrong.
5. The connection is closed after the response is sent.

---

## Error Responses

| Status | Cause                                    |
| ------ | ---------------------------------------- |
| 400    | Malformed request line                   |
| 403    | Path contains `..` (directory traversal) |
| 404    | File not found in `static/`              |

---

## Limitations

- Only `GET` requests are handled; other methods are not explicitly rejected (the server will attempt a file lookup and likely 404).
- No HTTPS support.
- No keep-alive; every connection closes after one response.
- The server does not daemonize — run it in a terminal or wrap it with a process manager if needed.
