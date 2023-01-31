

with open("source.rn", "w+") as f:
    for n in range(0, 10000):
        f.write(f"var test_{n}: int = 99;\n")
    f.write("print(\"Finished\");")