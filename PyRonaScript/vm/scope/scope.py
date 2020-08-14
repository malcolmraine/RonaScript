

class Scope(object):
    def __init__(self):
        self.id = None
        self.parent = None
        self.variables = []
        self.functions = []
        self.scopes = []

    def create_var(self, name):
        ...

    def create_func(self, name, body):
        ...

    def create_subscope(self):
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

