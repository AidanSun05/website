import type { Asciidoctor, Document, ProcessorOptions, Section } from "@asciidoctor/core";
import asciidoctor from "@asciidoctor/core";
import mathJaxProcessor from "@djencks/asciidoctor-mathjax";
import type { MarkdownHeading } from "astro";
import JSDOM from "jsdom";
import path from "path";
import * as shiki from "shiki";

type Catalog = {
  [key: string]: unknown;
  includes: {
    [key: string]: unknown;
    $$keys: string[];
  };
};

function getHeadings(doc: Document): MarkdownHeading[] {
  const tocLevels = <number>doc.getAttribute("toclevels", 2);
  return doc
    .findBy({ context: "section" }, (b) => b.getLevel() > 0 && b.getLevel() <= tocLevels)
    .map((b) => {
      const section = <Section>b;
      return {
        text: section.isNumbered() ? `${section.getSectionNumber()} ${section.getName()}` : section.getName(),
        slug: section.getId(),
        depth: section.getLevel()
      };
    });
}

function getIncludes(file: string, catalog: Catalog): string[] {
  return catalog.includes.$$keys.map((include) => path.join(path.dirname(file), include));
}

async function registerHighlighters(registry: Asciidoctor["SyntaxHighlighter"]) {
  const highlighter = await shiki.getSingletonHighlighter({
    langs: ["cpp", "python", "html", "css", "xml"],
    themes: ["github-light"]
  });

  registry.register("shiki", {
    handlesHighlighting() {
      return true;
    },
    highlight(_node, source, lang, opts) {
      const html = highlighter.codeToHtml(source, { lang, theme: "github-light" });

      // Apply line highlighting
      if (opts.highlight_lines === undefined) return html;
      const htmlDoc = new JSDOM.JSDOM(html);
      for (const [i, tag] of [...htmlDoc.window.document.getElementsByClassName("line")].entries())
        if (opts.highlight_lines.includes(i + 1)) tag.classList.add("highlight-line");

      // Document has full HTML structure so only get the <pre> tag
      return htmlDoc.window.document.getElementsByTagName("pre")[0].outerHTML;
    },
    format(node, _lang, _opts) {
      return node.getContent();
    }
  });
}

export default class AsciidocConverter {
  private converter;
  private extensionsRegistered;

  constructor() {
    this.converter = asciidoctor();
    this.extensionsRegistered = false;
  }

  getAttributes(fileUrl: URL, contents: string) {
    // Extract attributes (all content above 2 consecutive newlines) to avoid parsing entire document
    const attrs = /(.+?)(?:\r?\n){2}/s.exec(contents);
    let data;
    if (attrs !== null) {
      const doc = this.converter.load(attrs[0]);
      data = doc.getAttributes();
    }
    return {
      data,
      body: contents.trim(),
      slug: path.basename(fileUrl.toString(), ".adoc"),
      rawData: JSON.stringify(data)
    };
  }

  async convert(file: string) {
    if (!this.extensionsRegistered) {
      await registerHighlighters(this.converter.SyntaxHighlighter);
      mathJaxProcessor.register(this.converter.Extensions);
      this.extensionsRegistered = true;
    }

    const options = {
      safe: "unsafe",
      attributes: {
        "source-highlighter": "shiki",
        relfileprefix: "/articles/",
        relfilesuffix: "/",
        imagesdir: "@img",
        videosdir: "/videos"
      }
    };

    const doc = this.converter.loadFile(file, options);
    const html = doc.convert(<ProcessorOptions>{
      standalone: false,
      ...options
    });

    return {
      html,
      headings: getHeadings(doc),
      includes: getIncludes(file, <Catalog>doc.getCatalog())
    };
  }
}
