# RonaScript
RonaScript is an object oriented scripting language that's influenced by Python, C++, and Ada.

## Example
~~~ objective-c++

class Person is
begin
    var first_name: string = "";
    var middle_name: string = "";
    var last_name: string = "";
    var age: int = 0;
    
    routine construct(var full_name: string, var age: int): void
    begin
        var name_parts: array = str_split(full_name, " ");
        this->first_name = name_parts[0];
        this->middle_name = name_parts[1];
        this->last_name = name_parts[2];
        this->age = age;
    end // routine construct
    
    routine GetNameLastFirst(): string
    begin
        return strcat(this->last_name, ", ", 
                      this->first_name,  " ", 
                      this->middle_name);
    end // routine GetNameLastFirst
    
end // class Person
~~~

## Compiling RonaScript From Source
RonaScript can be built using a single build script, as it currently does not have any third-party dependencies.
~~~
./scripts/build.sh

Usage: bash build.sh <command> [options...]
  help,-h,--help            Show help.
  -t, --target <option>     Specify the build target.
      build                     Build RonaScript.
      clean                     Clean the project.
      install                   Install the project.
      rebuild                   Rebuild the entire project.

~~~
From the project directory, run the following:
~~~ commandline
bash ./scripts/build.sh --target build
~~~

## Running RonaScript
*Note: some options may not be available yet as this is in active development.*
~~~
Usage: RonaScript <file> [options...]
  <file>              Input file (*.rn | *.rnc)
  -c                  Compile to *.rnc file
  -norun              Compile to *.rnc file without running
  -v, --version       Show version information
  -h, --help          Show help

~~~