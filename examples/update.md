# Update

## Definition

```sql
TO <table> SET <column>="<data>" [, <column>="<data>" ...][WHERE <column>=<data> ...]];
```

## Examples

```sql
TO user_auth SET user_id="7" WHERE token="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9";
TO user_auth SET token="awjdajwd", user_id="9" WHERE user_id="5";
```
