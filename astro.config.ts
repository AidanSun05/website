import asciidoc from "./src/asciidoc";
import node from "@astrojs/node";
import sitemap from "@astrojs/sitemap";
import vue from "@astrojs/vue";
import { defineConfig } from "astro/config";

// https://astro.build/config
export default defineConfig({
  site: "https://www.aidansun.com",
  integrations: [asciidoc(), vue(), sitemap()],
  adapter: node({ mode: "standalone" })
});
