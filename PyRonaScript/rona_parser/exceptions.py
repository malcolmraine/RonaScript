

class RonaException(Exception):
    def __init__(self, file_pos: tuple):
        self.file_pos = file_pos
        self.message = ""


class InvalidIdentifier(RonaException):
    def __init__(self, identifier, file_pos: tuple):
        super().__init__(file_pos)
        self.identifier = identifier
        self.message = f"Invalid identifier '{self.identifier}' at {self.file_pos[0]}:{self.file_pos[0]}:{self.file_pos[0]}"
