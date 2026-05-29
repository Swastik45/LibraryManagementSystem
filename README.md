# Library Management System

A simple console application that manages books, members, issued records, fines, and search operations using plain text files.

---

## 1. Build and Run

### Build

From the repository root:

```bash
/usr/bin/gcc-13 -I include -g script/main.c script/management.c script/issuereturn.c script/search.c script/fine.c script/display.c -o library_management_system
```

### Run

```bash
./library_management_system
```

### Login

- Username: `admin`
- Password: `password123`

---

## 2. Project Structure

- `script/main.c` - main menu, login, and program flow
- `script/management.c` - add/remove books and members
- `script/issuereturn.c` - issue and return books
- `script/search.c` - search books and members
- `script/fine.c` - calculate fines for overdue books
- `script/display.c` - display current books, members, and issued records
- `include/*.h` - header files for each module
- `books.txt` - stored book catalog
- `members.txt` - stored member list
- `issued.txt` - stored book issue history

---

## 3. What the System Does

The system uses simple text files as storage. Each module reads or writes one of these files:

- `books.txt` contains book title and author lines.
- `members.txt` contains member ID and name lines.
- `issued.txt` contains issue records in the format:
  - `Title=...;ID=...;Time=...`

This design keeps the program easy to understand and modify, without using a database.

---

## 4. Module Responsibilities

### `main.c`

- Handles login and the main menu.
- Calls other modules when the user chooses an option.
- Contains these helpers:
  - `clear_screen()` to clean the terminal.
  - `clean_stdin()` to remove extra input left after `scanf()`.
  - `press_any_key()` to pause until the user presses Enter.

### `management.c`

- Adds and removes books from `books.txt`.
- Adds and removes members from `members.txt`.
- Removal uses a temporary file so the original file is safely rewritten.

### `issuereturn.c`

- `issuereturn()` shows a small menu for issuing or returning books.
- `issue_book()` checks both book title and member ID before writing a new issued record.
- `return_book()` removes the matching line from `issued.txt` and calculates overdue days.

### `search.c`

- Allows searching for books or members.
- Book search checks `books.txt`.
- Member search checks `members.txt`.
- The module prints any matching lines it finds.

### `fine.c`

- Calculates penalties for overdue books.
- Reads `issued.txt` and checks issue dates.
- Charges `5` units per day after 14 days.
- Shows the total fine for the entered member ID.

### `display.c`

- Prints all three data files together:
  - books catalog
  - member list
  - issued records table
- Converts issue timestamps into readable dates.

---

## 5. How the Modules Work Together

1. `main.c` manages the user interface and menu choices.
2. `management.c` updates the content of `books.txt` and `members.txt`.
3. `issuereturn.c` reads the catalog and member files, then writes `issued.txt`.
4. `search.c` reads `books.txt` and `members.txt` to help find matching entries.
5. `fine.c` reads `issued.txt` to compute overdue charges.
6. `display.c` reads all files to show the full system state.

This creates a simple flow: the catalog and members are stored in text files, the issue module records transactions, and the other modules read that shared data.

---

## 6. Important C Functions Used

These functions are used often in the code and are important to understand.

- `fopen(filename, mode)`
  - Opens a file for reading (`"r"`), writing (`"w"`), or appending (`"a"`).

- `fclose(file)`
  - Closes a file when the program is done with it.

- `fgets(buffer, size, stdin)`
  - Reads one line from keyboard input or from a file.

- `strcspn(string, "\n")`
  - Finds the position of the first newline character. Used to remove the trailing newline added by `fgets()`.

- `strstr(line, query)`
  - Searches for text inside another string.
  - Returns a pointer when found, or `NULL` when not found.

- `sscanf(line, "Title=%[^;];ID=%[^;];Time=%ld", title, id, &timestamp)`
  - Parses a string with a known format.
  - In `display.c`, it extracts the title, member ID, and timestamp from `issued.txt`.

- `time(NULL)`
  - Returns the current timestamp in seconds since 1970.
  - Used for issue times and fine calculation.

- `remove(filename)` and `rename(old, new)`
  - Used together when updating a file safely.
  - The program writes changes into a temporary file, then replaces the original file.

---

## 7. Notes & Improvements

- The system is intentionally simple and file-based.
- It does not use a database or advanced data structures.
- Future improvements can include input validation, search case-insensitivity, and safer record matching.

If you want, I can also add a flow diagram or a short example walkthrough showing how a user issues and returns a book.
