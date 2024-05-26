FROM nginx:1.26-alpine-slim AS base
ARG CONFIG

RUN rm -f /etc/nginx/conf.d/default.conf
COPY ./ssl-aidansun.conf /etc/nginx/conf.d/ssl-aidansun.conf
COPY ${CONFIG} /etc/nginx/conf.d/default.conf
