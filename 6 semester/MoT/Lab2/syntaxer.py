from lexer import tokens
from lexer import find_column
import ply.yacc as yacc


class Node:
    errors = []

    def __init__(self, type, parts):
        self.type = type
        self.parts = parts


    def parts_str(self):
        st = []
        for part in self.parts:
            st.append( str( part ) )
        return "\n".join(st)


    def add_parts(self, parts):
        if isinstance(parts, Node):
            if parts.parts:
                self.parts.extend(parts.parts)
        return self


    def __repr__(self):
        return self.type + ":\n|  " + self.parts_str().replace("\n", "\n|  ")


def p_start(p):
    """start : expression"""
    p[0] = p[1]


def p_block(p):
    """block : LEFTBRACE body RIGHTBRACE"""
    p[0] = Node('block', [p[2]])


def p_body(p):
    """body : expression"""
    p[0] = Node('body', [p[1]])


def p_expression(p):
    """expression : declaration expression
        | declaration_with_assignment expression
        | assignment expression
        | if expression
        | else expression
        | function expression
        | empty
        | while expression
        | for expression
        | return expression
        | BREAK SEMICOLON
    """
    if len(p) == 2:
        p[0] = Node('expression', [p[1]])
    else:
        p[0] = Node('expression', [p[1]]).add_parts(p[2])


def p_declaration(p):
    """declaration : type ID SEMICOLON
        | type function_declaration
        | type declaration_comma SEMICOLON"""
    p[0] = Node('declaration', [p[1], p[2]])


def p_function_declaration(p):
    """function_declaration : ID LPAREN simple_declaration RPAREN SEMICOLON
        | ID LPAREN simple_declaration RPAREN block"""
    p[0] = Node('function', [p[1], p[3]]).add_parts(p[5])


def p_simple_declaration(p):
    """simple_declaration : type ID COMMA simple_declaration
        | type ID"""
    if len(p) == 5:
        p[0] = Node('declaration', [p[1], p[2]]).add_parts(p[4])
    else:
        p[0] = Node('declaration', [p[1], p[2]])


def p_declaration_comma(p):
    """declaration_comma : ID COMMA declaration_comma
        | ID
        | assignment COMMA declaration_comma
        | assignment"""
    if len(p) == 1:
        p[0] = ''
    elif len(p) == 4:
        p[0] = Node('variables', [p[1]]).add_parts(p[3])
    else:
        p[0] = Node('variables', [p[1]])


def p_declaration_with_assignment(p):
    """declaration_with_assignment : type ID ASSIGN arithmetic_expression SEMICOLON
        | type ID ASSIGN LEFTBRACE func_params RIGHTBRACE SEMICOLON
        | type ID LSQBRACE arithmetic_expression RSQBRACE ASSIGN arithmetic_expression SEMICOLON
        | type ID LSQBRACE arithmetic_expression RSQBRACE ASSIGN LEFTBRACE func_params RIGHTBRACE SEMICOLON"""
    if len(p) == 6:
        p[0] = Node('decl_with_assign', [p[1], p[2], p[3], p[4]])
    elif len(p) == 9:
        p[0] = Node('decl_with_assign_on_index', [p[1], p[2], p[4], p[6], p[7]])
    elif len(p) == 11:
        p[0] = Node('decl_with_assign_on_index', [p[1], p[2], p[4], p[6], p[8]])
    else:
        p[0] = Node('decl_with_assign', [p[1], p[2], p[3], p[5]])


def p_assignment(p):
    """assignment : ID ASSIGN arithmetic_expression SEMICOLON
        | ID ASSIGN arithmetic_expression
        | ID ASSIGN LEFTBRACE declaration_comma RIGHTBRACE SEMICOLON
        | ID ASSIGN LEFTBRACE func_params RIGHTBRACE SEMICOLON
        | ID LSQBRACE arithmetic_expression RSQBRACE ASSIGN arithmetic_expression SEMICOLON
        | ID LSQBRACE arithmetic_expression RSQBRACE ASSIGN LEFTBRACE func_params RIGHTBRACE SEMICOLON"""
    if len(p) == 7:
        p[0] = Node('assignment', [p[1], p[2], p[4]])
    else:
        p[0] = Node('assignment', [p[1], p[2]]).add_parts(p[3])


