# Website

This repository contains the source code for aidansun.com. The website is built with:

- Astro (backend)
- Vue.js (frontend)
- Docker (deployment)
- DigitalOcean (hosting)

## Docker

This website is deployed using Docker and Docker Compose. Docker Compose files:

- `docker-compose.yml`: Base Compose file.
- `docker-compose.override.yml`: Local development, mounts the repository root in the container.
- `docker-compose.prod.yml`: Production, includes nginx and TLS configured.
- `docker-compose.staging.yml`: For testing use, like the production file but without TLS.

## Setup

```shell
npm i            # Install dependencies
npm run favicons # Generate favicon files

npm run dev      # Start development server
```

## Editor Configuration

EditorConfig, ESLint, Prettier, and VSCode configuration files are included in the repository.

### VSCode Extensions

- [Astro](https://marketplace.visualstudio.com/items?itemName=astro-build.astro-vscode)
- [EditorConfig for VS Code](https://marketplace.visualstudio.com/items?itemName=EditorConfig.EditorConfig)
- [ESLint](https://marketplace.visualstudio.com/items?itemName=dbaeumer.vscode-eslint)
- [language-postcss](https://marketplace.visualstudio.com/items?itemName=cpylua.language-postcss)
- [MDX](https://marketplace.visualstudio.com/items?itemName=unifiedjs.vscode-mdx)
- [nginx.conf hint](https://marketplace.visualstudio.com/items?itemName=hangxingliu.vscode-nginx-conf-hint)
- [Npm Dependency](https://marketplace.visualstudio.com/items?itemName=howardzuo.vscode-npm-dependency)
- [Prettier - Code formatter](https://marketplace.visualstudio.com/items?itemName=esbenp.prettier-vscode)
- [Vue Language Features (Volar)](https://marketplace.visualstudio.com/items?itemName=Vue.volar)
