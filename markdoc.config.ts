import { component, defineMarkdocConfig, nodes } from "@astrojs/markdoc/config";

export default defineMarkdocConfig({
  nodes: {
    // Custom styled blockquote
    blockquote: {
      ...nodes.blockquote,
      render: component("@components/nodes/Blockquote.astro")
    },
    // Inline code, math expressions
    code: {
      render: component("@components/nodes/Code.astro"),
      attributes: {
        content: { type: String }
      }
    },
    // Code blocks and embedding content from files
    fence: {
      render: component("@components/nodes/Fence.astro"),
      attributes: {
        language: { type: String },
        content: { type: String }
      }
    }
  },
  tags: {
    alert: {
      render: component("@components/tags/Alert.astro"),
      attributes: {
        type: { type: String }
      }
    },
    collapsiblepane: {
      render: component("@components/tags/CollapsiblePaneClient.astro"),
      attributes: {
        title: { type: String }
      }
    },
    tabbar: {
      render: component("@components/tags/TabBarClient.astro"),
      attributes: {
        id: { type: String },
        titles: { type: Array }
      }
    },
    tabpane: {
      render: component("@components/tags/TabPaneClient.astro"),
      attributes: {
        id: { type: String },
        index: { type: Number }
      }
    },
    video: {
      render: component("@components/tags/Video.astro"),
      attributes: {
        src: { type: String }
      }
    }
  }
});
