# SAMP-MapMove

Moves SAMP map objects by specified delta.

- Written in C++17.
- **Does not support** rotating objects. 
- Transforms only a position found in `CreateDynamicObject` function.
- Result is generated and saved inside `moved.txt` file.

## Usage

```
MapMove <mapfile> <deltaX> <deltaY> <deltaZ>
```

## Example

```
MapMove map.txt -1000 50 100
```
