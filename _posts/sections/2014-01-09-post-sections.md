---
category: Sections
path: '/sections'
title: 'Create a section'
type: 'POST'

layout: nil
---

Create a new section

## Request

### Header
None yet

### Body
* `course_id` (`integer`, required): The DB-given ID for the course for which
  this is a section..
* `leader_id` (`integer`, required): The DB-given ID for the person who leads
* this section.

Example:

```
{
  course_id: "3",
  leader_id: "26"
}
```

## Response

**If succeeds**, returns the created section.

```Status: 201 Created```
```
{
  {
    id: 2,
    course_id: "3",
    leader_id: "26"
 }
}
```

For error responses, see the [response status codes documentation](#response-status-codes).
