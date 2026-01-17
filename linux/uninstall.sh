#!/usr/bin/env bash
set -e

APP_NAME="neospoorkl"
TARGET_DIR="$HOME/.local/bin"
LINK_PATH="$TARGET_DIR/$APP_NAME"

if [[ -e "$LINK_PATH" && ! -L "$LINK_PATH" ]]; then
    echo "Refusing to overwrite non-symlink: $LINK_PATH"
    exit 1
fi

if [[ -L "$LINK_PATH" ]]; then
    rm "$LINK_PATH"
    echo "removed symlink: $LINK_PATH"
else
    echo "no symlink found at: $LINK_PATH"
fi

