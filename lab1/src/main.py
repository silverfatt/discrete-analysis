import random

my_file = open("tests.txt", "w")
my_file.write(str(2**64-1)+"\n")
for i in range(0, 10):
    my_file.write(str(random.randint(0,2**64 - 1)) + " " + 'a'*random.randint(1, 2048) + "\n")
my_file.close()