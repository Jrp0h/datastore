# Read

## Definition

```sql
FROM <table> [WHERE <column>="<data>" [OR/AND <column>="<data>" ...]] [MOD <modifier>, <modifier>, ...];
```

## Modifiers

ONE - Limits result to one, cancels searching once one is found

WITH_TTL - Adds a column ":ttl" with current ttl

## Examples

```sql
FROM user_auth WHERE token="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9" MOD :ONE;
FROM user_auth WHERE user_id="5";
```

Gets all the rows with column ":ttl" added.

```sql
FROM dead_drop MOD :WITH_TTL;
```

## Backend

Sends in a csv-like format

column1, column2, column3
value1, value2, value3
value1, value2, value3
