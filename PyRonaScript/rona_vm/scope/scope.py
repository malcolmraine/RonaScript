

class Scope(object):
    def __init__(self):
        self.id = None
        self.parent: Scope = None
        self.variables = {}
        self.functions = {}
        self.scopes = []

    def create_var(self, name, var_type=None, uuid=None):
        ...

    def create_func(self, name, body):
        ...

    def create_subscope(self):
        new_scope = Scope()
        self.scopes.append(new_scope)
        new_scope.parent = self
        return new_scope

    def resolve_identifier(self, identifier):
        if identifier in self.variables:
            return self.variables[identifier]

        elif identifier in self.functions:
            return self.functions[identifier]

        else:
            return self.parent.resolve_identifier(identifier)

    def delete(self):
        ...


class Variable(object):
    def __init__(self, name, type, value=None):
        self.name = name
        self.type = type
        self.value = value


class Function(object):
    def __init__(self, name, body):
        self.name = name
        self.body = body
        self.scope = Scope()

