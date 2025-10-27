# Use Node.js official image
FROM node:22-bullseye

# Set working directory
WORKDIR /usr/src/app

# Copy package.json and package-lock.json
COPY package*.json ./

# Install dependencies
RUN npm install
RUN npm rebuild node-pty

# Copy backend code
COPY . .

# Expose port (Render will provide PORT)
ENV PORT 10000
EXPOSE 10000

# Start the backend server
CMD ["node", "server.js"]
