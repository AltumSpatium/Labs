import re
import ply.lex as lex

id_table = []

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
    'typedef': 'TYPEDEF',
    'Point': 'POINT'
}
tokens += reserved.values()

t_CONTINUE = r'continue'
t_CASE = r'case'
t_ELSE = r'else'
t_BREAK = r'break'
t_INT = r'int'
t_UNION = r'union'
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
t_STRING = r'\"[^\n]*\"'
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
    token = '{0}, line: {1}, col: {2}'.format(t.value, t.lineno, find_column(t.lexer.lexdata, t) - 1)
    print("Illegal character:", token)
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


def create_id_table():
    global id_table
    errors = []
    data = open('main.c').read()
    lines = data.split('\n')
    
    lexer.input(data)

    while True:
        tok = lexer.token()
        if not tok: break

        if tok.type == 'ID':
            tok_line = tok.lineno - 1
            if lines[tok_line].find('int') != -1:
                if tok.value in id_table and lines[tok_line].find('=') == -1 and tok.value != 'printf':
                    error_str = 'Identifier already exists: {0}, line: {1}, col: {2}'\
                        .format(tok.value, tok_line + 1, lines[tok_line].find(tok.value))
                    errors.append(error_str)
                else:
                    id_table.append(tok.value)
    return (set(id_table), errors)
