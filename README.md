Machine learning algorithms implementation in C

**Coding conventions**

**_Memory ownership model_**

- If a function is passed a pointer, do not free that pointer.
- If a function is passed a pointer to a pointer, i.e. \*\*p, that function can free \*p and set \*p to null. So that the caller of that function will know that the pointer is already freed
