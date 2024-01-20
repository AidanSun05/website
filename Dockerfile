FROM node:20.10-alpine AS base
USER node
WORKDIR /code

ENV ASTRO_TELEMETRY_DISABLED=1
COPY --chown=node package*.json .

FROM base AS builder
USER node
RUN npm ci --include=dev

COPY --chown=node . .
RUN npm run favicons && npm run build && npm run svgs

FROM base AS deps
RUN npm ci --omit=dev

FROM alpine:3.19 AS runner
RUN apk add --update nodejs=20.11.0-r0 && \
  addgroup -g 65535 node && \
  adduser --shell /sbin/nologin --disabled-password \
  --no-create-home --uid 65535 --ingroup node node

USER node

WORKDIR /code
COPY --from=deps --chown=node /code/node_modules ./node_modules
COPY --from=builder --chown=node /code/dist ./dist
COPY --chown=node ./res/downloads ./res/downloads
