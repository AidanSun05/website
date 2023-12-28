FROM nginx:1.25-alpine-slim AS base
ARG CONFIG

RUN rm -f /etc/nginx/conf.d/default.conf
COPY ${CONFIG} /etc/nginx/conf.d/default.conf
