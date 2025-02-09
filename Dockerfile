FROM node:23 AS builder
RUN apt update && apt install zip

# Install dependencies (only depends on package*.json files)
WORKDIR /code
ENV ASTRO_TELEMETRY_DISABLED=1
COPY package*.json .
RUN npm cache clean --force && npm ci --include=dev

# Build
COPY . .
RUN chmod +x scripts/build.sh && ./scripts/build.sh

FROM nginx:alpine-slim AS runner
ARG CONFIG

RUN rm -f /etc/nginx/conf.d/default.conf
COPY ./nginx/include /etc/nginx/conf.d/include
COPY nginx/${CONFIG}.conf /etc/nginx/conf.d/default.conf
COPY --from=builder /code/dist /usr/share/nginx/html
