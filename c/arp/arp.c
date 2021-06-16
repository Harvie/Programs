#define _DEFAULT_SOURCE 1
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <endian.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <string.h>
#include <net/if_arp.h>

uint64_t arp_ip2mac(const char *dev, const char *ip) {
  int ret;
  struct arpreq arpreq;
  int iArpFD;

  struct sockaddr_in *sin = (struct sockaddr_in *) &arpreq.arp_pa;
  uint64_t mac = 0;

  memset(&arpreq, 0, sizeof(struct arpreq));

  strcpy(arpreq.arp_dev, dev);
  //sprintf(arpreq.arp_dev, "eth-pub.%d", vlan);

  iArpFD = socket(AF_INET, SOCK_DGRAM, 0);
  if (iArpFD == -1)
  {
    printf("Cannot open packet socket");
    return 0;
  }

  struct timeval tv = {
    .tv_sec = 0,
    .tv_usec = 200000
  };
  setsockopt(iArpFD, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(struct timeval));

  sin->sin_family = AF_INET;
  sin->sin_addr.s_addr = inet_addr(ip);
  sin->sin_port = htons(4242);

  sendto(iArpFD, "PING", 4, MSG_NOSIGNAL,
               (struct sockaddr*) sin, sizeof(struct sockaddr_in));


  char blah[5];
  if(recvfrom(iArpFD, blah, sizeof(blah), 0,
    (struct sockaddr *) sin, (socklen_t * restrict) sizeof(struct sockaddr)) < 0)
  {
    //printf("UDP ping timeout\n");
  }

  ret = ioctl(iArpFD, SIOCGARP, &arpreq);
  close(iArpFD);

  if(ret < 0) {
    printf("RET: %d FAMILY: %d\n", ret, arpreq.arp_ha.sa_family);
    perror("SIOCGARP");
    return 0;
  }

  memcpy(&mac, &arpreq.arp_ha.sa_data[0], 6);
  mac = be64toh(mac<<16);

  return mac;
}

int main() {
  char *dev = "eth0";
  char *ip = "10.10.161.3";
  uint64_t mac = arp_ip2mac(dev, ip);

  printf("DEV %s IP %s MAC: %.12lX %s\n", dev, ip, mac, mac?"OK":"FAIL");
}

