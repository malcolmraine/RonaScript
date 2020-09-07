from rona_stack.stack import Stack
from .vm import VM


class Function(VM):
    def __init__(self, instructions, stack, scope):
        super().__init__(instructions, stack, scope)
        self.instructions = []
        self.stack = Stack()
        self.scope = None
        self.return_var = None
