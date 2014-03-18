---
category: People
path: '/people/[:id]/courses'
title: 'Get courses by person'
type: 'GET'

layout: nil
---

Retrieve course offerings (like CS106A winter) for which a person is a student,
leader, or admin.

## Request

### URI

* Requred: Valid ID of a person in URL.

### Body

* `role` (`string`, optional) : Role in the course, from `["student", "admin",
  "leader"]`

### Headers

Currently, none

## Response

Sends back an array of objects, each of which contains a course and the
person's role in the course. If role is specified in the request body, only the
courses in which that person has the given role is returned; otherwise all
courses that the student is involved in, regardless of role, is returned.

```
Status: 200 OK
```
```
{
  [
    {
      course: {
        id: 3,
        course_dept: "CS",
        course_code: "106A",
        course_name: "Programming Methodology",
        term_name: "Winter",
        period: "13-14",
        active: true
      },
      role: "leader"
    },
    {
      course: {
        id: 4,
        course_dept: "CS",
        course_code: "142",
        course_name: "Web Applications",
        term_name: "Winter",
        period: "13-14",
        active: true
      },
      role: "student"
    }
  ]
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
