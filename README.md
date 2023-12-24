# BrainFuck++
BrainFuck++ is a strict superset of BrainFuck.
Any valid BrainFuck program is also a valid BrainFuck++ program.

## Additional syntax:
- `?`: Sets the current cell to it's index
- `!`: Sets the current index to the value of the current cell
- `=<num>`: Sets the current cell's value to the number after the `=`
- `#<num>`: Sets the current index to the given number
- `:<num>`: Adds the number to the current index
- `;<num>`: Subtracts the number from the current index
- `/`: Exits the program with the value in the current cell
- `(` and `)`: Code between is only executed if the current cell is not zero
