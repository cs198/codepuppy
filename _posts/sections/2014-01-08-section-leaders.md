---
category: Sections
path: '/section/[:section_id]/leaders'
title: 'Get a leader for a section'
type: 'GET'

layout: nil
---

Retrieve a leader for a section.

## Request

* Valid section ID in URL.

## Response

Sends back a person who leads the section.

```Status: 200 OK```
```
{
  {
    id: 2,
    given_name: "Omar",
    family_name: "Diab",
    user_system_id: "odiab"
  }
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
