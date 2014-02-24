---
category: People
path: '/people'
title: 'Create a person'
type: 'POST'

layout: nil
---

Create a new person

## Request

* The headers must include a **valid authentication token**.
* **The body can't be empty** and must include the following attributes:
  * `given_name`, a `string` containing the new person's "first" name.
  * `family_name`, a `string` containing the new person's "last" name.
  * `user_system_id`, a `string` containing the new person's system ID (in
    Stanford, this is the SUnet ID)

```
{
  given_name: "Omar",
  family_name: "Diab",
  user_system_id: "odiab"
}
```

## Response

**If succeeds**, returns the created person.

```Status: 201 Created```
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

For error responses, see the [response status codes documentation](#response-status-codes).
