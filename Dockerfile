FROM asciidoctor/docker-asciidoctor AS pdfs
WORKDIR /documents
COPY articles ./articles
COPY src/img ./img
COPY res ./res
RUN gem install rghost rouge && \
  asciidoctor-pdf -r asciidoctor-mathematical \
    # Define the icon set:
    -a icons=font -a icon-set=fas \
    -a imagesdir=/documents/img \
    # This is for display in the PDF to turn videos into links:
    -a videosdir=https://www.aidansun.com/videos \
    -a source-highlighter=rouge -a rouge-style=github \
    -a optimize \
    articles/*.adoc

FROM node:23 AS builder
RUN apt update && apt install zip

# Install dependencies (only depends on package*.json files)
WORKDIR /code
ENV ASTRO_TELEMETRY_DISABLED=1
COPY package*.json .
RUN npm cache clean --force && npm ci --include=dev

# Build
COPY . .
RUN chmod +x scripts/copy-downloads.sh && \
  scripts/copy-downloads.sh && \
  node scripts/generate-favicons.js && \
  npm run build && \
  npx svgo -f dist/_astro

FROM nginx:alpine-slim AS runner
ARG CONFIG

RUN rm -f /etc/nginx/conf.d/default.conf
COPY ./nginx/include /etc/nginx/conf.d/include
COPY nginx/${CONFIG}.conf /etc/nginx/conf.d/default.conf
COPY --from=builder /code/dist /usr/share/nginx/html
COPY --from=pdfs /documents/articles/*.pdf /usr/share/nginx/html/pdf/
