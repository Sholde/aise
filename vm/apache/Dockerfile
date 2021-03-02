FROM debian
RUN apt-get update && \
    apt-get install -y nginx apache2 && \
    apt-get clean
EXPOSE tcp/80
CMD ["nginx", "apache2"]