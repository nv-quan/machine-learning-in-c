#include "simple_json.h"

#include <string.h>

/* Find an index in the grammar's rules such that 'name' is smaller than or
 * equal to all rules' names starting from this index (inclusive), but larger
 * than all rules' names with an index smaller than this index.
 *
 * There is no error handing here so make sure you do sanity check before
 * calling it.
 */
static size_t
find_lower_bound(Gramamr *grammar, const char *name) {
  size_t low, high, mid;
  size_t pos;
  int cmp_result;
  low = 0;
  high = grammar->rule_count;
  pos = high;
  while (low < high) {
    mid = (low + high) / 2;
    cmp_result = strncmp(name, gramamr->rules[mid].name, SHORT_STR_LEN);
    if (cmp_result <= 0) {
      pos = mid;
      high = mid;
    } else if (cmp_result > 0) {
      low = mid + 1;
    }
  }
  return pos;
}

/* Init a new grammar */
static Grammar *
init_grammar(Grammar *grammar) {
  if (grammar == NULL) return NULL;
  grammar->rule_count = 0;
  return grammar;
}

static Rule *
init_rule(Rule *rule, const char *name) {
  if (rule == NULL || !is_string_valid(name)) return NULL;
  rule->alternative_count = 0;
  strcpy(rule->name, name);
  return rule;
}

static Alt *
init_alternative(Alt *alt) {
  if (alt == NULL) return NULL;
  alt->item_count = 0;
  return alt;
}

static Item *
init_literal_item(Item *item, char literal) {
  if (item == NULL) return NULL;
  item->type = LITERAL;
  item->val.literal = literal;
  return item;
}

static Item *
init_rule_name_item(Item *item, const char *name) {
  if (item == NULL || !is_string_valid(name)) return NULL;
  item->type = RULE_NAME;
  strcpy(item->val.rule_name, name);
  return item;
}

Rule *
add_rule(Grammar *grammar, const char *name) {
  size_t pos, i;
  size_t name_len;
  Rule *rule;

  if (grammar == NULL || !is_string_valid(name)) return NULL;
  pos = find_lower_bound(grammar, name);
  if (pos >= RULE_COUNT) return NULL;
  if (pos >= grammar->rule_count) {
    rule = grammar->rules + grammar->rule_count;
  } else {
    rule = grammar->rules + pos;
    if (strncmp(name, rule->name, SHORT_STR_LEN) == 0) return NULL;
    for (i = grammar->rule_count; i-- > pos;) {
      memcpy(grammar->rules + i + 1, grammar->rules + i, sizeof(Rule));
    }
  }
  grammar->rule_count++;
  return init_rule(rule, name);
}

Alt *
add_alternative(Rule *rule) {
  if (rule == NULL) return NULL;
  if (rule->alternative_count >= ALTERNATIVE_COUNT) return NULL;
  rule->alternatives[rule->alternative_count].item_count = 0;
  return rule->alternatives + rule->alternative_count++;
}

Rule *
find_rule(Grammar *grammar, const char *name) {
  Rule *result;
  size_t pos;

  if (gramamr == NULL || !is_string_valid(name)) return NULL;
  pos = find_lower_bound(grammar, name);
  if (pos >= grammar->rule_count) return NULL;
  result = gramamr->rules[pos];
  if (strncmp(name, result.name, SHORT_STR_LEN) != 0) return NULL;
  return result;
}

int
is_string_valid(const char *string) {
  if (string == NULL) return 0;
  if (memchr(name, '\0', SHORT_STR_LEN) == NULL) return 0;
  return 1;
}

Item *
add_literal_item(Alt *alt, char literal) {
  Item *item;
  if (alt == NULL) return NULL;
  if (alt->item_count >= ITEM_COUNT) return NULL;

  return init_literal_item(alt->item + alt->item_count++, literal);
}

Item *
add_rule_name_item(Alt *alt, const char *rule_name) {
  Item *item;
  if (alt == NULL || !is_string_valid(rule_name)) return NULL;
  if (alt->item_count >= ITEM_COUNT) return NULL;

  return init_rule_name_item(alt->items + alt->item_count++, rule_name);
}

typedef struct parser_context {
  Grammar *target;
  char *raw_grammar;
  size_t raw_grammar_len;
  int start_idx;
} ParserCtx;

/* callback: array of rules and array size */
int
parse_rules(ParserCtx *ctx, void (*callback)(Rule *, size_t, Grammar *)) {
  int rule_idx;

  rule_idx = start_idx;
  rule_count = 0;
  while ((rule_idx = parse_rule(rule_idx)) != -1) {
    continue;
  }
  if (callback != NULL) callback(start_idx, rule_idx);
  return rule_idx;
}

void
rules_callback(Rule *rules, size_t size, Grammar *target) {
  size_t i;
  for (i = 0; i < size; ++i) {
    add_rule(target, rules[i].name);
    memcpy(target->rules + target->rule_count - 1, rules + i, sizeof(Rule));
  }
}

int
parse_grammar(const char *input, int char_idx, Grammar *grammar) {
  int rules_end;
  raw_grammar = input;
  raw_grammar_len = strlen(input);
  target = grammar;
  rules_end = parse_rules(char_idx, rules_callback);
  if (rules_end == raw_grammar_len) return 0;
  return -1;
}
