from sly import Lexer

class FieldLexer(Lexer):
    tokens = {NUM, COMMENT}
    ignore = ' \t'
    literals = {"+", "-", "*", "/", "(", ")", "^"}


    COMMENT = r'\#.*'


    @_(r'\d+')
    def NUM(self, token):
        token.value = int(token.value)
        return token

    def error(self, token):
        self.index += 1