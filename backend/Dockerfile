# Stage 1: build minishell
FROM debian:bullseye AS builder
RUN apt-get update && apt-get install -y build-essential
WORKDIR /usr/src/app
COPY . .
RUN make minishell    # or whatever command builds your binary

# Stage 2: runtime
FROM node:22-bullseye
WORKDIR /usr/src/app
COPY backend/package*.json ./
RUN npm install
RUN npm rebuild node-pty
COPY --from=builder /usr/src/app/minishell ./
RUN chmod +x ./minishell
ENV PORT 10000
EXPOSE 10000
CMD ["node", "server.js"]
