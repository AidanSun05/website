FROM node:20.15-alpine AS base
USER node
WORKDIR /code

ENV ASTRO_TELEMETRY_DISABLED=1
COPY --chown=node package*.json .

FROM base AS builder
USER node
RUN npm ci --include=dev

COPY --chown=node . .
RUN node scripts/generate-favicons.js && npm run build && npx svgo -f dist/client/_astro

FROM base AS deps
RUN npm ci --omit=dev

FROM alpine:3.20.0 AS runner
RUN apk add --update nodejs=20.15.1-r0 && \
  addgroup -g 65535 node && \
  adduser --shell /sbin/nologin --disabled-password \
  --no-create-home --uid 65535 --ingroup node node

USER node

WORKDIR /code
COPY --from=deps --chown=node /code/node_modules ./node_modules
COPY --from=builder --chown=node /code/dist ./dist
COPY --chown=node ./res/downloads ./res/downloads
