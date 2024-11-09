# Department of Sanitization

## Description

```
Due to recent budget cuts, sanitizers can't use all the RAM they used to. Surely they're still effective though?
```

Difficulty: probably like 250-300?


## Distribution
Build per dockerfile removing flag, and distribute `libsan.so`, `dept_of_sanitization`.


## Internal challenge description
This is a custom address sanitizer implementation which uses a hash (xxhash) instead of a one-to-one mapping for determining the shadow map. This means that it's possible to groom the heap effectively to cause a collision between a new allocation’s address and free’d allocation’s address, letting you effectively bypass the sanitization and read free’d memory.


## Notes
* solve script isn't amazing, might take a handful of runs to be successful.
