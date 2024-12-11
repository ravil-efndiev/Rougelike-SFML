import sys

if len(sys.argv) < 2:
    print("usage: ", sys.argv[0], ' "<yaml property>"')
    exit(1)

lines = []
with open("test_map.yml", "r") as file:
    lines = file.readlines()

output_lines = []

for line in lines:
    output_lines.append(line)
    if "name:" in line:
        output_lines.append("    " + sys.argv[1] + "\n")

with open("test_map.yml", "w") as file:
    file.writelines(output_lines)
