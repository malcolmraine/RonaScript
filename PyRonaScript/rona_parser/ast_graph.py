

class AstGraph(object):
    def __init__(self):
        self.edges = []
        self.name = "Program"

    def add_edge(self, a, b) -> None:
        """
        Add an edge from node a to node b.
        a->b

        :param a: Parent node
        :param b: Child node
        :return: No return value
        """
        self.edges.append((a, b))

    def to_dot(self) -> str:
        """
        Converts the list of edge tuples to dot graph notation.

        :return: String containing dot notation version of graph.
        """
        out = f"Digraph {self.name} " + "{\n"

        for edge in self.edges:
            out += f"{edge[0]} -> {edge[1]};\n"

        out += f"{self.name} [shape=Mdiamond];\n"

        return out
