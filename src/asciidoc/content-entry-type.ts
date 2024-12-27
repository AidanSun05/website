import type { AstroConfig, ContentEntryType, ImageMetadata } from "astro";
import { emitESMImage } from "astro/assets/utils";
import JSDOM from "jsdom";
import fs from "node:fs";
import { fileURLToPath } from "node:url";
import path from "path";
import type { PluginContext } from "rollup";
import AsciidocConverter from "./asciidoctor";

// This is loosely based on the @astrojs/markdoc integration

function isValidUrl(str: string): boolean {
  try {
    new URL(str);
    return true;
  } catch {
    return false;
  }
}

function prependForwardSlash(str: string) {
  return str[0] === "/" ? str : "/" + str;
}

async function emitOptimizedImage(
  ctx: PluginContext,
  astroConfig: AstroConfig,
  fileUrl: URL,
  image: string
): Promise<ImageMetadata | undefined> {
  // Optimize anything that is NOT external or an absolute path to public
  if (isValidUrl(image) || image.startsWith("/")) return undefined;

  const filePath = fileURLToPath(fileUrl);
  const resolved = await ctx.resolve(image, filePath);

  if (!resolved?.id || !fs.existsSync(new URL(prependForwardSlash(resolved.id), "file://")))
    throw new Error(`Could not resolve image ${image} from ${filePath}`);

  const src = await emitESMImage(resolved.id, ctx.meta.watchMode, !!astroConfig.experimental.svg, ctx.emitFile);
  if (!src) {
    console.warn("Could not emit ESM image for:", image);
    return undefined;
  }

  const fsPath = resolved.id;
  if (astroConfig.output === "static")
    if (globalThis.astroAsset.referencedImages) globalThis.astroAsset.referencedImages.add(fsPath);

  return src;
}

export default async function getContentEntryType(
  converter: AsciidocConverter,
  astroConfig: AstroConfig
): Promise<ContentEntryType> {
  return {
    extensions: [".adoc"],
    getEntryInfo({ fileUrl, contents }) {
      return converter.getAttributes(fileUrl, contents);
    },
    handlePropagation: true,
    async getRenderModule({ fileUrl, viteId }) {
      if (!viteId.endsWith(".adoc")) return;

      // Extract images from HTML rather than Asciidoctor so inline images and <img> from passthrough can be detected
      const doc = await converter.convert(viteId);
      const htmlDoc = new JSDOM.JSDOM(doc.html);

      const imagesOut: { [key: string]: ImageMetadata | undefined } = {};
      for (const el of htmlDoc.window.document.getElementsByTagName("img")) {
        const image = el.src;
        if (imagesOut[image]) continue;

        imagesOut[image] = await emitOptimizedImage(this, astroConfig, fileUrl, image);
      }

      const code = `import { Fragment, jsx as h } from "astro/jsx-runtime";
import optimizeImages from "@asciidoc/optimize-assets";
export function getHeadings() { return ${JSON.stringify(doc.headings)}; }
export async function Content() {
  const content = h(Fragment, { "set:html": await optimizeImages(${JSON.stringify(
    imagesOut
  )}, ${JSON.stringify(doc.html)}) });
  return content;
}
export default Content;`;

      if (this.meta.watchMode) {
        for (const i of doc.includes) {
          const fullPath = path.extname(i) === "" ? i + ".adoc" : i;
          if (fs.existsSync(fullPath)) this.addWatchFile(fullPath);
        }
      }

      return { code, vite: { lang: "ts" } };
    },
    contentModuleTypes: await fs.promises.readFile(
      new URL("./types/content-module-types.d.ts", import.meta.url),
      "utf-8"
    )
  };
}
