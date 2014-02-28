---
category: Assignments
path: '/people/[:id]'
title: 'Get a person'
type: 'GET'

layout: nil
---

Retrieve an individual user by ID.

## Request

* Valid ID in URL.

## Response

Sends back a person with the given id.

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
