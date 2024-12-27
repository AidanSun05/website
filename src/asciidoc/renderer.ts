import type { NamedSSRLoadedRendererValue, SSRResult } from "astro";
import { AstroJSX, jsx } from "astro/jsx-runtime";
import { renderJSX } from "astro/runtime/server/index.js";

// This is a stripped version of astro/jsx/server.js which was removed in Astro 5.

export async function check(Component: unknown, props: Record<string, unknown>) {
  if (typeof Component !== "function") return false;
  try {
    const result = await Component({ ...props });
    return result[AstroJSX];
  } catch {
    return false;
  }
}

export async function renderToStaticMarkup(this: { result: SSRResult }, Component: unknown, props = {}) {
  const { result } = this;
  const html = await renderJSX(result, jsx(Component, { ...props }));
  return { html };
}

const renderer: NamedSSRLoadedRendererValue = { name: "astro:jsx", check, renderToStaticMarkup };
export default renderer;
