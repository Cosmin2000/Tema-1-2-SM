from numpy import random
import filecmp
import subprocess
import difflib


n_array = [100, 100000]

for i in range(len(n_array)):

    x=random.randint(1000, size=(n_array[i]))

    file = open("./input/input" + str(i + 1), "w+")

    file.write(str(n_array[i]) +'\n')

    for num in x:
        file.write(str(num) + ' ')

    # Saving the array in a text file
    file.close()

    x.sort()


    file = open("./ref/ref" + str(i + 1), "w+")

    for num in x:
        file.write(str(num) + ' ')

    # Saving the array in a text file
    file.close()
