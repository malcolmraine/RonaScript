

class VM(object):
    def __init__(self, instructions, stack, scope):
        self.stack = stack
        self.instructions = instructions
        self.scope = scope
        self.idx = 0

        self.bin_op_map = {
            'add': self._add,
            'sub': self._sub,
            'div': self._div,
            'mul': self._mul,
            'and': self._and,
            'or': self._or,
            'eq': self._eq,
            'geq': self._geq,
            'leq': self._leq,
            'gt': self._gt,
            'lt': self._lt,
            'xor': self._xor,
            'neq': self._neq,
        }

    def binary_op(self):
        a = self.stack.pop()
        b = self.stack.pop()
        c = self.bin_op_map[self.idx](a, b)
        self.stack.push(c)

    @staticmethod
    def _add(a, b):
        return a + b

    @staticmethod
    def _sub(a, b):
        return a - b

    @staticmethod
    def _div(a, b):
        return a / b

    @staticmethod
    def _mul(a, b):
        return a * b

    @staticmethod
    def _and(a, b):
        return a & b

    @staticmethod
    def _or(a, b):
        return a | b

    @staticmethod
    def _xor(a, b):
        return a ^ b

    @staticmethod
    def _eq(a, b):
        return a == b

    @staticmethod
    def _leq(a, b):
        return a <= b

    @staticmethod
    def _geq(a, b):
        return a >= b

    @staticmethod
    def _gt(a, b):
        return a > b

    @staticmethod
    def _lt(a, b):
        return a < b

    @staticmethod
    def _neq(a, b):
        return a != b
