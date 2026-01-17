#!/usr/bin/env bash
set -e

APP_NAME="neospoorkl"
TARGET_DIR="$HOME/.local/bin"
LINK_PATH="$TARGET_DIR/$APP_NAME"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APP_PATH="$SCRIPT_DIR/$APP_NAME"

# sanity checks
if [[ ! -f "$APP_PATH" ]]; then
    echo "error: $APP_NAME not found next to install.sh"
    exit 1
fi

if [[ ! -x "$APP_PATH" ]]; then
    echo "making $APP_NAME executable"
    chmod +x "$APP_PATH"
fi

mkdir -p "$TARGET_DIR"

# create or replace symlink
ln -sf "$APP_PATH" "$LINK_PATH"

echo "installed symlink:"
echo "  $LINK_PATH -> $APP_PATH"

# check PATH
case ":$PATH:" in
  *":$TARGET_DIR:"*) ;;
  *)
    echo
    echo "WARNING: $TARGET_DIR is not in your PATH."
    echo "add this to your shell config:"
    echo "  export PATH=\"\$PATH:$TARGET_DIR\""
    ;;
esac

echo
echo "you can now run:"
echo "  $APP_NAME"
echo "without ./ and also anywhere"
