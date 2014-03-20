---
category: Submissions
path: '/submissions/[:submission_id]/update_feedback_released'
title: 'Update feedback released'
type: 'PUT'

layout: nil
---

Update feedback released for a submission.

## Request

### URI

* **`:submission_id`** the id of the submission to update.

### Body

* `feedback_released` (`boolean`, required): Whether or not the feedback should
  be released.

Example:

```
{
    feedback_released: "true"
}
```

## Response

Sends back the updated Submission object.

```Status: 200 OK```
```
{
  {
    id: "71",
    person_id: "23",
    assignment_id: "15",
    feedback_released: "true",
    date_submitted: "2014-3-04 11:59:57"
  }
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
