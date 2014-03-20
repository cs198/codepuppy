---
category: Submission files
path: '/submissions/[:submission_id]/submission_files'
title: 'Create a submission file'
type: 'POST'

layout: nil
---

Create a new submission file

## Request

### Header
None yet

### Body
* `submission_id` (`integer`, required): Given in URI. The DB-given ID of the
  submission to which this file belongs.
* `file` (`angular file`, required): The AngularJS file object recieved through
  the file upload API.

Example:

```
{
  submission_id: "10",
  file: [AngularJS file]
}
```

## Response

**If succeeds**, returns the created submission file.

```Status: 201 Created```
```
{
  {
    id: "71",
    submission_id: "10",
    original_filename: "hello.cpp",
    data: "#include <iostream>\nint main() {\nstd::cout << \"Hello, world\" <<
    std::endl;}"
  }
}
```

For error responses, see the [response status codes documentation](#response-status-codes).
