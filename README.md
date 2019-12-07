# fnmatch

> Filter input stream using `glob (7)` patterns with `fnmatch (3)`

In the long tradition of posix-style, a simple utility filtering a stream using `fnmatch`.
This program allow to use `glob` syntax instead of `regex` when implementing filtes for ignore or similar stuff.

Syntax
------

```
fnmatch [-v] [-i input] [-o output] [-f patterns]* patterns*"

[-v|-r]       Invert match. Selected lines are those not matching any of the specified patterns.
[-i input]    Read <input> file instead of stdin.     
[-o input]    Write to <output> file instead of stdout.
[-f file]     Read patterns from <file>. May be used more than once.
```

Examples
--------

```
find . |         # Some command producing a list of files
fnmatch '*.[ch]' # One or more expression 
```

Installation
------------

using make:
`make fnmatch`

put it somewhere in reachable in your path (exercice left to the reader).
