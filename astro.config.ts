import markdoc from "@astrojs/markdoc";
import node from "@astrojs/node";
import sitemap from "@astrojs/sitemap";
import vue from "@astrojs/vue";
import Macros from "@vue-macros/astro";
import robotsTxt from "astro-robots-txt";
import { defineConfig } from "astro/config";

// https://astro.build/config
export default defineConfig({
  site: "https://www.aidansun.com",
  integrations: [markdoc(), vue(), Macros(), sitemap(), robotsTxt()],
  output: "hybrid",
  adapter: node({ mode: "standalone" })
});
