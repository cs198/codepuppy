---
category: Submission files
path: '/submission_files/[:id]'
title: 'Get a submission file by id'
type: 'GET'

layout: nil
---

Retrive a submission file by its ID.

## Request

* Valid submission file ID in URL.

## Response

Sends back a submission file with the corresponding ID.

```Status: 200 OK```
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

For errors responses, see the [response status codes documentation](#response-status-codes).
