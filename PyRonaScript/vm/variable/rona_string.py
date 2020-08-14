from .rona_var import RonaVar


class RonaString(RonaVar):
    def __str__(self):
        super(self).__init__()

    def length(self):
        return len(self.value)

    def clear(self):
        self.value = ""
