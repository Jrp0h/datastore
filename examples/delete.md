# Delete

## Definition

```sql
FROM <table> DELETE [WHERE <column>=<data> [OR/AND <column>="<data>" ...]];
```

## Examples

```sql
FROM user_auth DELETE WHERE token="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9";
FROM user_auth DELETE WHERE user_id="5";
```

```sql
FROM posts DELETE;
```
