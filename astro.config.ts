import asciidoc from "./src/asciidoc";
import sitemap from "@astrojs/sitemap";
import vue from "@astrojs/vue";
import { defineConfig } from "astro/config";

// https://astro.build/config
export default defineConfig({
  site: "https://www.aidansun.com",
  integrations: [asciidoc(), vue(), sitemap()]
});
