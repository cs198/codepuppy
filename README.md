# Updating API docs

* All API docs are in the `_posts/` folder.
  * The files are just [Markdown](http://daringfireball.net/projects/markdown/)
    documents...
  * ... that have a YAML header to distinguish what kind of entry into the API
    documentation it is. See some of the files for examples of valid options.
* Modify an existing file, or make a new one.
  * This API doc system relies on timestamps to determine ordering (it makes me
    sad, too) - so any new files you add need to have a date at the start of the
    filename, like the others.
* To see the results, in the root directory of the repo run:
```
jekyll serve --watch
```
* That's it!

## What powers this documentation?

Github Pages supports using a static blogging platform called
[Jekyll](http://jekyllrb.com/). The documentation here is running on a skin for
turning Jekyll blogs into API documentation, called
[Carte](https://github.com/devo-ps/carte). @osdiab slightly modified it to
display things more nicely.
