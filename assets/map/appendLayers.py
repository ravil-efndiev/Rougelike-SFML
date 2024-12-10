lines = []
with open("test_map.yml", "r") as file:
    lines = file.readlines()

output_lines = []

for i, line in enumerate(lines):
    output_lines.append(line)
    if "name:" in line:
        output_lines.append("    layer: layout\n")

with open("test_map.yml", "w") as file:
    file.writelines(output_lines)
