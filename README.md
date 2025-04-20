# What this is
This is my hand at building a [`bf`](https://brainfuck.org/brainfuck.html) interpretor.

## Motivation
To understand how `interpretors` / `compilers` work. 

This is a very simple one pass interpretor for `BF` that just accepts one source file and a `--debug` flag.

## Building
from the project root
```bash
cmake -S . -B build/
cd build
make
./bf ../examples/hello.bf
```

if you have noticed. There is a [`run.sh`](./run.sh) in the project. That is for the 
```bash
make run
```
command. The reason it exists is to combine 
```bash
make
./bf ../examples/hello.bf
```
into one single command.

With that the output will look something like below.
```bash

 ╰─λ make run
[ 83%] Built target bf
[100%] building and running bf
--------------------------------------------------

Enter args: ../examples/hello.bf 
Hello World!

--------------------------------------------------
[100%] Built target run
```
The script also contains its own history in the `build/` folder so the args provided are searchable just like with normal bash command line interface.

Some might have already noticed but this is a `bash` script.
Which means you need to be in a linux env to run this.


## future goals with the project
As noticed the project is basically complete. `BF's` interpretor is just a giant one pass parser. 

My next step is to integrate the entire `BF's` test suite and make a `compiler`/`JIT` out of this project.
Which ever gives me more to learn.
