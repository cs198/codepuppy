---
category: People
path: '/people/:id'
title: 'Get a person by id'
type: 'GET'

layout: nil
---

Retrieve an individual person by ID.

## Request

* The `id` must correspond to a valid person in the database

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
