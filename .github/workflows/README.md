# Action Workflows

Github actions for CI/CD

## Build

Builds libdaisy validating the source code builds with no errors.

## Style

Runs a style check using clang-format to make sure that all files within the src/ hierarchy conform to the proper style.

This can be tested locally by running:

```bash
./ci/run-clang-format.py -r src
```

## Build Docs

This runs steps to generate the documentation to confirm there are no errors.

This runs on PRs and merges to master.

Worth mentioning that this **does not fail on warnings** and is mostly provided to give useful feedback to see what doesn't have documentation.

## Deploy Docs

This runs the same build step as [Build Docs](#build-docs)

This is only run when something is pushed to master, rebuilds the docuemntation using Doxygen, and deploys it to the gh-pages branch.

This does not build and push the pdf yet...

View the documentation [here](https://electro-smith.github.io/libDaisy/index.html)