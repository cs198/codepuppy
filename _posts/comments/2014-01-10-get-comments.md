---
category: Comments
path: '/comments/[:id]'
title: 'Get a comment by id'
type: 'GET'

layout: nil
---

Retrieve a comment by ID.

## Request

* Valid comment ID in URL.

## Response

Sends back a comment with the given id.

```Status: 200 OK```
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

For errors responses, see the [response status codes documentation](#response-status-codes).
