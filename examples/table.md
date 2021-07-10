# Tables

## Defining

### Definition

```sql
DEFINE TABLE <name[:TTL]> WITH <column>, <column>, ... [MOD <modifier>, <modifier>, ...];
```

> NOTE: All columns are stored as strings

> TTL: How many seconds a record will live, when it reaches 0
> it will get removed, if excluded from definition it wont
> have a ttl and will exist until removed

### MODIFIERS

:POKE - Pokes time to live, this will set the ttl on the accessed record to tables ttl setting, extending it's life

:ONE_TOUCH - Removes record when accessed for the first time. Is this even useful?

### Examples

For storing a user and corresponding auth token.
Like for sessions

```sql
DEFINE TABLE user_auth:300 WITH user_id, token MOD :POKE;
```

Storing posts like in a database,
has to be removed manually.

```sql
DEFINE TABLE posts WITH post_id, description, title;
```

For messages that only should be accessed once
and only exist for 7.5 minutes

```sql
DEFINE TABLE dead_drop:450 WITH message MOD :ONE_TOUCH;
```

For storing things as a key value pair
but both key and value can be searched

```sql
DEFINE TABLE key_value_pair WITH key, value;
```

## Destroying

### Definition

```sql
DESTROY <table>;
```

### Examples

```sql
DESTROY auth_user;
```
