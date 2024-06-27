#include "simple_json.h"

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

void
init_json_grammar(Grammar *grammar) {
  int rule_count = 0;
  strcpy(grammar->names[rule_count], "JSON");
  init_json_rule(&grammar->rules[rule_count++]);
}
