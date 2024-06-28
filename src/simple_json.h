/* Simplified JSON grammar
 * JSON
 *      ELEMENT
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
  char name[SHORT_STR_LEN];
} Rule;

typedef struct grammar {
  Rule rules[RULE_COUNT];
  size_t rule_count;
} Grammar;

/* Add a rule to the grammar and return a pointer to it.
 *
 * Internally, rules are sorted by name so that find_rule can be used
 * efficiently. This is optimized for find_rule since find_rule is used more.
 *
 * If an error occurs, NULL is returned. Possible errors include the grammar
 * being full, the rule already existing, or name being too long.
 */
Rule *add_rule(Grammar *grammar, const char *name);

/* Find a rule in the grammar by name.
 *
 * When the rule is not found, NULL is returned. This function only works when
 * rules are added by add_rule.
 */
Rule *find_rule(Grammar *grammar, const char *name);

/* Add an empty alternative to a rule */
Alt *add_alternative(Rule *rule);

/* Add a literal item to an alternative */
Item *add_literal_item(Alt *alt, char literal);

/* Add a rule name to an alternative */
Item *add_rule_name_item(Alt *alt, const char *rule_name);

void init_json_grammar(Grammar *grammar);

/* Return 1 if string is smaller than SHORT_STR_LEN -1
 *
 * This is to prevent mishandling of null-terminated strings
 */
int is_string_valid(const char *string);

#endif /* SIMPLE_JSON_H */
