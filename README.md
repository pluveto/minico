
# Mini Coroutine Library

Minico is a simple, header-only stack-less coroutine library implemented in C. It's a toy project for learning purposes and is not intended for production use.

## Files



## Usage

Include the `co.h` header file in your source code and define a coroutine function. See [examples](examples/) for more details.

To compile the examples, you can use the provided `Makefile`. Simply run:

```sh
make
```

This will compile all the examples in the `examples` directory.

## Examples

### Fibonacci Example

The `fib.c` example demonstrates how to use the coroutine library to generate a Fibonacci sequence. You can compile and run it as follows:

```sh
make fib
./fib
```

### Multi-Producer Multi-Consumer Example

The `mpmc.c` example demonstrates a multi-producer, multi-consumer scenario using coroutines. Compile and run it as follows:

```sh
make mpmc
./mpmc
```

### Scheduler Example

The `sched.c` example demonstrates a simple coroutine scheduler. Compile and run it as follows:

```sh
make sched
./sched
```

### Timer Example

The `timer.c` example demonstrates a coroutine-based timer. Compile and run it as follows:

```sh
make timer
./timer
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue if you have any suggestions or improvements.

## Author

Pluveto <zijing.zhang@proton.me>

