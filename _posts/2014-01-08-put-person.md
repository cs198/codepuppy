---
category: People
path: '/people/:id'
title: 'Update a person'
type: 'PUT'
unimplemented: true

layout: nil
---

Update a user's information, like their `given_name` or `user_system_id`.

## Request

* **`:id`** is the id the thing to update.
* **The body can't be empty** and must include at least one editable field from
  this list:
  * `given_name` (`string`)
  * `family_name` (`string`)
  * `user_system_id` (`string`)

```
{
    given_name: "Alfred"  # used to be "Omar"
}
```

## Response

Sends back the updated Person object.

```Status: 200 OK```
```
{
  {
    id: 2,
    given_name: "Alfred",
    family_name: "Diab",
    user_system_id: "odiab"
  }
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
