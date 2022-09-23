# Blogator: Configuration parser integration tests


## Unit test format

Unit tests are placed inside a JSON `tests` array. Each test has a `description` and `input` field as well
as a `entries` and `errors` array for the expected key-value pairs and errors respectively.

Tokens and errors should be in the same order as the expected output.

```json
{"tests": [

  {
    "description": "unit test description",
    "input": "",
    "entries": [
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

## Entries

There is only one universal format:

`{ "key": "..", "value": "" }`

The key-value pair matches those found in the configuration.

For example:

```
:ns: = { key1, key2 = 1; };
```

would lead to:

```json
[
    { "key": ":ns:key1", "value": "1" },
    { "key": ":ns:key2", "value": "1" }
]
```