# Blogator: Markdown tokenizer unit tests


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

Some tokens require a `BLOCK_END` token as they hold content (character or otherwise). See below for expected individual token format.

## Errors

Errors are formatted as follows:

`{ "description": "...", "line": 1, "col": 1 }`

The description field matches the short error description of the error code raised.

## Token types

### Blockquote

```
{ "type": "BLOCK_QUOTE", "nesting": 0, "text": ">", "position": "1:1"}
{ "type": "BLOCK_END", "block": "BLOCK_QUOTE", "position": "1:2"}
```

### Character

`{ "type": "CHARACTER", "text": "...", "position": "1:1" }`

### Code block 

```
{ "type": "BLOCK_QUOTE", "nesting": 0, "text": ">", "position": "1:1" },
{ "type": "BLOCK_END", "block": "CODE_BLOCK", "position": "1:2"},
```

### Footnote

#### Reference token

`{ "type": "FOOTNOTE_REF", "text": "...", "position": "1:1" }`

#### Definition tokens

Footnote definitions are parsed like a list whose type is `FOOTNOTE_DEFS`.
Each consecutive definitions are nested in `LIST_ITEM` tokens whose ID is the reference string.

```
{ "type": "LIST", "list": "FOOTNOTE_DEFS", "tight": "true", "position": "1:1" },
{ "type": "LIST_ITEM", "id": "1", "modality": "NONE", "position": "1:1" },
...
{ "type": "BLOCK_END", "block": "LIST_ITEM", "position": "1:10" },
{ "type": "BLOCK_END", "block": "LIST", "position": "1:10" }
```

#### Example

```text
text[^1]

[^1]: definition
```

```json
[
   { "type": "PARAGRAPH", "id": "", "position": "1:1" },
   { "type": "CHARACTER", "text": "text", "position": "1:1" },
   { "type": "FOOTNOTE_REF", "text": "...", "position": "1:5" },
   { "type": "BLOCK_END", "block": "PARAGRAPH", "position": "2:1" },
   { "type": "LIST", "list": "FOOTNOTE_DEFS", "tight": "true", "position": "3:1" },
   { "type": "LIST_ITEM", "ref": "1", "modality": "NONE", "position": "3:1" },
   { "type": "PARAGRAPH", "id": "", "position": "3:7" },
   { "type": "CHARACTER", "text": "definition", "position": "3:7" },
   { "type": "BLOCK_END", "block": "PARAGRAPH", "position": "3:17" },
   { "type": "BLOCK_END", "block": "LIST_ITEM", "position": "3:17" },
   { "type": "BLOCK_END", "block": "LIST", "position": "3:17" }
]
```

### Formatting

```
{ "type": "FORMAT_BEGIN", "formatting": "BOLD", "text": "**", "position": "1:1" },
{ "type": "FORMAT_END", "formatting": "BOLD", "text": "**", "position": "1:3" },
```

