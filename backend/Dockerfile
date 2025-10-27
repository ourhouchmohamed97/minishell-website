# Use a Linux Node image with a newer GLIBC
FROM node:22-bullseye

# Set working directory
WORKDIR /usr/src/app

# Copy package files and install dependencies
COPY package*.json ./
RUN npm install
RUN npm rebuild node-pty

# Copy all backend code (including Linux minishell binary)
COPY . .

# Make sure minishell is executable
RUN chmod +x minishell

# Set Render port and expose
ENV PORT 10000
EXPOSE 10000

# Start server
CMD ["node", "server.js"]
