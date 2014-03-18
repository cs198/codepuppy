---
category: Assignments
path: '/assignments/[:id]'
title: 'Get an assignment'
type: 'GET'

layout: nil
---

Retrieve an assignment by ID.

## Request

* Valid ID in URL.

## Response

Sends back an assignment with the given id.

```Status: 200 OK```
```
{
  {
    course_id: 3,
    number: 6,
    date_assigned: "2014-2-27",
    date_due: "2014-03-6",
    name: "Name Surfer",
    description: "Some string containing wiki markup for Name Surfer"
    released: true,
    pdf_url: "http://cs106b.stanford.edu/handouts/namesurfer.pdf"
  }
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
