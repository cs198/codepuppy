---
category: Sections
path: '/section/[:section_id]/students'
title: 'Get students in a section'
type: 'GET'

layout: nil
---

Retrieve a list of students in a section.

## Request

* Valid section ID in URL.

## Response

Sends back a list of people who are in the section.

```Status: 200 OK```
```
{
  [
    {
      id: 3,
      given_name: "Anthony",
      family_name: "Mainero",
      user_system_id: "amainero"
    },
    {
      id: 4,
      given_name: "Reid",
      family_name: "Watson",
      user_system_id: "rawatson"
    }
  ]
}
```

For errors responses, see the [response status codes documentation](#response-status-codes).
