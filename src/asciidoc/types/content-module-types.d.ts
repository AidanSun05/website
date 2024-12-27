interface AsciiDocInstance {
  doctitle: object;
  description: object;
  image: object;
  revdate: object;
  tags: object;
  stylesheets: object;
}

declare module "astro:content" {
  interface Render {
    ".adoc": Promise<AsciiDocInstance>;
  }
}
