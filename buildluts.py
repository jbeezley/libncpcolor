'''
Preprocessing script to generate header files from binary lookup tables in "luts/".
Also creates header containing all of these headers under the namespace lut::
with a struct array call lookupTables that can be used to query and access the lookup
tables found.
'''

from glob import glob

import bin

if __name__ == '__main__':

    files = glob('luts/*')
    lutHeader = open('lutData.h', 'w')

    headers = []

    for file in files:
        if file[:-2] != '.h':
            dataname, filename = bin.Bin2Header( ('',file) )
            headers.append((dataname, filename))
    
    lutHeader.write('''
namespace lut {

''')

    for dataname, filename in headers:
        lutHeader.write('#include "%s"\n' % filename)

    lutHeader.write('''
    static const int NTables = %i;
    struct lutItem {
        const char *name;
        const unsigned char* data;
    };
    static const struct lutItem lookupTables[] =
            {

''' % len(headers))
    headersStr = []
    for dataname, filename in headers:
        headersStr.append('                {"%s", %s}' % (dataname, dataname))
    lutHeader.write(',\n'.join(headersStr))
    lutHeader.write('''
            };
}
''')

