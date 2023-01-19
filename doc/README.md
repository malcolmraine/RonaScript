
***
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

## Standard Library
***
### String Methods
| Handle                          | Arguments | Description                                          |
|:--------------------------------|:----------|:-----------------------------------------------------|
| `strcat(s1: str, s2: str, ...)` |           | Takes _n_ number of strings and concatenates them.   |
| `lower`                         |           |                                                     |
| `upper`                         |           |                                                     |
| `snake_case`                    |           |                                                     |
| `split`                         |           |                                                     |
| `substr`                        |           |                                                     |
| `str_merge`                     |           |                                                     |
| `array_clear`                   |           |                                                     |
| `str_startswith`                |           |                                                     |
| `str_endswith`                  |           |                                                     |
| `str_join`                      |           |                                                     |

***
### Array Methods
| Handle            | Arguments | Description                                         |
|:------------------|:----------|:----------------------------------------------------|
| `array_merge`     |           |                                                     |
| `array_fill`      |           |                                                     |
| `array_zeros`     |           |                                                     |
| `array_unique`    |           |                                                     |
| `array_pop`       |           |                                                     |
| `array_push`      |           |                                                     |
| `array_insersect` |           |                                                     |
| `array_union`     |           |                                                     |
| `array_filter`    |           |                                                     |
| `array_clear`     |           |                                                     |

***
### Type
| Handle        | Arguments | Description                                         |
|:--------------|:----------|:----------------------------------------------------|
| `is_int`      |           |                                                     |
| `is_array`    |           |                                                     |
| `is_string`   |           |                                                     |
| `is_float`    |           |                                                     |
| `is_callable` |           |                                                     |
| `is_iterable` |           |                                                     |
| `is_bool`     |           |                                                     |
| `is_object`   |           |                                                     |
| `to_float`    |           |                                                     |
| `to_int`      |           |                                                     |
| `to_string`   |           |                                                     |

***
### Math
#### Builtins
| Handle      | Arguments | Description                                         |
|:------------|:----------|:----------------------------------------------------|
| `sum`       |           |                                                     |
| `pow`       |           |                                                     |
| `mod`       |           |                                                     |
| `sqrt`      |           |                                                     |
| `cbrt`      |           |                                                     |
| `randf`     |           |                                                     |
| `randint`   |           |                                                     |

***
#### `Stat` Statistics Module
~~~c++
import Stat;
~~~
| Handle   | Arguments | Description |
|:---------|:----------|:------------|
| `Stat::` |           |             |

***
#### `Rand` Random Number Module
~~~c++
import Rand;
~~~
| Handle   | Arguments | Description |
|:---------|:----------|:------------|
| `Rand::` |           |             |

***
#### `Matrix` Matrix Library
~~~c++
import Matrix;
~~~
| Handle     | Arguments | Description |
|:-----------|:----------|:------------|
| `Matrix::` |           |             |

***
#### `Opt` Optimization Library
~~~c++
import Opt;
~~~
| Handle  | Arguments | Description |
|:--------|:----------|:------------|
| `Opt::` |           |             |

***
### File
~~~c++
import File;
~~~
| Handle   | Arguments | Description |
|:---------|:----------|:------------|
| `File::` |           |             |

***
### Path
~~~c++
import Path;
~~~
| Handle   | Arguments | Description |
|:---------|:----------|:------------|
| `Path::` |           |             |

***
### Tree
~~~c++
import Tree;
~~~
| Handle   | Arguments | Description |
|:---------|:----------|:------------|
| `Tree::` |           |             |

***
### System
~~~c++
import System;
~~~
| Handle         | Arguments | Description |
|:---------------|:----------|:------------|
| `System::exec` |           |             |