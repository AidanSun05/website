import fs from "fs/promises";
import path from "path";

// Astro's emitESMImage (used in image() in content collections and the Asciidoc integration) outputs original, unoptimized images in _astro/.
// These images are not actually referenced in the generated HTML.
// This script finds these images and removes them to save space.

async function main() {
  const files = fs.glob("dist/**/*.{html,js,css}");
  const assets = fs.glob("dist/_astro/*.{jpg,jpeg,png,gif,svg,webp}");

  const referencedImages = new Set();
  for await (const file of files) {
    const content = await fs.readFile(file, "utf-8");
    const imageMatches = content.match(/\/_astro\/[^\s]+?\.(?:jpg|jpeg|png|gif|svg|webp)/g) || [];
    for (const i of imageMatches) referencedImages.add(path.basename(i));
  }

  let removedFiles = 0;
  for await (const file of assets) {
    if (!referencedImages.has(path.basename(file))) {
      await fs.unlink(file);
      removedFiles++;
    }
  }
  console.log(`Removed ${removedFiles} unused assets`);
}

await main();
