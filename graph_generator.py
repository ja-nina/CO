from problem import Problem
from reader import Reader
from math import ceil
from math import log2
input_directory = "data"
output_directory = "graph_representations"
files = ["a","b","c","d","e","f"]
for name in files:
    path = "{}/{}.txt".format(input_directory, name)
    outputPath = "{}/{}_graph_rep.csv".format(output_directory, name)
    print(path, outputPath)
    my_problem = Reader.read(path)
    with open(outputPath, 'w') as graphFile:
        lines= []
        lines.append("Source;Target;Weight")
        for edge in my_problem.Streets:
            lines.append(f"{edge.start};{edge.end};{(1/(1+log2(edge.length)))}")
        graphFile.writelines(["{}\n".format(line) for line in lines])
        graphFile.close()

#