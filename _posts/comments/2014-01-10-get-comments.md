---
category: Comments
path: '/submission_files/[:submission_file_id]/file_comments'
title: 'Get all comments for a submission file'
type: 'GET'

layout: nil
---

Retrieve a list of all comments for a given submission file.

## Request

* Valid submission file ID in URL.

## Response

Sends back a list of all file comments for the submission file with the given id.

```Status: 200 OK```
```
{
  [
    {
      id: "15",
      submission_file_id: "43",
      line_number: "12",
      num_lines: "1",
      comment: "You should consider not using a GOTO statement here."
    },
    {
      id: "17",
      submission_file_id: "43",
      line_number: "26",
      num_lines: "1",
      comment: "Generally speaking, we try to avoid while-false loops."
    }
  ]
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
