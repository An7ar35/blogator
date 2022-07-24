# Blogator: Markdown tree builder unit tests


## Unit test format

Unit tests are placed inside a JSON `tests` array. Each test has a `description`, `markdown` and `html` field as well
as a `errors` array for the expected errors in order.

The `markdown` field hold the markdown formatted input string whilst the `html` field holds the expected HTML code produced from the input.

```json
{"tests": [

  {
    "description": "unit test description",
    "markdown": "...",
    "html": "",
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