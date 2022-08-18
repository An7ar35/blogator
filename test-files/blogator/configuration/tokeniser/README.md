# Blogator: Configuration tokenizer unit tests


## Unit test format

Unit tests are placed inside a JSON `tests` array. Each test has a `description` and `input` field as well
as a `tokens` and `errors` array for the expected tokens and errors respectively.

Tokens and errors should be in the same order as the expected output.

```json
{"tests": [

  {
    "description": "unit test description",
    "input": "",
    "tokens": [
      ...
    ],
    "errors": [
      ...
    ]
  },
  
  ...

] }
```

## Errors

Errors are formatted as follows:

`{ "description": "...", "line": 1, "col": 1 }`

The description field matches the short error description of the error code raised.

## Token types

There is only one universal format:

`{ "type": "..", "text": "", "position": "1:1" }`

types:
- `COMMENT`: inlined (`//...`) or blocks (`/* ... */`)
- `NAMESPACE`: namespace for key
- `KEY`: key
- `OPERATOR`: operators 
- `PUNCTUATOR`: punctuators
- `BRACKET`: brackets (curly and parenthesis)
- `CHARACTER`: individual characters that do not fit into any other tokens
- `VALUE_NAME`: value name
- `STRING_LITERAL`: string literal (`"..."`)
- `INTEGER_LITERAL_10`: integer literal base 10
- `INTEGER_LITERAL_16`: integer literal base 16
- `INTEGER_LITERAL_FLT`: float literal number
- `BOOLEAN_LITERAL`: boolean literal (`true` or `false`)
- `END_OF_FILE`: \[Quiet\] The EOF token is only used as a boundary check and never used in the tests for output comparison directly.