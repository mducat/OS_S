
import sys
import os.path

print()
print('#ifndef _FILES_H')
print('#define _FILES_H')
print()

passed = False
bins = 0
text = 0

files = {}

for v in sys.argv[1:]:

    if v == 'and':
        passed = True
        continue

    if passed:
        with open(v, 'r') as f:
            data = f.readlines()
            fname = f'plain{text}'
            print(f'static const char {fname}[] = ', end='\n')

            for line in data[:-1]:
                line = line.replace('"', '\"')[:-1]
                print(f'    "{line}\\n" \\')

            line = data[-1].replace('"', '\"')[:-1]
            print(f'    "{line}\\n";')

            print()

            text += 1
            files[os.path.basename(v)] = fname
        continue
    
    with open(v + '/' + v, 'rb') as f:
        data = f.read()
        data = [str(v) for v in data]
        fname = f'bin{bins}'

        print(f'static const char {fname}[] = ', end='{')
        print(', '.join(data), end='};\n\n')

        bins += 1
        files[os.path.basename(v)] = fname

varlist = [f'{{\"{k}\", {v}}}' for k, v in files.items()]

print('static const char **files[] = {', end='\n    ')
print(',\n    '.join(varlist))
print('};\n')

print('#endif')
