---
category: Submissions
path: '/submissions'
title: 'Create a submission'
type: 'POST'

layout: nil
---

Create a new submission

## Request

### Header
None yet

### Body
* `person_id` (`integer`, required): The DB-given ID of the student submitting
  the assignment.
* `assignment_id` (`integer`, required): The DB-given ID of the assignment for
  which the student is submitting.
* `feedback_released` (`boolean`, required): Indicates whether or not the
  student can view their leader's comments on their submission.
* `date_submitted` (`datetime`, required): A timestamp of when the student
  uploaded this submission.

Example:

```
{
  person_id: "23",
  assignment_id: "15",
  feedback_released: "false",
  date_submitted: "2014-3-04 11:59:57"
}
```

## Response

**If succeeds**, returns the created course.

```Status: 201 Created```
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

For error responses, see the [response status codes documentation](#response-status-codes).
