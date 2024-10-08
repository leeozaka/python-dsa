# Python Interpreter
This project were my homework in my undergraduate.
It was not mean't to be a serious project, but still it's one of my dumbest and favourites code ever written.

### But how?
Actually, i don't know.
I don't think that it's even a good idea.
When i was planning how to do it, i've never even dreamed to interpret a language using another. So, the ideas were chaning fast during the development.
Actually, it can even run some things. This makes me happy tbh.

## How it works?
The program tokenizes a text into linked lists then uses a (big) recursive function to find if has anything to do. The recursive call starts when a function is called, so, it makes a new scoped block of linked lists. This was the way that i found so i could call functions inside functions.
For behaves just like functions, but they don't make a memory pointer.
Wait, a memory pointer?

### How it works 1.0.1: The memory
This program emulates a stack memory. Variables go to the stack and functions have a returning memory pointer. When a function ends, the memory will be cleaned up and it will try to find anything the proper returning memory. The scoped functions can't access memory outside of their scope (C is unsafe by itself, don't need no more unsafety)

### How it works 1.1: Meth
Mathematics are handled with a [f* strange linked lists structure](https://www.geeksforgeeks.org/generalized-linked-list/). The first thing that the loop seeks for, it's for a operation (like +, -, //, etc), resolves them, and leaves a rebuild flag. The program continues normally, and at the end of the loop, rebuilds the line (if we need of the original line further, like in a for loop). It could be a little more sophisticated, but i was running out of ideas.

## Installation
It was built using a Makefile with clangd compiler. Everything you'll need, until the CLI, can be built at almost any platform. When the graphic part gets implemented, it will use a lot of windows console api, so, use a Windows machine to build

```bash
make
```

## Contributing
I don't think that i will be active on this repository, since it's a academic project. Tbh, after some time into the development, i thought to rewrite everything, the edge cases start to grow really fast when you are developing something to run code.
