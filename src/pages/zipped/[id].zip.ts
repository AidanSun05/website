import archiver from "archiver";
import type { APIContext } from "astro";
import NodeCache from "node-cache";
import path from "path";

export const prerender = false;

// Generated zips are cached for 15 minutes to reduce server load
const zipCache = new NodeCache({
  stdTTL: 900
});

export async function GET({ params }: APIContext) {
  // Validate requested download id
  const { id } = params;
  const allDownloads = ["DS130X", "SevenSegmentShield", "SSD1306_I2C"];

  if (!id || !allDownloads.includes(id)) return new Response(null, { status: 404, statusText: "Download not found" });

  // Response headers
  const headers: HeadersInit = {
    "Content-Type": "application/zip",
    "Content-Disposition": `attachment; filename="${id}.zip"`
  };

  // Check for cache
  const cached = zipCache.get<Buffer>(id);
  if (cached) return new Response(cached, { headers });

  // Generate zip file
  const chunks = [];
  const target = archiver.create("zip");

  const absPath = path.join(process.cwd(), "res", "downloads", id);

  await target.directory(absPath, false).finalize();
  for await (const data of target) chunks.push(data);

  // Add generated zip to cache
  const finalBuf = Buffer.concat(chunks);
  zipCache.set(id, finalBuf);

  return new Response(finalBuf, { headers });
}
