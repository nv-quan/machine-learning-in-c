#include "simple_json.h"
#include <string.h>

/* Find an index in the grammar's rules such that 'name' is smaller than or
 * equal to all rules' names starting from this index (inclusive), but larger
 * than all rules' names with an index smaller than this index.
 */
static size_t find_lower_bound(Gramamr *grammar, const char* name) {
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


/* JSON
 *      ELEMENT
 */
void
init_json_rule(Rule *rule) {
  Alt *alt;
  Item *item;

  rule->alternative_count = 1;
  alt = rule->alternatives[0];
  alt.item_count = 1;
  item = alt.items[0];
  item.type = RULE_NAME;
  strcpy(item.val.rule_name, "ELEMENT");
}
/* VALUE
 *      OBJECT
 *      ARRAY
 *      STRING
 *      NUMBER
 *      TRUE
 *      FALSE
 *      NULL
 */

Rule* add_rule(Grammar* grammar, const char* name) {
  size_t pos, i;
  size_t name_len;
  Rule *rule;

  if (grammar == NULL || name == NULL) return NULL;
  if (memchr(name, '\0', SHORT_STR_LEN) == NULL) return NULL;
  if (name_len == SHORT_STR_LEN) return NULL;
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
  rule->alternative_count = 0;
  /* strcpy is safe because memchr checked the existence of '\0' */
  strcpy(rule->name, name);
  return rule;
}


void
init_json_grammar(Grammar *grammar) {
  int rule_count = 0;
  Rule* rule;
  rule = add_rule(grammar, "JSON");
  init_json_rule(rule);
}

Rule* find_rule(Grammar* grammar, const char* name) {
  Rule* result;
  size_t pos;

  if (gramamr == NULL || name == NULL) return NULL;
  pos = find_lower_bound(grammar, name);
  if (pos >= grammar->rule_count) return NULL;
  result = gramamr->rules[pos];
  if (strncmp(name, result.name, SHORT_STR_LEN) != 0) return NULL;
  return result;
}
