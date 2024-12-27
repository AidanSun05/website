import type { ImageMetadata } from "astro";
import { getImage } from "astro:assets";
import JSDOM from "jsdom";

export default async function optimizeImages(
  images: { [key: string]: ImageMetadata | undefined },
  html: string
): Promise<string> {
  const htmlDoc = new JSDOM.JSDOM(html);
  for (const image of [...htmlDoc.window.document.getElementsByTagName("img")]) {
    // Only replace images that have optimized counterparts
    const optimizedSrc = images[image.src];
    if (!optimizedSrc) continue;

    // Apply dimensions from asciidoc if defined
    const width = image.width > 0 ? image.width : undefined;
    const height = image.height > 0 ? image.height : undefined;
    const getImageResult = await getImage({ src: optimizedSrc, width, height, format: "webp" });
    image.src = getImageResult.src;

    // Set additional attributes for optimization
    for (const [prop, val] of Object.entries(getImageResult.attributes)) image.setAttribute(prop, val);
  }

  return htmlDoc.window.document.body.innerHTML;
}
