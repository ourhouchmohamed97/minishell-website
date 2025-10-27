# Use a Linux Node image
FROM node:22-bullseye

# Set working directory
WORKDIR /usr/src/app

# Copy package files and install dependencies
COPY backend/package*.json ./
RUN npm install
RUN npm rebuild node-pty

# Copy backend code including your Linux minishell
COPY backend ./

# Make sure minishell is executable
RUN chmod +x minishell

# Set Render port and expose
ENV PORT 10000
EXPOSE 10000

# Start server
CMD ["node", "server.js"]
