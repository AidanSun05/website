#!/usr/bin/env bash

cd res/lib
for SUBDIR in *; do zip -r -q "../../public/dl/${SUBDIR}.zip" "$SUBDIR"; done

cd ../..
npm run build
npx svgo -f dist/_astro
node scripts/clean-assets.js
