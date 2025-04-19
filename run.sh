#/bin/env bash
SCRIPT_PATH="$(readlink -f "$0")"
SCRIPT_DIR="$(dirname $SCRIPT_PATH)"

HISTFILE=$SCRIPT_DIR/build/.run_args_history
HISTSIZE=1000
HISTCONTROL=ignoredups
history -r # load previous history

read -e -p 'Enter args: ' RUN_ARGS
history -s -- "$RUN_ARGS" # save current entry
history -w # write to history file

exec "$1" $RUN_ARGS
