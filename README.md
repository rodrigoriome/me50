# My Application to CS50

#### Compiling

Here is a little alias to mimic 2019 CS50's `make` command:

```
alias cs50="make LDLIBS='-lcs50 -lm' CFLAGS='-fsanitize=signed-integer-overflow -fsanitize=undefined -O0 -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow'"
```

Run the code above in your terminal then run `cs50` passing the name of the source code file name as argument.
