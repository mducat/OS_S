
import sys
import os.path

print()
print('#ifndef _FILES_RAW_H')
print('#define _FILES_RAW_H')
print()

passed = False
bins = 0
text = 0

files = {}
sizes = []

for v in sys.argv[1:]:

    if v == 'and':
        passed = True
        continue

    if passed:
        with open(v, 'r') as f:
            data = f.readlines()
            fname = f'plain{text}'
            csize = 0
            print(f'static char {fname}[] = ', end='\n')

            for line in data[:-1]:
                line = line.replace('"', '\\\"')[:-1]
                print(f'    "{line}\\n" \\')
                csize += len(line) + 1

            line = data[-1].replace('"', '\\\"').replace('\n', '')
            print(f'    "{line}\\n";')
            csize += len(line) + 1

            print()

            text += 1
            files[os.path.basename(v)] = fname
            sizes += [csize]
        continue
    
    with open(v + '/' + v, 'rb') as f:
        data = b'OSS' + f.read()
        data = [str(v) for v in data]
        fname = f'bin{bins}'

        print(f'static char {fname}[] = ', end='{')
        print(', '.join(data), end='};\n\n')

        bins += 1
        files[os.path.basename(v)] = fname
        sizes += [len(data)]

varlist = [str(v) for k, v in files.items()] + ['0']
varname = ["\""+str(k)+"\"" for k, v in files.items()] + ['0']
varsize = [str(v) for v in sizes] + ['0']

print('static char *f_raw[] = {', end='\n    ')
print(',\n    '.join(varlist))
print('};\n')

print('static char *f_nam[] = {', end='\n    ')
print(',\n    '.join(varname))
print('};\n')

print('static const int f_siz[] = {', end='\n    ')
print(',\n    '.join(varsize))
print('};\n')

print('#endif')
