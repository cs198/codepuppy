---
category: Announcements
path: '/courses/[:course_id]/announcements'
title: 'Get all announcements by course'
type: 'GET'

layout: nil
---

Retrieve a list of all announcements for a given course.

## Request

* Valid course ID in URL.

## Response

Sends back a list of all announcements for the course with the given id.

```Status: 200 OK```
```
{
  [
    {
      id: "21",
      course_id: "17",
      subject: "Final exam room change",
      body: "The final exam will now take place in Gates 104 at 12:15pm on Friday."
    },
    {
      id: "35",
      course_id: "17",
      subject: "Final exam grades posted",
      body: "Grades have been posted online. You all passed!"
    }
  ]
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
