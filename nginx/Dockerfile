FROM nginx:1.26-alpine-slim AS base
ARG CONFIG

RUN rm -f /etc/nginx/conf.d/default.conf
COPY ./include /etc/nginx/conf.d/include
COPY ${CONFIG} /etc/nginx/conf.d/default.conf
