import time
import re
with open('text.txt', 'r') as file:
    load_set_start = time.time()
    dataset = {x.strip(' \t\n\r') for x in file if not x.startswith(';') and not x == ''}
    load_set_end = time.time()
    file.seek(0)
    load_list_start = time.time()
    datalist = [x.strip(' \t\n\r') for x in file if not x.startswith(';')] 
    load_list_end = time.time()
    
x = 'hate'
set_start = time.time()
if x in dataset:
    print('set')
set_end = time.time()

list_start = time.time()
if x in datalist:
    print('list')
list_end = time.time()
print('set search time: {:.2e}, load time: {:.2e}, \nlist search time: {:.2e} load time:{:.2e}'.format(set_end - set_start, list_end - list_start, load_set_end - load_set_start, load_list_end - load_list_start))    
print(dataset)
print(datalist)

