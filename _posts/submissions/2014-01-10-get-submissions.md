---
category: Submissions
path: '/submissions/[:id]'
title: 'Get a submission by id'
type: 'GET'

layout: nil
---

Retrieve a submission by ID.

## Request

* Valid submission ID in URL.

## Response

Sends back a submission with the given id.

```Status: 200 OK```
```
{
  {
    id: "71",
    person_id: "23",
    assignment_id: "15",
    feedback_released: "false",
    date_submitted: "2014-3-04 11:59:57"
  }
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
