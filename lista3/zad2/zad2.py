from lexer import FieldLexer
from parser import FieldParser


if __name__ == '__main__':
    lexer = FieldLexer()
    parser = FieldParser()

    while True:
        try:
            line = input("\n>> ")
        except error:
            break

        if line:
            try:
                parser.parse(lexer.tokenize(line))
            except:
                continue