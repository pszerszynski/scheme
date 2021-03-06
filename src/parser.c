#include "parser.h"

scheme_object * Parser_Parse(struct lexer * lex) {
	Lexer_NextToken(lex); // get first token
	return Parser_ParseExpression(lex);
}

scheme_object * Parser_ParseExpression(struct lexer * lex) {
	switch (Lexer_CurrToken(lex)) {
	case TOKEN_EOF:
		return NULL;
	case TOKEN_SYMBOL:
		return Parser_ParseSymbol(lex);
	case TOKEN_NUMBER:
		return Parser_ParseNumber(lex);
	case TOKEN_BOOLEAN:
		return Parser_ParseBoolean(lex);
	case TOKEN_STRING:
		return Parser_ParseString(lex);
	case '(':
		return Parser_ParseList(lex);
	case '\'':
		return Parser_ParseQuote(lex);
	default:
		Scheme_SetError("unexpected token");
		return NULL;
	}
}

scheme_object * Parser_ParseNumber(struct lexer * lex) {
	scheme_object * obj;

	switch (lex->number_type) {
	case NUMBER_INTEGER:
		obj = Scheme_CreateInteger(lex->integer_val);
		break;
	case NUMBER_RATIONAL:
		obj = Scheme_CreateRational(lex->numerator, lex->denominator);
		break;
	case NUMBER_DOUBLE:
		obj = Scheme_CreateDouble(lex->double_val);
		break;
	}

	return obj;
}

scheme_object * Parser_ParseBoolean(struct lexer * lex) {
	scheme_object * obj = Scheme_CreateBoolean(lex->bool_val);
	return obj;
} 

scheme_object * Parser_ParseSymbol(struct lexer * lex) {
	scheme_object * obj = Scheme_CreateSymbol(lex->symbol);

	return obj;
}

scheme_object * Parser_ParseString(struct lexer * lex) {
	scheme_object * obj = Scheme_CreateString(lex->string);

	return obj;
}

#include "scheme.h"

scheme_object * Parser_ParseList(struct lexer * lex) {
	int token = Lexer_NextToken(lex); // eat '(' token
	if (token == ')')
		return NULL; // empty list

	scheme_object * first_object = Parser_ParseExpression(lex);
	
	scheme_object * base_pair = Scheme_CreatePairWithoutRef(first_object, NULL);
	scheme_object * next_pair = base_pair;
	
	while (Lexer_NextToken(lex) != ')') {
		if (Lexer_CurrToken(lex) == TOKEN_EOF) {
			Scheme_SetError("unexpected EOF");
			return NULL;
		}

		scheme_object * new_object = Parser_ParseExpression(lex);

		scheme_pair * pair = (scheme_pair *)next_pair->payload;
		pair->cdr = Scheme_CreatePairWithoutRef(new_object, NULL);

		next_pair = pair->cdr;
	}

	return base_pair;
}

scheme_object * Parser_ParseQuote  (struct lexer * lex) {
	Lexer_NextToken(lex); // eat ' token

	scheme_object * to_quote = Parser_ParseExpression(lex);
	if (to_quote == NULL)
		return NULL;

	scheme_object * quote_symbol = Scheme_CreateSymbolLiteral("quote");

	scheme_object * p2 = Scheme_CreatePairWithoutRef(to_quote, NULL);
	scheme_object * p1 = Scheme_CreatePairWithoutRef(quote_symbol, p2);

	return p1;
}
