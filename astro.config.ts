import sitemap from "@astrojs/sitemap";
import vue from "@astrojs/vue";
import { defineConfig } from "astro/config";
import asciidoc from "./src/asciidoc";

// https://astro.build/config
export default defineConfig({
  site: "https://www.aidansun.com",
  integrations: [asciidoc(), vue(), sitemap()]
});
