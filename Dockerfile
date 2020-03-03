FROM ademus4/root-6-14:latest
USER root
WORKDIR /work
ENV HOME /work
ENV ROOTSYS /usr/local/bin/root
ENV CLAS12ROOT /work/clas12root/
ENV PATH="$PATH:$CLAS12ROOT/bin"

# make cmake3 the default
RUN ln -s cmake3 /usr/bin/cmake

# copy current branch into directory
COPY . /work/clas12root/

# build
RUN cd clas12root && ./installC12Root

