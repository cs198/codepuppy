---
category: Courses
path: '/courses/[:id]'
title: 'Get a course'
type: 'GET'

layout: nil
---

Retrieve an course by ID.

## Request

* Valid ID in URL.

## Response

Sends back a course with the given id.

```Status: 200 OK```
```
{
  {
    id: 2,
    course_dept: "CS",
    course_code: "106A",
    course_name: "Programming Methodology",
    term_name: "Winter",
    period: "2013-2014",
    active: true
  }
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
