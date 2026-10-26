# TINY Scanner DFA: All Tokens

Every token type has its own accepting state. Triple circles `(((...)))` are accepting states. Rounded boxes show the token returned.

## 1. Full DFA

```mermaid
flowchart LR
    S0((S0<br/>start))

    %% ---------- Identifier / reserved words ----------
    S0 -- "letter" --> ID(((IN_ID)))
    ID -- "letter" --> ID
    ID -- "other<br/>(retract)" --> LOOKUP{{"reserved-word<br/>lookup"}}
    LOOKUP -- "not reserved" --> T_ID(["IDENTIFIER"])
    LOOKUP -- "if" --> T_IF(["IF"])
    LOOKUP -- "then" --> T_THEN(["THEN"])
    LOOKUP -- "else" --> T_ELSE(["ELSE"])
    LOOKUP -- "end" --> T_END(["END"])
    LOOKUP -- "repeat" --> T_REPEAT(["REPEAT"])
    LOOKUP -- "until" --> T_UNTIL(["UNTIL"])
    LOOKUP -- "read" --> T_READ(["READ"])
    LOOKUP -- "write" --> T_WRITE(["WRITE"])

    %% ---------- Number ----------
    S0 -- "digit" --> NUM(((IN_NUM)))
    NUM -- "digit" --> NUM
    NUM -- "other<br/>(retract)" --> T_NUM(["NUMBER"])

    %% ---------- Assign ----------
    S0 -- "':'" --> COLON((IN_COLON))
    COLON -- "'='" --> A(((DONE_ASSIGN))) --> T_ASSIGN(["ASSIGN"])
    COLON -- "other" --> ERR(["ERROR"])

    %% ---------- Single-character tokens ----------
    S0 -- "';'" --> D1(((DONE))) --> T_SEMI(["SEMICOLON"])
    S0 -- "'<'" --> D2(((DONE))) --> T_LT(["LESSTHAN"])
    S0 -- "'='" --> D3(((DONE))) --> T_EQ(["EQUAL"])
    S0 -- "'+'" --> D4(((DONE))) --> T_PLUS(["PLUS"])
    S0 -- "'-'" --> D5(((DONE))) --> T_MINUS(["MINUS"])
    S0 -- "'*'" --> D6(((DONE))) --> T_MULT(["MULT"])
    S0 -- "'/'" --> D7(((DONE))) --> T_DIV(["DIV"])
    S0 -- "'('" --> D8(((DONE))) --> T_OB(["OPENBRACKET"])
    S0 -- "')'" --> D9(((DONE))) --> T_CB(["CLOSEDBRACKET"])

    %% ---------- Skipped input ----------
    S0 -- "whitespace" --> S0
    S0 -- "'{'" --> CMT((IN_COMMENT))
    CMT -- "any except '}'" --> CMT
    CMT -- "'}'" --> S0

    S0 -- "any other char" --> ERR
```

## 2. Summary: token, accepting state, lexeme

| Token type | Lexeme | Accepting state | Path from S0 |
|---|---|---|---|
| IDENTIFIER | `x`, `abc`, `xyz` | IN_ID | letter, then letter* |
| IF | `if` | IN_ID + lookup | letters spelling `if` |
| THEN | `then` | IN_ID + lookup | letters spelling `then` |
| ELSE | `else` | IN_ID + lookup | letters spelling `else` |
| END | `end` | IN_ID + lookup | letters spelling `end` |
| REPEAT | `repeat` | IN_ID + lookup | letters spelling `repeat` |
| UNTIL | `until` | IN_ID + lookup | letters spelling `until` |
| READ | `read` | IN_ID + lookup | letters spelling `read` |
| WRITE | `write` | IN_ID + lookup | letters spelling `write` |
| NUMBER | `12`, `289` | IN_NUM | digit, then digit* |
| ASSIGN | `:=` | DONE_ASSIGN | `:` then `=` |
| SEMICOLON | `;` | DONE | `;` |
| LESSTHAN | `<` | DONE | `<` |
| EQUAL | `=` | DONE | `=` |
| PLUS | `+` | DONE | `+` |
| MINUS | `-` | DONE | `-` |
| MULT | `*` | DONE | `*` |
| DIV | `/` | DONE | `/` |
| OPENBRACKET | `(` | DONE | `(` |
| CLOSEDBRACKET | `)` | DONE | `)` |

## 3. Notes

- **Reserved words:** keywords follow the same letter path as identifiers. Once the DFA accepts in IN_ID, the lexeme is compared with the 8 reserved words. This is the standard approach, and it keeps the DFA small. (The textbook TINY scanner does the same.)
- **Retraction:** IN_ID and IN_NUM end on a character that is not part of the token, so that character is pushed back and read again from S0.
- **Longest match:** `:=` is only recognised when `=` directly follows `:`. A lone `:` is an error. The single `=` in S0 gives EQUAL.
- **No token output:** whitespace and `{ ... }` comments return to S0 without producing a token.
- **ELSE** is included because the TINY grammar uses it, but it is not in the handout's token table.
