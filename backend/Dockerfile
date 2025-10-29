FROM ubuntu:20.04

# Set non-interactive frontend to avoid timezone prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies for both minishell and Node.js
RUN apt-get update && apt-get install -y \
    nodejs \
    npm \
    gcc \
    make \
    libreadline-dev \
    python3 \
    python3-pip \
    build-essential \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Install Node.js 18+ (Ubuntu 20.04 has old Node.js by default)
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash - \
    && apt-get install -y nodejs

# Set working directory
WORKDIR /app

# Copy package files first for better caching
COPY package*.json ./

# Install Node.js dependencies
RUN npm install

# Copy all source code (including minishell source)
COPY . .

# Build minishell
RUN make

# Expose port (Render will use PORT environment variable)
EXPOSE 10000

# Start server
CMD ["node", "server.js"]