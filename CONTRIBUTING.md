# Contribute to FeudOS

FeudOS is originally my personal project, but it's always open to contributions.

### Opening an issue

Make sure you're on the latest commit and that there isn't already an issue for the same problem. Give as much useful information as you can.

# Now the code contributing

## What to work on

* If you want something smaller (or not), you can look for TODOs in the code: `grep -irn "todo" .`
* Bigger and more general goals are listed in `todo.md`.
* If you want to add something cool and useful (like a driver), go for it. If you 
want it to eventually be merged into the kernel, I'd suggest opening an issue first to discuss it.

## Code standards / conventions

* 1TBS brace formatting style.
* Prefer modern C23 style, e.g. `[[gnu::packed]]` instead of `__attribute__((packed))`.
* Use the increment/decrement operators only in `for` loops.
* Keep lines at 80 characters or fewer.
* If a function name is an uncommon abbreviation, write a doc comment (`///`) explaining what it means.

## LLM standpoint

If you use LLMs or coding agents, you **must** review any generated code for 
coding standards, correctness, security, and similar issues before submitting it.

Handwritten code is preferred.