Formatting types:
- `BOLD`: double asterisk or underscore characters ("`**...**` or `__...__`)
- `ITALIC`: single asterisk or underscore characters (`*...*` or `_..._`)
- `STRIKETHROUGH`: double tilde characters (`~~...~~`)
- `HIGHLIGHT`: double equal characters (`==...==`)
- `SUBSCRIPT`: single tilde character (`~...~`)
- `SUPERSCRIPT`: single circumflex accent character (`^...^`)
- `INLINE_CODE_1`: single grave accents ("\`...\`")
- `INLINE_CODE_2`: double grave accents ("\`\`...\`\`")

### Heading

```
{ "type": "HEADING", "id": "", "level": 1, "text": "#", "position": "1:1" },
{ "type": "CHARACTER", "text": "heading", "position": "1:3" },
{ "type": "BLOCK_END", "block": "HEADING", "position": "1:10" }
```

### Horizontal rule

The text field in the token hold the full horizontal rule declaration characters.

`{ "type": "HORIZONTAL_RULE", "text": "***", "position": "1:1" }`

### Hyperlink

```
{ "type": "HYPERLINK", "url": "https://example.com", "position": "1:1" },
{ "type": "CHARACTER", "text": "text", "position": "1:2" },
{ "type": "BLOCK_END", "block": "HYPERLINK", "position": "1:27" }
```

### Image

Parsed in a similar fashion to a hyperlink. The 'alt' text is passed as character tokens inside the image block.

```
{ "type": "IMAGE", "src": "image.jpg", "position": "1:1" }
{ "type": "CHARACTER", "text": "alt-text", "position": "1:3" },
{ "type": "BLOCK_END", "block": "IMAGE", "position": "1:22" }
```

### Ordered list

Modality will always be `NONE` and the list item IDs empty.

```
{ "type": "LIST", "list": "OL_ALPHA_LOWER", "tight": "false", "position": "1:1" },
{ "type": "LIST_ITEM" "ref": "", "modality": "NONE", "position": "1:1" },
{ "type": "PARAGRAPH", "id": "", "position": "1:4" },
{ "type": "CHARACTER", "text": "item 1", "position": "1:4" },
{ "type": "BLOCK_END", "block": "PARAGRAPH", "position": "2:1" },
{ "type": "BLOCK_END", "block": "LIST_ITEM", "position": "3:1" },
{ "type": "LIST_ITEM", "ref": "", "modality": "NONE", "position": "3:1" },
{ "type": "PARAGRAPH", "position": "3:4" },
{ "type": "CHARACTER", "text": "item 2", "position": "3:4" },
{ "type": "BLOCK_END", "block": "PARAGRAPH", "position": "4:1" },
{ "type": "BLOCK_END", "block": "LIST_ITEM", "position": "4:10" },
{ "type": "BLOCK_END", "block": "LIST", "position": "5:10" }
```

Ordered list types:
- `OL_ALPHA_LOWER`
- `OL_ALPHA_UPPER`
- `OL_NUMERIC`

### Unordered list

Modality will always be `NONE` and the list item IDs empty.

```
{ "type": "LIST", "list": "UL_HYPHEN", "tight": "false", "position": "1:1" },
{ "type": "LIST_ITEM", "ref": "", "modality": "NONE", "position": "1:1" },
{ "type": "PARAGRAPH", "id": "", "position": "1:3" },
{ "type": "CHARACTER", "text": "item 1", "position": "1:3" },
{ "type": "BLOCK_END", "block": "PARAGRAPH", "position": "2:1" },
{ "type": "BLOCK_END", "block": "LIST_ITEM", "position": "3:1" },
{ "type": "LIST_ITEM", "ref": "", "modality": "NONE", "position": "3:1" },
{ "type": "PARAGRAPH", "position": "3:3" },
{ "type": "CHARACTER", "text": "item 2", "position": "3:3" },
{ "type": "BLOCK_END", "block": "PARAGRAPH", "position": "4:1" },
{ "type": "BLOCK_END", "block": "LIST_ITEM", "position": "4:9" },
{ "type": "BLOCK_END", "block": "LIST", "position": "4:9" }
```

Unordered list types:
- `UL_ASTERISK`
- `UL_PLUS_SIGN`
- `UL_HYPHEN`

### Task list

Modality will always be either `CHECKED` or `UNCHECKED` and the list item IDs empty.

```
{ "type": "LIST", "list": "UL_TASK", "tight": "true", "position": "1:1" },
{ "type": "LIST_ITEM", "ref": "", "modality": "UNCHECKED", "position": "1:1" },
{ "type": "PARAGRAPH", "id": "", "position": "1:6" },
{ "type": "CHARACTER", "text": "item 1", "position": "1:6" },
{ "type": "BLOCK_END", "block": "PARAGRAPH", "position": "2:1" },
{ "type": "BLOCK_END", "block": "LIST_ITEM", "position": "2:1" },
{ "type": "LIST_ITEM", "ref": "", "modality": "CHECKED", "position": "2:1" },
{ "type": "PARAGRAPH", "position": "2:6" },
{ "type": "CHARACTER", "text": "item 3", "position": "2:6" },
{ "type": "BLOCK_END", "block": "PARAGRAPH", "position": "2:12" },
{ "type": "BLOCK_END", "block": "LIST_ITEM", "position": "2:12" },
{ "type": "BLOCK_END", "block": "LIST", "position": "2:12" }
```

Modality types:
- `NONE`
- `UNCHECKED`
- `CHECKED`

### Line break

Line break tokens should only be found inside paragraph block.

`{ "type": "LINE_BREAK", "position": "1:10" }`

### Tables

```
{"type": "TABLE", "id": "", "position": "1:1"},
...
{"type": "BLOCK_END", "block": "TABLE", "position": "3:10"}
```

Each row is specified as such inside a table:

```
{"type": "TABLE_ROW", "position": "1:1"},
...
{"type": "BLOCK_END", "block": "TABLE_ROW", "position": "1:10"}
```

#### Table Heading

Table headings are nested in table rows.

```
{"type": "TABLE_HEADING", "alignment": "NONE", "position": "1:1"},
...
{"type": "BLOCK_END", "block": "TABLE_HEADING", "position": "1:10"}
```

Table headings are content containers. The alignment can be any value in:
- `NONE` (default: `---`)
- `LEFT` (`:--`)
- `CENTER` (`:-:`)
- `RIGHT` (`--:`)

#### Table Data

Like headings, data cells are also nested in rows. Cells are specified as follows:

```
{"type": "TABLE_CELL", "position": "3:1"},
...
{"type": "BLOCK_END", "block": "TABLE_CELL", "position": "3:9"}
```

#### Example

| heading 1 | heading 2 |
|----------:|:---------:|
|         a |     b     |

For a simple 2x2 table with column 1 right-aligned and column 2 center aligned:

```
|heading 1|heading 2|
|--------:|:-------:|
|a|b|
```


```json
[
    { "type": "TABLE", "id": "", "position": "1:1" },
        { "type": "TABLE_ROW", "position": "1:1" },
            { "type": "TABLE_HEADING", "alignment": "RIGHT", "position": "1:1" },
              { "type": "CHARACTER", "text": "heading 1", "position": "1:2" },
            { "type": "BLOCK_END", "block": "TABLE_HEADING", "position": "1:11" },
            { "type": "TABLE_HEADING", "alignment": "CENTER", "position": "1:11" },
              { "type": "CHARACTER", "text": "heading 2", "position": "1:12" },
            { "type": "BLOCK_END", "block": "TABLE_HEADING", "position": "1:21" },
        { "type": "BLOCK_END", "block": "TABLE_ROW", "position": "1:22" },
        { "type": "TABLE_ROW", "position": "3:1" },
            { "type": "TABLE_CELL", "position": "3:1" },
              { "type": "CHARACTER", "text": "a", "position": "3:2" },
            { "type": "BLOCK_END", "block": "TABLE_CELL", "position": "3:3" },
            { "type": "TABLE_CELL", "position": "3:3" },
              { "type": "CHARACTER", "text": "b", "position": "3:4" },
            { "type": "BLOCK_END", "block": "TABLE_CELL", "position": "3:5" },
        { "type": "BLOCK_END", "block": "TABLE_ROW", "position": "3:6" },
    { "type": "BLOCK_END", "block": "TABLE", "position": "3:6" }
]
```

### Definition list

Definition lists are defined with 'dt' and 'dd' tokens. Each 'dt' blocks are followed by 1 or more 'dd' blocks. 

```
{"type": "DEFINITION_LIST_DT", "position": "1:1"},
...
{"type": "BLOCK_END", "block": "DEFINITION_LIST_DT", "position": "2:1"},
{"type": "DEFINITION_LIST_DD", "position": "2:1"},
...
{"type": "BLOCK_END", "block": "DEFINITION_LIST_DD", "position": "2:13"}
```

##### Example:

```text
word
: definition
```

will yield:

```json
[
    { "type": "DEFINITION_LIST_DT", "position": "1:1" },
    { "type": "CHARACTER", "text": "word", "position": "1:1" },
    { "type": "BLOCK_END", "block": "DEFINITION_LIST_DT", "position": "2:1" },
    { "type": "DEFINITION_LIST_DD", "position": "2:1" },
    { "type": "CHARACTER", "text": "definition", "position": "2:3" },
    { "type": "BLOCK_END", "block": "DEFINITION_LIST_DD", "position": "2:13" }
]
```

### End-of-file

The EOF token is only used as a boundary check and never used in the tests for output comparison directly.

If the token is printed it would look as such:

```
{ "type": "EOF", "position": "100:1" },
```