# Datastore - In lack of a better name

A fast\* in-memory datastore with a SQL like language.
Doesn't persist data.

Tables are only allowed to be created via cli and not with a connection,
this is because there are pointers to tables, which may move in
memory if the vector/unordered_map resizes

Footnotes:
fast - Not tested

## CRUD

-   [Create](create.md)
-   [Read](read.md)
-   [Update](Update.md)
-   [Delete](delete.md)
