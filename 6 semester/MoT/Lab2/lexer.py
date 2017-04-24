import re
import ply.lex as lex

tokens = [
    'NUMBER',
    'PLUS',
    'MINUS',
    'MULT',
    'DIVIDE',
    'LPAREN',
    'RPAREN',
    'ID',
    'COMMA',
    'DOT',
    'SEMICOLON',
    'LEFTBRACE',
    'RIGHTBRACE',
    'LSQBRACE',
    'RSQBRACE',
    'ASSIGN',
    'EQUAL',
    'TERNARY',
    'STRING',
    'GT',
    'LT',
    'GOE',
    'LOE',
    'AND',
    'MOD',
    'INC'
]

reserved = {
    'while': 'WHILE',
    'else': 'ELSE',
    'if': 'IF',
    'for': 'FOR',
    'switch': 'SWITCH',
    'case': 'CASE',
    'do': 'DO',
    'break': 'BREAK',
    'return': 'RETURN',
    'int': 'INT',
    'float': 'FLOAT',
    'double': 'DOUBLE',
    'string': 'STRING_TYPE',
    'continue': 'CONTINUE',
    'struct': 'STRUCT',
    'union': 'UNION',
    'char': 'CHAR',
    'printf': 'PRINTF',
    'scanf': 'SCANF',
    'typedef': 'TYPEDEF',
    'Point': 'POINT'
}
tokens += reserved.values()

t_CONTINUE = r'continue'
t_CASE = r'case'
t_ELSE = r'else'
t_BREAK = r'break'
t_INT = r'int'
t_SCANF = r'scanf'
t_UNION = r'union'
t_PRINTF = r'printf'
t_CHAR = r'char'
t_ASSIGN = r'='
t_EQUAL = r'=='
t_LEFTBRACE = r'{'
t_RIGHTBRACE = r'}'
t_LSQBRACE = r'\['
t_RSQBRACE = r'\]'
t_PLUS = r'\+'
t_MINUS = r'-'
t_MULT = r'\*'
t_DIVIDE = r'/'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_COMMA = r','
t_DOT = r'\.'
t_SEMICOLON = r';'
t_FOR = r'for'
t_WHILE = r'while'
t_SWITCH = r'switch'
t_STRUCT = r'struct'
t_RETURN = r'return'
t_IF = r'if'
t_DO = r'do'
t_FLOAT = r'float'
t_DOUBLE = r'double'
t_STRING_TYPE = r'string'
t_TERNARY = r'\?[\s\S]*[:]'
t_STRING = r'\".*\"'
t_GT = r'>'
t_LT = r'<'
t_GOE = r'>='
t_LOE = r'<='
t_AND = r'&&'
t_MOD = r'%'
t_INC = r'\+\+'
t_POINT = r'Point'


def t_ID(t):
    r'[a-zA-Z_][a-zA-Z0-9_]*'
    if t.value in reserved:
        t.type = reserved[t.value]
    return t


def t_NUMBER(t):
    r'\d+'
    try:
        t.value = int(t.value)
    except ValueError:
        print("Line %d: Number %s is too large!", (t.lineno, t.value))
        t.value = 0
    return t


def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)


t_ignore = ' \t'


def t_error(t):
    print("Illegal character '%s'", t.value[0])
    t.lexer.skip(1)


def t_comment(t):
    r'(/\*(.|\n)*?\*/)|(//[^\n]*)'
    pass


def find_column(input, token):
    last_cr = input.rfind('\n', 0, token.lexpos)
    if last_cr < 0:
        last_cr = 0
    column = (token.lexpos - last_cr) + 1
    return column - 1


lexer = lex.lex(reflags=re.UNICODE | re.DOTALL | re.IGNORECASE)
