import { defineCollection, z } from "astro:content";
import { glob } from "astro/loaders";

const articles = defineCollection({
  loader: glob({ pattern: "**/[^_]*.adoc", base: "./articles" }),
  schema: ({ image }) => z.object({
    doctitle: z.string(),
    description: z.string(),
    image: image().optional(),
    revdate: z.coerce.date(),
    tags: z.string(),
    stylesheets: z.string().optional()
  }),
});

export const collections = { articles };
