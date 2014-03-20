---
category: Announcements
path: '/courses/[:course_id]/announcements'
title: 'Create an announcement'
type: 'POST'

layout: nil
---

Create a new announcement for a course

## Request

### Header
None yet

### Body
* `course_id` (`integer`, required): Given in URI. The DB-generated ID for the course for
  which this is an announcement.
* `subject` (`string`, required): The subject line of the announcement.
* `body` (`string`, required): The body of the announcement.

Example:

```
{
  course_id: "17",
  subject: "Final exam room change",
  body: "The final exam will now take place in Gates 104 at 12:15pm on Friday."
}
```

## Response

**If succeeds**, returns the created course.

```Status: 201 Created```
```
{
  {
    id: "21",
    course_id: "17",
    subject: "Final exam room change",
    body: "The final exam will now take place in Gates 104 at 12:15pm on Friday."
  }
}
```

For error responses, see the [response status codes documentation](#response-status-codes).
