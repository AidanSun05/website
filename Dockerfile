FROM node:23 AS builder
RUN apt update && apt install zip

USER node
WORKDIR /code

ENV ASTRO_TELEMETRY_DISABLED=1
COPY --chown=node . .
RUN npm cache clean --force && \
  npm ci --include=dev && \
  chmod +x scripts/copy-downloads.sh && \
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
