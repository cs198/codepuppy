---
category: Submissions
path: '/assignments/[:assignment_id]/submissions'
title: 'Get submissions by assignment'
type: 'GET'

layout: nil
---

Retrieve all submissions for sectionees of the current_user for the given assignment.

## Request

* Valid assignment ID in URL.

## Response

Sends back a list of submissions for all students who have current_user as their leader for the given assignment.

```Status: 200 OK```
```
{
  [
    {
      id: 2,
      id: "71",
      person_id: "23",
      assignment_id: "15",
      feedback_released: "false",
      date_submitted: "2014-3-04 11:59:57"
    },
    {
      id: "72",
      person_id: "24",
      assignment_id: "15",
      feedback_released: "false",
      date_submitted: "2014-3-04 11:59:58"
    }
  ]
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
