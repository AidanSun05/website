import { favicons, type FaviconOptions, type FaviconResponse } from "favicons";
import fs from "fs/promises";
import path from "path";

async function main() {
  const iconPath = "src/img/favicon.svg";
  const outPath = "public/favicons";
  const manifestPath = "src/generated/manifest.astro";

  const configuration: FaviconOptions = {
    path: "/favicons",
    background: "#555",
    theme_color: "#555"
  };

  let response: FaviconResponse;

  try {
    response = await favicons(iconPath, configuration);
  } catch (error) {
    console.log((error as Error).message);
    return;
  }

  await fs.mkdir(outPath);
  await fs.mkdir(path.dirname(manifestPath));

  // Write images and data files
  await Promise.all(
    response.images.map(async (image) => await fs.writeFile(path.join(outPath, image.name), image.contents))
  );
  await Promise.all(
    response.files.map(async (file) => await fs.writeFile(path.join(outPath, file.name), file.contents))
  );

  // Write manifest to be included at compile time
  await fs.writeFile(manifestPath, response.html.join("\n"));
}

main();
