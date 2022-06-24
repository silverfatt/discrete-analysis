import random 
import sys    
import string

def generate_random_string(length):
    letters = string.ascii_lowercase
    rand_string = ''.join(random.choice(letters) for i in range(length))
    return rand_string

File = open('tests.txt', 'w')


pattern_length = int(sys.argv[1])
text_length = int(sys.argv[2])
pattern = ""
text = ""
# Generate pattern
for i in range(pattern_length):
    pattern+=generate_random_string(random.randint(0,16))
    pattern+=" "

# Generate text
for i in range(text_length):
    text += generate_random_string(random.randint(0, 16))
    i = random.randint(0,5)
    if i == 0:
        text += "\n"
    else: 
        text += " "
File.write(pattern + "\n")
File.write(text + "\n")
File.close()

