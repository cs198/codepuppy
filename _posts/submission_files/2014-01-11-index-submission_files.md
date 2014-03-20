---
category: Submission files
path: '/submissions/[:submission_id]/submission_files'
title: 'Get files by submission'
type: 'GET'

layout: nil
---

Retrieve all files for the given submission.

## Request

* Valid submission ID in URL.

## Response

Sends back a list of submission files for the given submission.

```Status: 200 OK```
```
{
  [
    {
      id: "71",
      submission_id: "10",
      original_filename: "hello.cpp",
      data: "#include <iostream>\nint main() {\nstd::cout << \"Hello, world\" <<
      std::endl;}"
    },
    {
      id: "72",
      submission_id: "10",
      original_filename: "goodbye.cpp",
      data: "#include <iostream>\nint main() {\nstd::cout << \"Goodbye, world\" <<
      std::endl;}"
    },

  ]
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