def p_type(p):
    """type : INT
        | FLOAT
        | DOUBLE
        | STRING_TYPE
        | POINT"""
    p[0] = Node('type', [p[1]])


def p_empty(p):
    """empty :"""
    p[0] = Node('', [])


def p_func_params(p):
    """func_params : 
        | ID
        | NUMBER
        | ID COMMA func_params
        | NUMBER COMMA func_params
        | arithmetic_expression
        | arithmetic_expression COMMA func_params"""
    if len(p) == 1:
        p[0] = ''
    elif len(p) == 4:
        p[0] = Node('params', [p[1]]).add_parts(p[3])
    else:
        p[0] = Node('params', [p[1]])


def p_arithmetic_expression(p):
    """arithmetic_expression : ID PLUS arithmetic_expression
        | ID MINUS arithmetic_expression
        | ID MULT arithmetic_expression
        | ID DIVIDE arithmetic_expression
        | ID MOD arithmetic_expression
        | ID PLUS PLUS arithmetic_expression
        | ID LPAREN func_params RPAREN
        | ID INC
        | ID DOT ID
        | arithmetic_expression DOT ID
        | arithmetic_expression PLUS arithmetic_expression
        | arithmetic_expression MINUS arithmetic_expression
        | arithmetic_expression MULT arithmetic_expression
        | arithmetic_expression DIVIDE arithmetic_expression
        | ID LSQBRACE arithmetic_expression RSQBRACE
        | LPAREN arithmetic_expression RPAREN
        | NUMBER PLUS arithmetic_expression
        | NUMBER MINUS arithmetic_expression
        | NUMBER MULT arithmetic_expression
        | NUMBER DIVIDE arithmetic_expression
        | NUMBER MOD arithmetic_expression
        | NUMBER
        | MINUS NUMBER
        | ID
        | MINUS ID
        | STRING"""
    if len(p) == 2:
        p[0] = Node('expression', [p[1]])
    elif len(p) == 5:
        type = 'call' if p[2] == '(' else 'take_on_index'
        p[0] = Node(type, [p[1], p[3]])
    elif len(p) == 3:
        p[0] = Node('expression', [p[1], p[2]])
    else:
        p[0] = Node('expression', [p[1], p[2], p[3]])


def p_if(p):
    """if : IF LPAREN predicate RPAREN block"""
    p[0] = Node('if', [p[3]]).add_parts(p[5])


def p_else(p):
    """else : ELSE block"""
    p[0] = Node('else', [p[2]])


def p_predicate(p):
    """predicate : ID EQUAL arithmetic_expression
        | NUMBER EQUAL arithmetic_expression
        | STRING EQUAL arithmetic_expression
        | ID GT arithmetic_expression
        | ID LT arithmetic_expression
        | ID GOE arithmetic_expression
        | ID LOE arithmetic_expression
        | NUMBER GT arithmetic_expression
        | NUMBER LT arithmetic_expression
        | NUMBER GOE arithmetic_expression
        | NUMBER LOE arithmetic_expression
        | arithmetic_expression EQUAL arithmetic_expression
        | ID
        | NUMBER
        | STRING
        | predicate AND predicate"""
    if len(p) == 2:
        p[0] = Node('condition', [p[1]])
    else:
        p[0] = Node('condition', [p[1], p[2], p[3]])


def p_while(p):
    """while : WHILE LPAREN predicate RPAREN block"""
    p[0] = Node('while', [p[3]]).add_parts(p[5])


def p_for(p):
    """for : FOR LPAREN assignment predicate SEMICOLON arithmetic_expression RPAREN block"""
    p[0] = Node('for', [p[3], p[4], p[6]]).add_parts(p[8])


def p_return(p):
    """return : RETURN arithmetic_expression SEMICOLON"""
    p[0] = Node('return', [p[2]])


def p_function(p):
    """function : ID LPAREN arithmetic_expression RPAREN SEMICOLON
        | ID LPAREN arithmetic_expression RPAREN block"""
    p[0] = Node('function', [p[1], p[3]])


def p_error(p):
    if p:
        token = "{0}, line: {1}, col: {2}".format(p.value, p.lineno, find_column(p.lexer.lexdata, p))
    else:
        token = p
    error_str = 'Unexpected token: {0}'.format(token)
    Node.errors.append(error_str)


parser = yacc.yacc()


def build_tree(code):
    return parser.parse(code)
