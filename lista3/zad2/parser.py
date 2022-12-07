from sly import Parser
from lexer import FieldLexer
import field

class FieldParser(Parser):
    debugfile = 'debug.txt'
    tokens = FieldLexer.tokens
    precedence = (('left', "-", "+"),
                  ('left', "*", "/"),
                  ('right', "^"),
                  ('right', UMINUS)) 


    order = field.ORDER
    result_rpn = ""
    error_message = ""


    ################
    ## Drukowanie ##
    ################

    @_('expression')
    def line(self, p):
        if self.result_rpn != "":
            print("|-> RPN   : " + self.result_rpn)
            print("\\-> Wynik : " + str(p.expression))

        self.result_rpn = ""


    @_('COMMENT')
    def line(self, p):
        pass



    ####################
    ## Obsługa błędów ##
    ####################

    def error(self, p):
        if self.error_message == "":
            self.error_message = "Błąd składni."
        
        print("ERROR: " + self.error_message)
        self.result_rpn = ""
        self.error_message = ""



    ############################
    ## Wyrażenia arytmetyczne ##
    ############################

    @_('expression "+" expression')
    def expression(self, p):
        self.result_rpn += "+ "
        return field.f_add(p.expression0, p.expression1, self.order)
        

    @_('expression "-" expression')
    def expression(self, p):
        self.result_rpn += "- "
        return field.f_sub(p.expression0, p.expression1, self.order)


    @_('expression "*" expression')
    def expression(self, p):
        self.result_rpn += "* "
        return field.f_mul(p.expression0, p.expression1, self.order)


    @_('expression "/" expression')
    def expression(self, p):
        if p.expression1 == 0:
            self.error_message += "Dzielenie przez 0."
            self.error(p.expression1)
        else:
            self.result_rpn += "/ "
            return field.f_div(p.expression0, p.expression1, self.order)
    

    @_('"(" expression ")"')
    def expression(self, p):
        return p.expression


    @_('"-" "(" expression ")" %prec UMINUS')
    def expression(self, p):
        self.result_rpn += "~ "
        return field.f_sub(0, p.expression, self.order)



    #################
    ## Potęgowanie ##
    #################

    @_('exponent')
    def expression(self, p):
        return p.exponent


    @_('expression power expression')
    def exponent(self, p):
        self.result_rpn += "^ "
        self.order = field.ORDER
        return field.f_pow(p.expression0, p.expression1, self.order)

    @_('"^"')
    def power(self, p):
        self.order = field.ORDER - 1
        return



    #######################
    ## Wartości liczbowe ##
    #######################


    @_('"-" NUM %prec UMINUS')
    def expression(self, p):
        value = field.f_sub(0, p.NUM, self.order)
        self.result_rpn += str(value) + " "
        return value


    @_('NUM')
    def expression(self, p):
        value = p.NUM % self.order
        self.result_rpn += str(value) + " "
        return value
        