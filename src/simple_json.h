/* Simplified JSON grammar
 *
 * VALUE
 *      OBJECT
 *      ARRAY
 *      STRING
 *      NUMBER
 *      TRUE
 *      FALSE
 *      NULL
 *
 * OBJECT
 *      '{' WHITESPACE '}'
 *      '{' MEMBERS '}'
 *
 * MEMBERS
 *      MEMBER
 *      MEMBER ',' MEMBERS
 *
 * MEMBER
 *      WHITESPACE STRING WHITESPACE ':' ELEMENT
 *
 * ARRAY
 *      '[' WHITESPACE ']'
 *      '[' ELEMENTS ']'
 *
 * ELEMENTS
 *      ELEMENT
 *      ELEMENT ',' ELEMENTS
 *
 * ELEMENT
 *      WHITESPACE VALUE WHITESPACE
 *
 * STRING
 *      '"' CHARACTERS '"'
 *
 * CHARACTERS
 *      ""
 *      CHARACTER CHARACTERS
 *
 * CHARACTER
 *      '0020' . '10FFFF' - '"' - '\'
 *      '\' ESCAPE
 *
 * ESCAPE
 *      '"'
 *      '\'
 *      '/'
 *      'b'
 *      'f'
 *      'n'
 *      'r'
 *      't'
 *      'u' HEX HEX HEX HEX
 *
 * HEX
 *      DIGIT
 *      'A' . 'F'
 *      'a' . 'f'
 *
 * NUMBER
 *      INTEGER FRACTIONS EXPONENT
 *
 * INTEGER
 *      DIGIT
 *      ONENINE DIGITS
 *      '-' DIGIT
 *      '-' ONENINE DIGITS
 *
 * DIGITS
 *      DIGIT
 *      DIGIT DIGITS
 *
 * DIGIT
 *      '0'
 *      ONENINE
 *
 * ONENINE
 *      '1' . '9'
 *
 * FRACTIONS
 *      ""
 *      '.' DIGITS
 *
 * EXPONENT
 *      ""
 *      'E' SIGN DIGITS
 *      'e' SIGN DIGITS
 *
 * SIGN
 *      ""
 *      '+'
 *      '-'
 *
 * WHITESPACE
 *      ""
 *      '0020' WHITESPACE (' ')
 *      '000A' WHITESPACE ('\n')
 *      '000D' WHITESPACE ('\r')
 *      '0009' WHITESPACE ('\t')
 */
#ifndef SIMPLE_JSON_H
#define SIMPLE_JSON_H

#define SHORT_STR_LEN 256
#define RULE_COUNT 100
#define ALTERNATIVE_COUNT 256
#define ITEM_COUNT 5

typedef enum item_type { LITERAL, RULE_NAME } ItemType;
typedef union item_value {
  char literal;
  char rule_name[SHORT_STR_LEN];
} ItemVal;

typedef struct item {
  ItemType type;
  ItemVal val;
} Item;

typedef struct alternative {
  Item items[ITEM_COUNT];
  size_t item_count;
} Alt;

typedef struct rule {
  Alt alternatives[ALTERNATIVE_COUNT];
  size_t alternative_count;
} Rule;

typedef struct grammar {
  char names[RULE_COUNT][SHORT_STR_LEN];
  Rule rules[RULE_COUNT];
} Grammar;

void init_json_grammar(Grammar *grammar);

#endif /* SIMPLE_JSON_H */
