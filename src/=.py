#!/usr/bin/python3

import sys
from math import *

help = "\n".join( [
    "Python command line calculator v1.0",
    " Example: = \"2 * 3 + 7\"",
    " help - return this help screen",
    " list - return list of functions"
] )

def func_list():
    return "Function list:\n " + "\n ".join( access['local'].keys() )

access = {
    'global': {
        '__builtins__': None
    },

    'local': {
        'sqrt': sqrt, 
        'acos': acos, 
        'acosh': acosh, 
        'asin': asin, 
        'asinh': asinh, 
        'atan': atan, 
        'atan2': atan2, 
        'atanh': atanh, 
        'ceil': ceil, 
        'cos': cos, 
        'cosh': cosh, 
        'degrees': degrees, 
        'e': e, 
        'erf': erf, 
        'erfc': erfc, 
        'exp': exp, 
        'expm1': expm1, 
        'fabs': fabs, 
        'factorial': factorial, 
        'floor': floor, 
        'fmod': fmod, 
        'frexp': frexp, 
        'fsum': fsum, 
        'gamma': gamma, 
        'gcd': gcd, 
        'hypot': hypot, 
        'inf': inf, 
        'isclose': isclose, 
        'isfinite': isfinite, 
        'isinf': isinf, 
        'isnan': isnan, 
        'ldexp': ldexp, 
        'lgamma': lgamma, 
        'log': log, 
        'log10': log10, 
        'log1p': log1p, 
        'log2': log2, 
        'modf': modf, 
        'nan': nan, 
        'pi': pi, 
        'pow': pow, 
        'radians': radians, 
        'sin': sin, 
        'sinh': sinh, 
        'tan': tan, 
        'tanh': tanh, 
        'tau': tau, 
        'trunc': trunc,
        'help': help,
        'list': func_list
    }
}

try:
    print( eval( ' '.join( sys.argv[1:] ).lower().replace("list", "list()"), access['global'], access['local'] ) )
except:
    print("Syntax error! Use 'help' for more info.")

