# Use a lightweight Python image
FROM python:3.9-slim-buster

# Install Playwright dependencies
RUN apt-get update && apt-get install -y \
    libnss3 \
    libatk1.0-0 \
    libatk-bridge2.0-0 \
    libcups2 \
    libxkbcommon0 \
    libxcomposite1 \
    libxrandr2 \
    libgbm1 \
    libpango-1.0-0 \
    libasound2 \
    libgtk-3-0 \
    && rm -rf /var/lib/apt/lists/*

# Install Playwright
RUN pip install playwright

# Create the ctf user and set up working directory
RUN useradd -r -m ctf

# Switch to the non-root user to install Playwright browsers
USER ctf

# Set the environment variable to use a custom path for the Playwright cache
ENV PLAYWRIGHT_BROWSERS_PATH=/home/ctf/.cache/ms-playwright

# Install the Playwright browsers as the ctf user
RUN playwright install

# Set up the working directory
WORKDIR /app

# Copy the requirements file and install dependencies as root
USER root
COPY requirements.txt ./
RUN pip install -r requirements.txt

# Copy the bot script and .env file into the container
COPY . /app

# Set permissions for the app directory to allow access by the ctf user
RUN chown -R ctf /app

# Switch back to the non-root user
USER ctf

# Expose the Flask port
EXPOSE 4444

# Run the Flask app with Gunicorn
CMD ["gunicorn", "-b", "0.0.0.0:4444", "-w", "1", "admin:app"]
