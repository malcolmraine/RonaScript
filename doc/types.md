# Types
There are 8 basic types in RonaScript. Classes are all described as object types, without consideration to 
their specific classes.

| Type       |                Description                 |          Example           |
|:-----------|:------------------------------------------:|:--------------------------:|
| `string`   |           A string of characters           |      `"Hello world"`       |
| `int`      |                 An integer                 |         `1999999`          |
| `array`    |             An array of values             |       `[1, 2, 3, 4]`       |
| `callable` | A callable, such as a routine or a closure | `{routine @ 0x0928810491}` |
| `object`   |    An object, such as a class instance     | `{object @ 0x18468817467}` |
| `bool`     |     A ```true``` or ```false``` value      |           `true`           |
| `float`    |           A floating point value           |       `1.0928845901`       |
| `any`      |                 Any value                  |       `1.0928845901`       |

## ```string```
A ```string``` in is a sequence of characters surrounded by double quotes

## ```int```
An ```int``` is a positive or negative whole number.

## ```array```
An ```array``` is a sequence of values, stored as ```any``` types.

## ```callable```
A ```callable``` is either a routine or a closure. A closure if just a reduced syntax for an anonymous function 
and is treated the same as a normal routine otherwise.

## ```object```
An ```object``` is a class instance.

## ```bool```
A boolean, ```true``` or ```false``` value.

## ```float```
A decimal number.

## ```any```
An ```any``` value is essentially a variant and can hold any of the above types.

