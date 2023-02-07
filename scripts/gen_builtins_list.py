import os
import re
from glob import glob

handle_rgx = re.compile(r"void rn_builtin_([a-zA-Z0-9_]+)\(")


def generate_list() -> dict:
    functions = {}
    for file in glob("../src/builtins/*.h"):
        functions[os.path.basename(file).strip(".h")] = []
        with open(file, "r") as header:
            functions[os.path.basename(file).strip(".h")].extend(handle_rgx.findall(header.read()))
    return functions


def generate_registry(function_map: dict) -> list:
    registry = []
    for namespace in function_map:
        functions = function_map[namespace]
        for f in functions:
            registry.append("{" + f"\"{f}\", CastToBuiltin(&{namespace}::rn_builtin_{f}), RnType::RN_VOID" + "},")
    return registry


if __name__ == "__main__":
    map = generate_list()
    for item in generate_registry(map):
        print(item)
