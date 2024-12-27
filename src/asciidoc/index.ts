import type { AstroIntegration, ContentEntryType, HookParameters } from "astro";
import AsciidocConverter from "./asciidoctor";
import getContentEntryType from "./content-entry-type";

// Non-exposed APIs
type ExtendedParams = HookParameters<"astro:config:setup"> & {
  addContentEntryType: (contentEntryType: ContentEntryType) => void;
};

export default function asciidoc(): AstroIntegration {
  const converter = new AsciidocConverter();

  return {
    name: "asciidoc",
    hooks: {
      "astro:config:setup": async (params) => {
        const { addContentEntryType, addRenderer } = <ExtendedParams>params;

        addRenderer({
          name: "astro:jsx",
          serverEntrypoint: "@asciidoc/renderer"
        });
        addContentEntryType(await getContentEntryType(converter, params.config));
      }
    }
  };
}
