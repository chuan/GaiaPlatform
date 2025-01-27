#!/usr/bin/env bash

###################################################
# Copyright (c) Gaia Platform LLC
#
# Use of this source code is governed by the MIT
# license that can be found in the LICENSE.txt file
# or at https://opensource.org/licenses/MIT.
###################################################

# Simple function to start the process off.
start_process() {
    local DIRECTORY_TO_INSTALL_TO=$1

    if [ "$VERBOSE_MODE" -ne 0 ]; then
        echo "Installing the $PROJECT_NAME project to directory $DIRECTORY_TO_INSTALL_TO"
    fi
}

# Simple function to stop the process, including any cleanup.
complete_process() {
    local SCRIPT_RETURN_CODE=$1
    local COMPLETE_REASON=$2

    if [ -n "$COMPLETE_REASON" ] ; then
        echo "$COMPLETE_REASON"
    fi

    if [ "$SCRIPT_RETURN_CODE" -ne 0 ]; then
        echo "Installation of the $PROJECT_NAME project failed."
    else
        if [ "$VERBOSE_MODE" -ne 0 ]; then
            echo "Installation of the $PROJECT_NAME project succeeded."
        fi
    fi
    exit "$SCRIPT_RETURN_CODE"
}

# Show how this script can be used.
show_usage() {
    local SCRIPT_NAME=$0

    echo "Usage: $(basename "$SCRIPT_NAME") [flags] <directory>"
    echo "Flags:"
    echo "  -v,--verbose      Display detailed information during execution."
    echo "  -h,--help         Display this help text."
    echo "Other:"
    echo "  directory         Directory to create and install the project into."
    exit 1
}

# Parse any command line values.
parse_command_line() {
    VERBOSE_MODE=0
    PARAMS=()
    while (( "$#" )); do
    case "$1" in
        -v|--verbose)
            VERBOSE_MODE=1
            shift
        ;;
        -h|--help)
            show_usage
        ;;
        -*) # unsupported flags
            echo "Error: Unsupported flag $1" >&2
            show_usage
        ;;
        *) # preserve positional arguments
            PARAMS+=("$1")
            shift
        ;;
    esac
    done
}

# Calculate where to install the project to and that it is in the right state
# for that install to continue.
calculate_install_directory() {
    INSTALL_DIRECTORY=${PARAMS[0]}

    if [ -z "$INSTALL_DIRECTORY" ]
    then
        echo "Directory to install the project to was not provided."
        show_usage
    fi
    length=${#INSTALL_DIRECTORY}
    last_char=${INSTALL_DIRECTORY:length-1:1}
    [[ $last_char != "/" ]] && INSTALL_DIRECTORY="$INSTALL_DIRECTORY/"; :

    # Make sure it does not already exist.
    if [ -d "$INSTALL_DIRECTORY" ]; then
        complete_process 1 "Directory '$(realpath "$INSTALL_DIRECTORY")' already exists. Please specify a new directory to create."
    fi

    if [ -f "$INSTALL_DIRECTORY" ]; then
        complete_process 1 "'$(realpath "$INSTALL_DIRECTORY")' already exists as a file. Please specify a new directory to create."
    fi
}

# Remove any dynamic directories so that the build is really clean.
remove_dynamic_directory() {
    local DYNAMIC_DIRECTORY_PATH=$1

    if [ "$DYNAMIC_DIRECTORY_PATH" == "/" ]; then
        complete_process 1 "Removing the specified directory '$DYNAMIC_DIRECTORY_PATH' is dangerous. Aborting."
    fi

    if [ -d "$DYNAMIC_DIRECTORY_PATH" ]; then
        if ! rm -rf "$DYNAMIC_DIRECTORY_PATH"; then
            complete_process 1 "Existing '$(realpath "$DYNAMIC_DIRECTORY_PATH")' directory not removed from the '$(realpath "$INSTALL_DIRECTORY")' directory."
        fi
    fi
}

# Copy the example into the directory.
install_into_directory() {
    if ! cp -rf "$SCRIPTPATH" "$INSTALL_DIRECTORY"; then
        complete_process 1 "Project $PROJECT_NAME cannot be copied into the '$(realpath "$INSTALL_DIRECTORY")' directory."
    fi
    remove_dynamic_directory "$INSTALL_DIRECTORY/$BUILD_DIRECTORY"
    remove_dynamic_directory "$INSTALL_DIRECTORY/$LOG_DIRECTORY"
    remove_dynamic_directory "$INSTALL_DIRECTORY/$TEST_RESULTS_DIRECTORY"

    # ...then go into that directory.
    if ! cd "$INSTALL_DIRECTORY"; then
        complete_process 1 "Cannot change the current directory to the '$(realpath "$INSTALL_DIRECTORY")' directory."
    fi
}

# Set up any global script variables.
# shellcheck disable=SC2164
SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
# shellcheck disable=SC1091 source=./properties.sh
source "$SCRIPTPATH/properties.sh"

parse_command_line "$@"

calculate_install_directory

# Clean entrance into the script.
start_process "$INSTALL_DIRECTORY"

install_into_directory

# If we get here, we have a clean exit from the script.
complete_process 0
