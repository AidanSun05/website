FROM node:22.11-alpine AS base
USER node
WORKDIR /code

ENV ASTRO_TELEMETRY_DISABLED=1
COPY --chown=node package*.json .
RUN npm cache clean --force

FROM base AS builder
USER root
RUN apk add --update zip

USER node
RUN npm ci --include=dev

COPY --chown=node . .
RUN chmod +x scripts/copy-downloads.sh && \
  scripts/copy-downloads.sh && \
  node scripts/generate-favicons.js && \
  npm run build && \
  npx svgo -f dist/client/_astro

FROM base AS deps
RUN npm ci --omit=dev

FROM alpine:3.21.0 AS runner
RUN apk add --update nodejs=22.11.0-r0 && \
  addgroup -g 65535 node && \
  adduser --shell /sbin/nologin --disabled-password \
  --no-create-home --uid 65535 --ingroup node node

USER node
WORKDIR /code
COPY --from=deps --chown=node /code/node_modules ./node_modules
COPY --from=builder --chown=node /code/dist ./dist
