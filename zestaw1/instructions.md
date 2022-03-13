# How to build?
## Two types of build:

- shared
  - `make main_shared`
- static
  - `make main_static`

# How to run?:
To run my tests write `make test$(number)_$(buildType)`
for ex. `make test1_static`

You can of course add your own tests or just write it from commandline.
It would look for ex. like this:

- `make main_static`
- `./main_static create_table 1 wc_files "big.txt small.txt"`

If it does not work - check main.c, data written as arguments should match the pattern placed in not really so complicated for loop :)

If you have any issues with the code contact me or just pull request
