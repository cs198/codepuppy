---
category: Courses
path: '/courses'
title: 'Create a course'
type: 'POST'

layout: nil
---

Create a new course

## Request

### Header
None yet

### Body
* `course_dept` (`string`, required): The new course's department code (e.g.
* "CS").
* `course_code` (`string`, required): The new course's in-department number.
* `course_name` (`string`, required): The new course's title.
* `term_name` (`string`, required): The collegiate term for this offering of
  the course (e.g. "Fall", "Winter" or "Spring").
* `period` (`string`, required): The academic year for this offering of the
  course (e.g. "2013-2014").
* `active` (`boolean`, required): Whether or not this course is actively
  offered.

Example:

```
{
  course_dept: "CS",
  course_code: "106A",
  course_name: "Programming Methodology",
  term_name: "Winter",
  period: "2013-2014",
  active: true
}
```

## Response

**If succeeds**, returns the created course.

```Status: 201 Created```
```
{
  {
    course_dept: "CS",
    course_code: "106A",
    course_name: "Programming Methodology",
    term_name: "Winter",
    period: "2013-2014",
    active: true
  }
}
```

For error responses, see the [response status codes documentation](#response-status-codes).
