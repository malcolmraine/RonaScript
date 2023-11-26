
## Types
| Type       | Description |          Example           |
|:-----------|:-----------:|:--------------------------:|
| `string`   |    Title    |      `"Hello world"`       |
| `int`      |    Text     |         `1999999`          |
| `array`    |    Text     |       `[1, 2, 3, 4]`       |
| `callable` |    Text     | `{routine @ 0x0928810491}` |
| `object`   |    Text     | `{object @ 0x18468817467}` |
| `bool`     |    Text     |           `true`           |
| `float`    |    Text     |       `1.0928845901`       |

### Ranges
There is currently limited support for additional type information that is not taken into account during runtime.
A type can be given a range that specifies a constraint for the value that can be assigned.

```objective-c++
var age: int<0..99> = 44;  // Variable age is limited to the range 0 to 99
```

## Variables
| Type      | Description                                                                                                                |
|:----------|:---------------------------------------------------------------------------------------------------------------------------|
| `var`     | Normal variable declaration .                                                                                              |
| `local`   | Scoped variable that results in a quick, non-garbage-collected memory allocation.                                          |
| `const`   | Constant value that can only be assigned once.                                                                             |
| `literal` | Literal value whose symbol will be replaced with its contents during parsing. There is currently limited support for this. |

### Example
``` objective-c++
var age: int = 100;
local age: int = 100;
const PI: float = 3.14;
literal TAG: string = "<TEST>"
```

## Functions
Functions are declared with the ```routine``` keyword. Arguments can be named or anonymous and a function can be defined normally or as a closure.
Functions are first class citizens and can be passed around like any other object.

### Standard Usage
```objective-c++
routine foo(var x: float): float
begin
    return x * 6;
end

var n: float = foo(89.232);
```

### Closures
```objective-c++
var fn: callable = routine (var n: int): void { print(n); };

fn(55);
```

### Anonymous Arguments
Anonymous arguments provide a simpler syntax for small functions or closures.
Instead of a named argument using the form ```var <arg>: <type>```, on the type is given.
Arguments are accessed based on their position, using ```$1, $2, ...$<n>``` syntax.
```objective-c++
var fn: callable = routine (int): void { print($1); };

fn(55);
```

## Classes
Classes are declared using the ```class```. They must always have a ```construct``` routine that returns ```this```, which is the object reference.

```objective-c++
class Person is 
begin
    var _age: int = 0;
    var _name: string = "";
	
	routine construct(var name: string, var age: int): object
	begin
	    this->_age = age;
	    this->_name = name;
	    return this;
	end
end
```

## Imports

```objective-c++
import Math.Math;

print(Math::PI);
```

## Modules
A module is essentially a namespace. Members of the namespace can be accessed using the ```::``` operator.
```objective-c++
module Test is
begin
    routine foo(): int
    begin
        return 1;
    end
end

print(Test::foo());
```

## Loops
RonaScript supports both for and while loops.

### ```for``` Loop
```objective-c++
for (var i: int = 0; i < 10; i++):
    print(i);
end
```
### ```while``` Loop
```objective-c++
var i: int = 0;
while i < 10:
    print(i);
    i++;
end
```

## Aliases
Aliases can be used to assign different names to the same object. An alias creates another entry in the current symbol table for the given name.

```objective-c++
var y: int = 9;
alias x is y;
print(x);
```
