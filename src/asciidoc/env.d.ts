type Extensions = typeof import("@asciidoctor/core").Extensions;

declare module "@djencks/asciidoctor-mathjax" {
  const mathJaxProcessor: {
    register: (_: Extensions) => void;
  };
  export default mathJaxProcessor;
}
