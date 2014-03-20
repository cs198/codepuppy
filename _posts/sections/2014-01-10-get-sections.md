---
category: Sections
path: '/section/[:id]'
title: 'Get a section'
type: 'GET'

layout: nil
---

Retrieve a section by ID.

## Request

* Valid section ID in URL.

## Response

Sends back a section with the given id.

```Status: 200 OK```
```
{
  {
    id: 2,
    course_id: "3",
    leader_id: "26"
  }
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
