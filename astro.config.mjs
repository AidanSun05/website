import mdx from "@astrojs/mdx";
import node from "@astrojs/node";
import sitemap from "@astrojs/sitemap";
import vue from "@astrojs/vue";
import Macros from "@vue-macros/astro";
import robotsTxt from "astro-robots-txt";
import { defineConfig } from "astro/config";
import rehypeKatex from "rehype-katex";
import { extendedTableHandlers, remarkExtendedTable } from "remark-extended-table";
import remarkGfm from "remark-gfm";
import remarkMath from "remark-math";
import supersub from "remark-supersub";

// https://astro.build/config
export default defineConfig({
  site: "https://www.aidansun.com",
  integrations: [mdx(), vue(), Macros(), sitemap(), robotsTxt()],
  markdown: {
    gfm: false,
    remarkPlugins: [
      [
        remarkGfm,
        {
          singleTilde: false
        }
      ],
      supersub,
      remarkMath,
      remarkExtendedTable
    ],
    rehypePlugins: [rehypeKatex],
    remarkRehype: {
      handlers: extendedTableHandlers
    },
    shikiConfig: {
      theme: "github-light"
    }
  },
  output: "hybrid",
  adapter: node({
    mode: "standalone"
  })
});
