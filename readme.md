# Website

This repository contains the source code for aidansun.com. The website is built with:

- Astro (backend)
- Vue.js (frontend)
- Nginx (server)
- Docker (deployment)
- DigitalOcean (hosting)

## Docker

This website is deployed using Docker and Docker Compose. Docker Compose files:

- `docker-compose.yml`: For testing use, like the production file but without TLS.
- `docker-compose.prod.yml`: Production, includes nginx and TLS configured.

Run development container locally:

```shell
docker compose up --build
```

Run production container locally:

```shell
docker compose -f docker-compose.prod.yml up --build
```

## Setup

```shell
npm i            # Install dependencies
npm run favicons # Generate favicon files

npm run dev      # Start development server
```

## Software

- **Code**: VSCode
- **SVGs** (`/src/img/**.svg`): Inkscape
- [**Schematics**](https://github.com/AidanSun05/website-schematics): KiCad

## License

This website's code and assets are licensed under the [MIT License](/LICENSE.txt).

This repository also contains the Arduino libraries that are featured on the site in `res/downloads/`; each has its own MIT license document.
