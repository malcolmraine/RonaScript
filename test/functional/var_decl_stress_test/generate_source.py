import random
import string


def make_rand_string():
    return "\"" + "".join(random.choices(string.ascii_uppercase + string.digits, k=100)) + "\""


def make_rand_float():
    return random.uniform(-100000, 100000)


def make_rand_int():
    return random.randint(-100000, 100000)


def make_rand_bool():
    return random.choice(["true", "false"])


type_generators = {
    "float": make_rand_float,
    "int": make_rand_int,
    "string": make_rand_string,
    "array": None,
    "bool": make_rand_bool,
}


def make_rand_decl():
    t = random.choice([key for key in type_generators.keys()])
    v = type_generators[t]()
    return t, v


def make_rand_array():
    result = []
    for r in range(random.randint(0, 5)):
        result.append(make_rand_float())
        result.append(make_rand_bool())
        result.append(make_rand_int())
        result.append(make_rand_string())
    return "[" + ", ".join([str(_) for _ in result]) + "]"


if __name__ == "__main__":
    type_generators["array"] = make_rand_array
    with open("source.rn", "w+") as f:
        for n in range(0, 5000):
            t, v = make_rand_decl()
            f.write(f"var test_{n}: {t} = {v};\n")
        f.write("print(\"Finished\");")