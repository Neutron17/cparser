#ifndef _NTR_TOKENIZER_H_
#define _NTR_TOKENIZER_H_ 1
#include "list.h"

enum TType {
	T_IDENTIFIER = 0,
	T_KEYWORD,
	T_TYPE_KEYWORD,
	T_SEPARATOR,
	T_OPERATOR,
	T_LITERAL,
	T_COMMENT
};

enum KeyWordType {
	K_IF = 0,
	K_ELSE,
	K_WHILE,
	K_FOR,
	K_DO,
	K_UNTIL,
	K_ELIF
};

enum TypeKeyWordType {
	TK_INT = 0,
	TK_CHAR,
	TK_FLOAT,
	TK_DOUBLE,
	TK_LONG,
	TK_STR,
	TK_SHORT
};

enum OperatorType {
	O_EQUALS = 0,
	O_PLUS,
	O_MINUS,
	O_MULT,
	O_DIV,
	O_MOD,
	O_AND
};

enum SeparatorType {
	S_SEMICOL = 0,
	S_PRENTH_BEGIN,
	S_PRENTH_END,
	S_BRACE_BEGIN,
	S_BRACE_END,
	S_BRACKET_BEGIN,
	S_BRACKET_END
};

enum LiteralType {
	L_TRUE,
	L_FALSE,
	L_INT,
	L_UINT,
	L_LONG,
	L_FLOAT,
	L_DOUBLE
};

enum IdentifierType {
	I_VAR,
	I_VAR_STATIC,
	I_CONST,
	I_PTR,
	I_CONST_PTR,
	I_FUNC,
	I_FUNC_INLINE
};

int strtotype(const char *restrict str, const char *restrict arr[7]);

struct Token {
	enum TType type;
	int subtype;
	/*union sub {
		enum KeyWordType KeywordType;
		enum TypeKeyWordType typeKeywordType,
		enum OperatorType operatorType
	};*/
	char value[7];
};
struct Token *token(enum TType type, int subtype, char *value);

List tokenize(char *txt);

#endif //_NTR_TOKENIZER_H_