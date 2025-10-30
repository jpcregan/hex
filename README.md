# hex

A tool to assemble binary files from text. 

## Requirements

- C89 Compiler

## Compilation

	$ cc hex.c -o hex

## Installation

	# cp hex /usr/local/bin

## Usage

`hex input output`

## Example

	$ cat input
	b8 03 00
	cd 10

	$ hex input output

	$ hd output
	00000000  b8 03 00 cd 10                                    |.....|
	00000005

## Features

- ### Comments
		b8 03 00	; ax = 0x3
		cd 10		; int 0x10

- ### Set position
		0x1fe		; boot
		55 aa

- ### String conversion
		$ cat input
		"Hello, World!"

		$ hex input output

		$ hd output
		00000000  48 65 6c 6c 6f 2c 20 57  6f 72 6c 64 21           |Hello, World!|
		0000000d

## License

Licensed under [0BSD](https://opensource.org/license/0BSD).
