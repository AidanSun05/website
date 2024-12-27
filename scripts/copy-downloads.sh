#!/usr/bin/env bash

mkdir public/dl

for SUBDIR in res/downloads/*; do
  if [ -d "$SUBDIR" ]; then
    # Get the name of the subdirectory without the full path
    SUBDIR_NAME=$(basename "$SUBDIR")

    # Create a ZIP file for the subdirectory
    ZIP_FILE="public/dl/${SUBDIR_NAME}.zip"
    echo "Archiving '$SUBDIR' into '$ZIP_FILE'..."
    zip -r -q "$ZIP_FILE" "$SUBDIR"
  fi
done

find res/downloads -maxdepth 1 -type f | xargs cp -t public/dl
