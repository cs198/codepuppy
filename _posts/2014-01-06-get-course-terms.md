---
category: People
path: '/people/[:id]/course_terms'
title: 'Get course terms'
type: 'GET'

layout: nil
---

Retrieve course terms (like CS106A winter) that a person is a student in.

## Request

### URI

* Requred: Valid ID of a person in URL.

### Parameters

* (optional) `role`: Role in the course_term, from `{"student", "admin",
  "leader"}`

### Headers

Currently, none

## Response

Sends back an array of `course_term` objects.

```Status: 200 OK```
```
{
  {
    id: 3,
    course_id: 2,  # this refers to the generated "id" field in a course, NOT "CS106A"
    course_name: "Introduction to Computer Science",
    term_name: "Winter",
    period: "12-13",
    active: true
  }
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
