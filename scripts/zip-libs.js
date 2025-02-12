import archiver from "archiver";
import fs from "fs";
import path from "path";

/**
 * Function to create a zip archive from a directory
 * @param {string} sourceDir - Path to the subdirectory to zip
 * @param {string} outputFilePath - Path where the zip file will be saved
 */
function zipDirectory(sourceDir, outputFilePath) {
  return new Promise((resolve, reject) => {
    const output = fs.createWriteStream(outputFilePath);
    const archive = archiver("zip", { zlib: { level: 9 } });

    output.on("close", () => {
      console.log(`Zipped: ${sourceDir} -> ${outputFilePath} (${archive.pointer()} bytes)`);
      resolve();
    });

    archive.on("error", (err) => reject(err));

    archive.pipe(output);
    archive.directory(sourceDir, false);
    archive.finalize();
  });
}

/**
 * Main function to process directories
 * @param {string} inputDir - Directory containing subdirectories
 * @param {string} outputDir - Directory where zip files will be saved
 */
async function processDirectories(inputDir, outputDir) {
  if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir, { recursive: true });
  }

  const subdirs = fs
    .readdirSync(inputDir, { withFileTypes: true })
    .filter((dirent) => dirent.isDirectory())
    .map((dirent) => dirent.name);

  for (const subdir of subdirs) {
    const sourcePath = path.join(inputDir, subdir);
    const zipPath = path.join(outputDir, `${subdir}.zip`);
    await zipDirectory(sourcePath, zipPath);
  }

  console.log("All libraries have been zipped.");
}

const sourceDirectory = "res/lib";
const destinationDirectory = "dist/dl";

processDirectories(sourceDirectory, destinationDirectory).catch((err) => console.error("Error:", err));
