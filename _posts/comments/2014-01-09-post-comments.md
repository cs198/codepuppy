---
category: Comments
path: '/submission_files/[:submission_file_id]/file_comments'
title: 'Create a comment'
type: 'POST'

layout: nil
---

Create a new comment for a submission file

## Request

### Header
None yet

### Body
* `submission_file_id` (`integer`, required): Given in URI. The DB-given ID for
  the submission file to which this comment will be added.
* `line_number` (`integer`, required): The starting line number for the comment..
* `num_lines` (`integer`, required): The number of lines this comment spans.
  Currently, only supports a single line, so use '1' for num_lines until this
  issue is resolved.
* `comment` (`string`, required): The actual text of the comment.

Example:

```
{
  submission_file_id: "43",
  line_number: "12",
  num_lines: "1",
  comment: "You should consider not using a GOTO statement here."
}
```

## Response

**If succeeds**, returns the created person.

```Status: 201 Created```
```
{
  {
    id: "15",
    submission_file_id: "43",
    line_number: "12",
    num_lines: "1",
    comment: "You should consider not using a GOTO statement here."
  }
}
```

For error responses, see the [response status codes documentation](#response-status-codes).
