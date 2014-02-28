---
category: Comments
path: '/people/:id'
title: 'Update a person'
type: 'PUT'
unimplemented: true

layout: nil
---

Update a user's information, like their `given_name` or `user_system_id`.

## Request

### URI

* **`:id`** the id the person to update.

### Body

At least one of the following, to denote what is being edited:

* `given_name` (`string`, required): The new person's "first" name.
* `family_name` (`string`, required): The new person's "last" name.
* `user_system_id` (`string`, required): The new person's system ID (in
  Stanford, this is the SUnet ID)

Example:

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
