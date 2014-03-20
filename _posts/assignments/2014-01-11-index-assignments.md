---
category: Assignments
path: '/courses/[:course_id]/assignments'
title: 'Get all assignments by course'
type: 'GET'

layout: nil
---

Retrieve a list of all assignments for a given course.

## Request

* Valid course ID in URL.

## Response

Sends back a list of all assignments for the course with the given id.

```Status: 200 OK```
```
{
  [
    {
      id: 20,
      course_id: 3,
      number: 6,
      date_assigned: "2014-2-27",
      date_due: "2014-03-6",
      name: "Name Surfer",
      description: "Some string containing wiki markup for Name Surfer"
      released: true,
      pdf_url: "http://cs106b.stanford.edu/handouts/namesurfer.pdf"
    },
    {
      id: 33,
      course_id: 3,
      number: 7,
      date_assigned: "2014-3-06",
      date_due: "2014-03-18",
      name: "FacePamphlet",
      description: "Some string containing wiki markup for FacePamphlet"
      released: true,
      pdf_url: "http://cs106b.stanford.edu/handouts/facepamphlet.pdf"
    }
  ]
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
