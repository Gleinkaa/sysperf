# Agent Rules for sysperf

## Git Operations
Nik handles ALL git operations himself (commit, push, pull, fetch, merge, rebase).
The agent MUST NOT run any git commands in this repository.
The agent only does implementation work — reads code, writes answers.

## Communication Protocol
Nik writes `issue<N>.md` and pushes it.
The agent reads it, does the work, and writes `answer<N>.md`.
Communication happens via git-synced issue/answer markdown files.
