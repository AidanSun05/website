import fs from "fs/promises";
import path from "path";

/**
 * Function to filter paths in a directory by extension
 * @param {string[]} paths - Path to filter from
 * @param {string[]} exts - Extensions to preserve
 */
function filterPaths(paths, exts) {
  return paths.filter((name) => exts.some((ext) => path.extname(name) === ext));
}

/**
 * Function to find paths in a directory with output paths relative to the working directory
 * @param {string} dir - Directory to read from
 * @param {string[]} exts - Extensions to preserve
 */
async function findFiles(dir, exts) {
  return filterPaths(await fs.readdir(dir, { recursive: true }), exts).map((i) => path.join(dir, i));
}

// Astro's emitESMImage (used in image() in content collections and the Asciidoc integration) outputs original, unoptimized images in _astro/.
// These images are not actually referenced in the generated HTML.
// This script finds these images and removes them to save space.

const files = await findFiles("dist", [".html", ".js", ".css"]);
const assets = await findFiles("dist/_astro/", [".jpg", ".jpeg", ".png", ".gif", ".svg", ".webp"]);

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
