# daemon runs in the background
# run something like tail /var/log/traaittnetwork/current to see the status
# be sure to run with volumes, ie:
# docker run -v $(pwd)/traaittnetwork:/var/lib/traaittnetwork -v $(pwd)/wallet:/home/traaitt --rm -ti traaitt:0.2.2
ARG base_image_version=0.10.0
FROM phusion/baseimage:$base_image_version

ADD https://github.com/just-containers/s6-overlay/releases/download/v1.21.2.2/s6-overlay-amd64.tar.gz /tmp/
RUN tar xzf /tmp/s6-overlay-amd64.tar.gz -C /

ADD https://github.com/just-containers/socklog-overlay/releases/download/v2.1.0-0/socklog-overlay-amd64.tar.gz /tmp/
RUN tar xzf /tmp/socklog-overlay-amd64.tar.gz -C /

ARG TURTLECOIN_BRANCH=master
ENV TURTLECOIN_BRANCH=${TURTLECOIN_BRANCH}

# install build dependencies
# checkout the latest tag
# build and install
RUN apt-get update && \
    apt-get install -y \
      build-essential \
      python-dev \
      gcc-4.9 \
      g++-4.9 \
      git cmake \
      libboost1.58-all-dev && \
    git clone https://github.com/trrxitte/traaitt.git /src/traaitt && \
    cd /src/traaitt && \
    git checkout $TURTLECOIN_BRANCH && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_CXX_FLAGS="-g0 -Os -fPIC -std=gnu++11" .. && \
    make -j$(nproc) && \
    mkdir -p /usr/local/bin && \
    cp src/TurtleCoind /usr/local/bin/TurtleCoind && \
    cp src/walletd /usr/local/bin/walletd && \
    cp src/zedwallet /usr/local/bin/zedwallet && \
    cp src/miner /usr/local/bin/miner && \
    strip /usr/local/bin/TurtleCoind && \
    strip /usr/local/bin/walletd && \
    strip /usr/local/bin/zedwallet && \
    strip /usr/local/bin/miner && \
    cd / && \
    rm -rf /src/traaitt && \
    apt-get remove -y build-essential python-dev gcc-4.9 g++-4.9 git cmake libboost1.58-all-dev && \
    apt-get autoremove -y && \
    apt-get install -y  \
      libboost-system1.58.0 \
      libboost-filesystem1.58.0 \
      libboost-thread1.58.0 \
      libboost-date-time1.58.0 \
      libboost-chrono1.58.0 \
      libboost-regex1.58.0 \
      libboost-serialization1.58.0 \
      libboost-program-options1.58.0 \
      libicu55

# setup the traaittnetwork service
RUN useradd -r -s /usr/sbin/nologin -m -d /var/lib/traaittnetwork traaittnetwork && \
    useradd -s /bin/bash -m -d /home/traaitt traaitt && \
    mkdir -p /etc/services.d/traaittnetwork/log && \
    mkdir -p /var/log/traaittnetwork && \
    echo "#!/usr/bin/execlineb" > /etc/services.d/traaittnetwork/run && \
    echo "fdmove -c 2 1" >> /etc/services.d/traaittnetwork/run && \
    echo "cd /var/lib/traaittnetwork" >> /etc/services.d/traaittnetwork/run && \
    echo "export HOME /var/lib/traaittnetwork" >> /etc/services.d/traaittnetwork/run && \
    echo "s6-setuidgid traaittnetwork /usr/local/bin/TurtleCoind" >> /etc/services.d/traaittnetwork/run && \
    chmod +x /etc/services.d/traaittnetwork/run && \
    chown nobody:nogroup /var/log/traaittnetwork && \
    echo "#!/usr/bin/execlineb" > /etc/services.d/traaittnetwork/log/run && \
    echo "s6-setuidgid nobody" >> /etc/services.d/traaittnetwork/log/run && \
    echo "s6-log -bp -- n20 s1000000 /var/log/traaittnetwork" >> /etc/services.d/traaittnetwork/log/run && \
    chmod +x /etc/services.d/traaittnetwork/log/run && \
    echo "/var/lib/traaittnetwork true traaittnetwork 0644 0755" > /etc/fix-attrs.d/traaittnetwork-home && \
    echo "/home/traaitt true traaitt 0644 0755" > /etc/fix-attrs.d/traaitt-home && \
    echo "/var/log/traaittnetwork true nobody 0644 0755" > /etc/fix-attrs.d/traaittnetwork-logs

VOLUME ["/var/lib/traaittnetwork", "/home/traaitt","/var/log/traaittnetwork"]

ENTRYPOINT ["/init"]
CMD ["/usr/bin/execlineb", "-P", "-c", "emptyenv cd /home/traaitt export HOME /home/traaitt s6-setuidgid traaitt /bin/bash"]
