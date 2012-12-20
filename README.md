# NSFF - Not So Fast Forward
An evil hybrid A\*/hill-climbing planner used to solve Labyrinth (based
on the board game) problems. It's fast, mostly optimal and is written in
dangerous C(++).   
By Eugen Sawin <esawin@me73.com>.

## Version
This is the winning version (performance, optimality and code size) for the
Hackolaus, a 24h programming contest at the University of Freiburg, Germany.  

### Remarks
Please do **not** learn from this code. The competition ranked, besides
performance, correctness and solution quality, also (uncommented) code size in
number of bytes. So one goal was to provide very dense code. I can proudly say,
this is the *worst code* I have written.
 
## Requirements
* GNU GCC 4.6+
* GNU Make

## Dependencies
None.

## Code Minimizer
My bare hands. Yes, I really wrote it that way from the start.

## Building NSFF
To build NSFF use:

    $ make

## Using NSFF
To start nsff use:

    $ ./nsff < input.txt

## License
The MIT License.
