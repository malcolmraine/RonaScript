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
