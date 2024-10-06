import * as readline from "readline/promises";
import fs from "fs/promises";
import path from "path";

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

console.log("Creating a new article");

try {
  const createImages = await rl.question("Enter 'y' to create images directory > ") == "y";

  const filename = await rl.question("Enter file name > ");
  const title = JSON.stringify(await rl.question("Enter title > "));
  const description = JSON.stringify(await rl.question("Enter description > "));
  const tags = await rl.question("Enter tags separated by commas > ");
  const date = new Date().toISOString().split("T")[0];

  let outText = `---\ntitle: ${title}\ndescription: ${description}\n`;
  if (createImages) outText += `image: ${filename}/thumbnail.jpg\n`;

  outText += `date: ${date}\ntags:\n`
  for (const tag of tags.split(",")) outText += `  - ${tag.trim()}\n`;

  outText += "---\n\n## Heading\n";

  await fs.writeFile(path.join(process.cwd(), "src", "content", "articles", `${filename}.mdoc`), outText);
  if (createImages) await fs.mkdir(path.join(process.cwd(), "src", "img", filename));
} finally {
  rl.close();
}
