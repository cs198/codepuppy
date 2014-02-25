---
category: People
path: '/people'
title: 'Create a person'
type: 'POST'

layout: nil
---

Create a new person

## Request

### Header
None yet

### Parameters
* `given_name` (`string`, required): The new person's "first" name.
* `family_name` (`string`, required): The new person's "last" name.
* `user_system_id` (`string`, required): The new person's system ID (in
  Stanford, this is the SUnet ID)

#### Example:
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
