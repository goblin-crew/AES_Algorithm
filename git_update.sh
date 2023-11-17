#!/bin/bash

function push_changes_to_git {
    git add --all &&
    git commit --message="update" &&
    git push
}

function pull_from_git {
    git pull
}


case a in $1
    -p|--push)
        push_changes_to_git
        ;;
    -f|--pull)
        pull_from_git
        ;;
    *)
        push_changes_to_git
        ;;
esac