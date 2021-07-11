# Redisqle

Ridesqle come from combining Redis and SQL into a new word meaning ridicule,
since I'm ridiculed by myself by making this project.

## Protocol

> NOTE: This is a WIP and drastic changes may come.

### Requests

All requests comes with 2 tcp messages.

First one is a padded 32 byte long message containing the size of the query.
Note: null terminator must NOT be included in size

Second one is the query.

### Response

Responses comes in 3 parts:

First one is a 2 byte code which describes what will come.

ex.

-   DT - Means that data will come.
-   SX - Means invalid syntax

Second one is the size of the data/message/whatever may come.

Third one is the data/message/whatever
