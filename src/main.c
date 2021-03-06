#include "parser.h"
#include "scheme.h"

void test_lexer(struct lexer * lex) {
	int token;

	scheme_object * obj;

	while ((token = Lexer_NextToken(lex)) != TOKEN_EOF) {
		switch (token) {
		case TOKEN_STRING:
			obj = Scheme_CreateString(lex->string);
			Scheme_Display(obj);
			Scheme_Newline();
			Scheme_FreeObject(obj);
			break;
		case TOKEN_SYMBOL:
			obj = Scheme_CreateSymbol(lex->symbol);
			Scheme_Display(obj);
			Scheme_Newline();
			Scheme_FreeObject(obj);
			break;
		case TOKEN_NUMBER:
			if (lex->number_type == NUMBER_DOUBLE)
				obj = Scheme_CreateDouble(lex->double_val);
			else if (lex->number_type == NUMBER_INTEGER)
				obj = Scheme_CreateInteger(lex->integer_val);
			Scheme_Display(obj);
			Scheme_Newline();
			Scheme_FreeObject(obj);
			break;
		default:
			printf("%c ", (char)token);
			Scheme_Newline();
			break;
		}
	}
	Scheme_Newline();

	char * err;
	if ((err = Lexer_GetError()) != NULL) {
		printf("Lexer: %s\n", err);
	}
}

int main(int argc, char ** argv) {
	struct lexer lex;
	/*FILE * file = fopen("test.scm", "r");
	if (!file) {
		printf("Cannot open file\n");
		return 0;
	}*/

	/*if (!Lexer_LoadFromFile(&lex, file)) {
		printf("Error\n");
		return 0;
	}*/

	InitSymTable(SYM_TABLE_INIT_SIZE);

	Lexer_LoadFromStream(&lex, stdin);
	Scheme_DefineStartupEnv();

	Scheme_InitCallStack(SCHEME_STACK_SIZE);

	//Scheme_DisplayEnv(SYSTEM_GLOBAL_ENVIRONMENT);
	//DisplaySymbolTable();

	while (!SCHEME_INTERPRETER_HALT) {
		printf("~> ");
		scheme_object * obj = Parser_Parse(&lex);
		if (!obj) break;

		scheme_object * eval_result = Scheme_Eval(obj, USER_INITIAL_ENVIRONMENT_OBJ);
		char * err = Scheme_GetError();

		if (eval_result) {
			Scheme_Display(eval_result);
			printf("\n");
		} else if (err) {
			printf("%s\n", err);
		}

		Scheme_DereferenceObject(&obj);
		Scheme_DereferenceObject(&eval_result);
	}

	Scheme_FreeCallStack();
	Scheme_FreeStartupEnv();
	FreeSymTable();
	
	//fclose(file);
	//Lexer_Free(&lex);
	return 0;
}
