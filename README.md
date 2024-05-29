Machine learning algorithms implementation in C

**Coding conventions**

- C99 compatible.
- Make a best effort to follow C89, except in cases where it makes the code harder to read (e.g., the rule for 6 significant characters for external variable names).
- Only use goto for cleaning up in error handling, as C does not support exceptions.

**Memory ownership model**

- If a function is passed a pointer, do not free that pointer.
- If a function is passed a pointer to a pointer, i.e. \*\*p, that function can free \*p and set \*p to null. So that the caller of that function will know that the pointer is already freed
- A function that allocates memory owns it and must be responsible for freeing it, except in cases where it explicitly states ownership transfer.
