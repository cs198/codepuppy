---
category: Assignments
path: '/assigments'
title: 'Create an assignment'
type: 'POST'

layout: nil
---

Create a new assignment

## Request

### Header
None yet

### Body
* `course_id` (`integer``, required): The DB-generated ID for the course for
  which this is an assignment.
* `number` (`integer`, required): The ordered number of this assignment (i.e.
  3) (think "assignment #3").
* `date_assigned` (`datetime`, required): The day on which the assignment is
  released.
* `date_due` (`datetime`, required): The day on which the assignment is due.
* `name` (`string`, required): The name of the assignment.
* `description` (`string`, required): The wiki-formatted description of the
  assignment for use by admins and leaders, but not students.
* `released` (`string`, required): Specifies whether or not the assignment has
  been released.
* `pdf_url` (`string`, optional): A link to an assignment handout from an
  outside website.

Example:

```
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
```

## Response

**If succeeds**, returns the created assignment.

```Status: 201 Created```
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

For error responses, see the [response status codes documentation](#response-status-codes).
